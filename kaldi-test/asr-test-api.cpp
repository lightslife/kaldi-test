
#include "stdafx.h"

#include "asr-test-api.h"
#include "online-nnet3-decoding.h"
#include "outputText.h"
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
				outputText(*asrShareResource->wordSymbol, olabel, &resultText);
			}
		}

		//final result
		if (num_seconds_decoded > 0.1) {
			decoder.FinalizeDecoding();
			bool end_of_utterance = true;
			std::vector<int> olabel;
			std::vector<std::wstring> resultText;
			decoder.GetBestPath(end_of_utterance, &olabel);
			outputText(*asrShareResource->wordSymbol, olabel,&resultText);

			//³¤¶Î¾²Òô
			if (resultText.size() == 0)
				*more_data = false;
		}
		return 0;
	}
	int asrOnlineLoop(AsrShareOpt *asrShareOpt, AsrShareResource *asrShareResource, WaveDataInfo *waveDataInfo) {
		bool more_data = true;
		while (more_data) {
			asrSegment(&more_data, asrShareOpt, asrShareResource, waveDataInfo);
		}
		return 0;
	}



}//namespace