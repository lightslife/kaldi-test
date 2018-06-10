// kaldi-test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "kaldi-types.h"
#include <iostream>
#include "transition-model.h"
#include "am-nnet-simple.h"
#include "decodable-simple-looped.h"
#include "nnet-utils.h"
#include "online-nnet2-feature-pipeline.h"
#include "lattice-faster-online-decoder.h"
#include "online-endpoint.h"
#include "online-nnet3-decoding.h"
#include "wave-reader.h"
int main()
{


	using namespace kaldi;

	typedef kaldi::int32 int32;
	typedef kaldi::int64 int64;

	std::string nnet3_rxfilename = "final.mdl";
	TransitionModel trans_model;
	nnet3::AmNnetSimple am_nnet;
	{
		bool binary;
		Input ki(nnet3_rxfilename, &binary);
		trans_model.Read(ki.Stream(), binary);
		am_nnet.Read(ki.Stream(), binary);
		SetBatchnormTestMode(true, &(am_nnet.GetNnet()));
		SetDropoutTestMode(true, &(am_nnet.GetNnet()));
		//nnet3::CollapseModel(nnet3::CollapseModelConfig(), &(am_nnet.GetNnet()));
	}


	OnlineNnet2FeaturePipelineConfig feature_opts;
	nnet3::NnetSimpleLoopedComputationOptions decodable_opts;
	LatticeFasterDecoderConfig decoder_opts;
	OnlineEndpointConfig endpoint_opts;

	BaseFloat chunk_length_secs = 0.18;
	bool do_endpointing = false;
	bool online = true;

	OnlineNnet2FeaturePipelineInfo feature_info(feature_opts);
	nnet3::DecodableNnetSimpleLoopedInfo decodable_info(decodable_opts,
		&am_nnet);
 

	const char * name = "HCLG.fst";
	std::filebuf file;
	file.open("HCLG.fst", std::ios::in | std::ios::binary);
	std::istream is(&file);

	Wfst *wfst;
	wfst->ReadHead(is);
	wfst->wfstRead(is);

	OnlineNnet2FeaturePipeline feature_pipeline(feature_info);
	SingleUtteranceNnet3Decoder decoder(decoder_opts, trans_model,
		decodable_info,
		wfst, &feature_pipeline);


	WaveData wave_data;

	const char * wavename = "test.wav";
	std::filebuf wavefile;
	wavefile.open(wavename, std::ios::in | std::ios::binary);
	std::istream iswave(&wavefile);

	wave_data.Read(iswave);
	 
	// get the data for channel zero (if the signal is not mono, we only
	// take the first channel).
	SubVector<BaseFloat> data(wave_data.Data(), 0);




	BaseFloat samp_freq = wave_data.SampFreq();
	int32 chunk_length;
	if (chunk_length_secs > 0) {
		chunk_length = int32(samp_freq * chunk_length_secs);
		if (chunk_length == 0) chunk_length = 1;
	}
	else {
		chunk_length = std::numeric_limits<int32>::max();
	}

	int32 samp_offset = 0;
	std::vector<std::pair<int32, BaseFloat> > delta_weights;

	while (samp_offset < data.Dim()) {
		int32 samp_remaining = data.Dim() - samp_offset;
		int32 num_samp = chunk_length < samp_remaining ? chunk_length
			: samp_remaining;

		SubVector<BaseFloat> wave_part(data, samp_offset, num_samp);
		feature_pipeline.AcceptWaveform(samp_freq, wave_part);

		samp_offset += num_samp;
		//decoding_timer.WaitUntil(samp_offset / samp_freq);
		if (samp_offset == data.Dim()) {
			// no more input. flush out last frames
			feature_pipeline.InputFinished();
		}

		decoder.AdvanceDecoding();

		if (do_endpointing && decoder.EndpointDetected(endpoint_opts)) {
			break;
		}
	}
	decoder.FinalizeDecoding();

    return 0;
}

