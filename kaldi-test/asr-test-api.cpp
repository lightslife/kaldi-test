
#include "stdafx.h"

#include "asr-test-api.h"
#include "online-nnet3-decoding.h"
#include "outputText.h"
#include "nnet-utils.h"
namespace kaldi {


	int asrSegment(bool *more_data, AsrShareOpt *asrShareOpt, AsrShareResource *asrShareResource, WaveDataInfo *waveDataInfo) {

		OnlineNnet2FeaturePipeline feature_pipeline(*(asrShareOpt->feature_info));

		SingleUtteranceNnet3Decoder decoder(*asrShareOpt->decoder_opts, *asrShareResource->trans_model,
			*asrShareOpt->decodable_info,
			asrShareResource->wfst, &feature_pipeline);
		Vector<BaseFloat> wave_part = Vector<BaseFloat>(waveDataInfo->chunk_length);
		BaseFloat last_traceback = 0.0;
		BaseFloat num_seconds_decoded = 0.0;

		while (true) {

			*more_data = wave_part.ReadFromQueue(&(waveDataInfo->waveQueue));

			feature_pipeline.AcceptWaveform(waveDataInfo->sample_rate, wave_part);
			if (!*more_data) {
				feature_pipeline.InputFinished();
			}

			decoder.AdvanceDecoding();
			num_seconds_decoded += 1.0 * wave_part.Dim() / waveDataInfo->sample_rate;
			//waveDataInfo->total_time_decoded += 1.0 * wave_part.Dim() / waveDataInfo->sample_rate;

			if (!*more_data) {
				break;
			}
			//endpoint
			bool do_endpoint = true;
			if (do_endpoint && (decoder.NumFramesDecoded() > 0) &&decoder.EndpointDetected(*asrShareOpt->endpoint_opts))
				break;
			//partial result
			if ((num_seconds_decoded - last_traceback > waveDataInfo->traceback_period_secs)
				&& (decoder.NumFramesDecoded() > 0)) {
				bool end_of_utterance = true;
				std::vector<int> olabel;
				std::vector<std::wstring> resultText;
				decoder.GetBestPath(end_of_utterance, &olabel);
				outputText(asrShareResource->wordSymbol, olabel, &resultText);
			}
		}

		//final result
		if (num_seconds_decoded > 0.1) {
			decoder.FinalizeDecoding();
			bool end_of_utterance = true;
			std::vector<int> olabel;
			std::vector<std::wstring> resultText;
			decoder.GetBestPath(end_of_utterance, &olabel);
			outputText(asrShareResource->wordSymbol, olabel,&resultText);
			std::cout << std::endl;
			//长段静音
			if (resultText.size() == 0)
				*more_data = false;
		}
		return 0;
	}
	int asrOnlineLoop(AsrShareOpt *asrShareOpt, AsrShareResource *asrShareResource, WaveDataInfo *waveDataInfo) {
		Timer timer;
		bool more_data = true;
		while (more_data) {
			asrSegment(&more_data, asrShareOpt, asrShareResource, waveDataInfo);
		}
		std::cout <<"used time ="<< timer.Elapsed() << std::endl;

		return 0;
	}

	int asrLoadResource(const char* wordsName, const char*modelName, const char* wfstName, AsrShareResource *asrShareResource) {

		//读入词表
		//std::vector<std::wstring> wordSymbol;
		readSymbol(wordsName, &(asrShareResource->wordSymbol)  );



		//加载声学模型
	//	std::string nnet3_rxfilename = "final.mdl";
		TransitionModel *trans_model=new TransitionModel();
		nnet3::AmNnetSimple *am_nnet=new nnet3::AmNnetSimple();
		{
			bool binary;
			Input ki(modelName, &binary);
			trans_model->Read(ki.Stream(), binary);
			am_nnet->Read(ki.Stream(), binary);
			SetBatchnormTestMode(true, &(am_nnet->GetNnet()));
			SetDropoutTestMode(true, &(am_nnet->GetNnet()));
			//nnet3::CollapseModel(nnet3::CollapseModelConfig(), &(am_nnet.GetNnet()));
		}

		 
		std::filebuf file;
		file.open(wfstName, std::ios::in | std::ios::binary);
		std::istream is(&file);

		Wfst *wfst = new Wfst();
		wfst->ReadHead(is);
		wfst->wfstRead(is);

		//asrShareResource->wordSymbol = &wordSymbol;
		(asrShareResource)->am_nnet = am_nnet;
		(asrShareResource)->trans_model = trans_model;
		(asrShareResource)->wfst = wfst;

		return 0;
	}

	int asrSetWaveInfo(WaveDataInfo *waveDataInfo) {


		waveDataInfo->chunk_length = 400;
		waveDataInfo->sample_rate = 8000;
		waveDataInfo->traceback_period_secs = 0.25;
		return 0;
	}

	int asrSetShareOpt(AsrShareOpt *asrShareOpt, AsrShareResource* asrShareResource){

		OnlineNnet2FeaturePipelineConfig *feature_opts=new OnlineNnet2FeaturePipelineConfig();
		nnet3::NnetSimpleLoopedComputationOptions *decodable_opts =new nnet3::NnetSimpleLoopedComputationOptions();
		LatticeFasterDecoderConfig *decoder_opts =new LatticeFasterDecoderConfig();
		OnlineEndpointConfig *endpoint_opts =new OnlineEndpointConfig();

		BaseFloat chunk_length_secs = 0.18;
		bool do_endpointing = false;
		bool online = true;

		OnlineNnet2FeaturePipelineInfo *feature_info=new OnlineNnet2FeaturePipelineInfo(*feature_opts);
		nnet3::DecodableNnetSimpleLoopedInfo *decodable_info =new nnet3::DecodableNnetSimpleLoopedInfo(*decodable_opts,
			asrShareResource->am_nnet);


		OnlineNnet2FeaturePipeline *feature_pipeline =new OnlineNnet2FeaturePipeline(*feature_info);

		SingleUtteranceNnet3Decoder *decoder= new SingleUtteranceNnet3Decoder(*decoder_opts, *asrShareResource->trans_model,
			*decodable_info,
			asrShareResource->wfst, feature_pipeline);



		asrShareOpt->decodable_info = decodable_info;
		asrShareOpt->decodable_opts = decodable_opts;
		asrShareOpt->decoder_opts = decoder_opts;
		asrShareOpt->endpoint_opts = endpoint_opts;
		asrShareOpt->feature_info = feature_info;
		asrShareOpt->feature_opts = feature_opts;



		return 0;
	}

}//namespace