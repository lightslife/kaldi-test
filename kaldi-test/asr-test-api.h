#pragma once
#ifndef _ASR_TEST_API_H__
#define _ASR_TEST_API_H__

#include "online-nnet2-feature-pipeline.h"
#include "online-endpoint.h"
#include "decodable-simple-looped.h"
#include "lattice-faster-online-decoder.h"
#include "wfst-read.h"
#include <queue>

namespace kaldi {

	typedef struct AsrShareOpt {
		kaldi::OnlineNnet2FeaturePipelineInfo *feature_info;
		kaldi::OnlineNnet2FeaturePipelineConfig *feature_opts;
		kaldi::nnet3::NnetSimpleLoopedComputationOptions *decodable_opts;
		kaldi::LatticeFasterDecoderConfig *decoder_opts;
		kaldi::OnlineEndpointConfig *endpoint_opts;
		kaldi::nnet3::DecodableNnetSimpleLoopedInfo *decodable_info;
	}AsrShareOpt, *AsrShareOptPst;

	typedef struct AsrShareResource {
		kaldi::Wfst *wfst;
		kaldi::nnet3::AmNnetSimple *am_nnet;
		kaldi::TransitionModel *trans_model;
		std::vector<std::wstring> wordSymbol;
	}AsrShareResource, *AsrShareResourcePst;

	typedef struct WaveDataInfo {
		int chunk_length;
		BaseFloat traceback_period_secs;
		int sample_rate;
		std::queue<short> waveQueue;
	}WaveDataInfo, *WaveDataInfoPst;
	 
	int asrSegment(bool *more_data, AsrShareOpt *asrShareOpt, AsrShareResource *asrShareResource, WaveDataInfo *waveDataInfo);
	int asrOnlineLoop(AsrShareOpt *asrShareOpt, AsrShareResource *asrShareResource, WaveDataInfo *waveDataInfo);

	int asrLoadResource(const char* wordsName, const char*modelName,const char* wfstName, AsrShareResource *asrShareResource);
	int asrSetWaveInfo(WaveDataInfo *waveDataInfo);
	int asrSetShareOpt(AsrShareOpt *asrShareOpt, AsrShareResource* asrShareResource);

}//namespace
#endif // !_ASR_TEST_API_H__
