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
#include <fstream>
#include <vector>
#include <iostream>
#include<locale>  
#include <codecvt>
using namespace kaldi;
int outputText(std::vector<std::wstring> wordSymbol, std::vector<int> olabel) {

	std::locale china("chs");
	std::wcout.imbue(china);
	for (int i = olabel.size() - 1; i >= 0; i--) {
		int idState = olabel[i];
		std::wcout << wordSymbol[idState] << " ";
	}
	std::cout << std::endl;
	return 0;
}
struct AsrShareOpt {
	kaldi::OnlineNnet2FeaturePipelineInfo *feature_info;
	kaldi::OnlineNnet2FeaturePipelineConfig feature_opts;
	kaldi::nnet3::NnetSimpleLoopedComputationOptions decodable_opts;
	kaldi::LatticeFasterDecoderConfig decoder_opts;
	kaldi::OnlineEndpointConfig endpoint_opts;
	kaldi::nnet3::DecodableNnetSimpleLoopedInfo decodable_info;
};

struct AsrShareResource {
	kaldi::Wfst *wfst;
	kaldi::nnet3::AmNnetSimple *am_nnet;
	kaldi::TransitionModel *trans_model;
	std::vector<std::wstring> *wordSymbol;
};
struct WaveDataInfo {
	int chunk_length;
	BaseFloat traceback_period_secs;
	int sample_rate;
};
int asrSegment(bool more_data,AsrShareOpt *asrShareOpt, AsrShareResource *asrShareResource ,WaveDataInfo *waveDataInfo) {
	kaldi::OnlineNnet2FeaturePipeline feature_pipeline(*(asrShareOpt->feature_info));

	kaldi::SingleUtteranceNnet3Decoder decoder(asrShareOpt->decoder_opts, *asrShareResource->trans_model,
		asrShareOpt->decodable_info,
		asrShareResource->wfst, &feature_pipeline);
	Vector<BaseFloat> wave_part = Vector<BaseFloat>(waveDataInfo->chunk_length);
	BaseFloat last_traceback = 0.0;
	BaseFloat num_seconds_decoded = 0.0;

	while (true) {

	//more_data = filter->audio_source->Read(&wave_part);
		 
	feature_pipeline.AcceptWaveform(waveDataInfo->sample_rate, wave_part);
	if (!more_data) {
		feature_pipeline.InputFinished();
	}

	 decoder.AdvanceDecoding();
	 num_seconds_decoded += 1.0 * wave_part.Dim() / waveDataInfo->sample_rate;
	//waveDataInfo->total_time_decoded += 1.0 * wave_part.Dim() / waveDataInfo->sample_rate;
	
	if (!more_data) {
		break;
		}
 
		//partial result
		if ((num_seconds_decoded - last_traceback > waveDataInfo->traceback_period_secs)
			&& (decoder.NumFramesDecoded() > 0)) {
			bool end_of_utterance = true;
			std::vector<int> olabel;
			decoder.GetBestPath(end_of_utterance, &olabel);
			outputText(*asrShareResource->wordSymbol, olabel);
		}
	}

	//final result
	if (num_seconds_decoded > 0.1) {
		decoder.FinalizeDecoding();
		bool end_of_utterance = true;
		std::vector<int> olabel;
		decoder.GetBestPath(end_of_utterance, &olabel);
		outputText(*asrShareResource->wordSymbol, olabel);
	}
	return 0;
}
int asrOnlineLoop(AsrShareOpt *asrShareOpt, AsrShareResource *asrShareResource, WaveDataInfo *waveDataInfo) {
	bool more_data = true;
	while (more_data) {
		asrSegment(more_data,asrShareOpt, asrShareResource, waveDataInfo);
	}
	return 0;
}

int main()
{


	using namespace kaldi;

	typedef kaldi::int32 int32;
	typedef kaldi::int64 int64;


	std::ifstream wordsFile("words.txt");
	//if (!wordsFile.is_open());
	//return -1;
	std::string temp;
	std::vector<std::wstring> wordSymbol;
	std::locale china("chs");
	std::wcout.imbue(china);
	std::wstring wtemp;
	std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;

	while (std::getline(wordsFile, temp)) {
		wtemp = strCnv.from_bytes(temp);
		wordSymbol.push_back(wtemp);
		//std::wcout << wtemp << std::endl;
	}



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
 



	OnlineNnet2FeaturePipeline feature_pipeline(feature_info);



	const char * name = "HCLG.fst.vector";
	std::filebuf file;
	file.open(name, std::ios::in | std::ios::binary);
	std::istream is(&file);

	Wfst *wfst = new Wfst();
	wfst->ReadHead(is);
	wfst->wfstRead(is);
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

	std::vector<int> olabel;
	decoder.GetBestPath(true, &olabel);

	outputText(wordSymbol, olabel);


	int xx = 0;

    return 0;
}

