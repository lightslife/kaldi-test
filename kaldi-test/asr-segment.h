#pragma once
#ifndef _ASR_TEST_API_H__
#define _ASR_TEST_API_H__

#include "online-nnet2-feature-pipeline.h"
#include "online-endpoint.h"
#include "decodable-simple-looped.h"
#include "lattice-faster-online-decoder.h"
#include "wfst-read.h"
#include <queue>
#include "online-nnet3-decoding.h"
#include <map>
#include <vector>
#include <string>
#include "ThreadPool.h"
#include <mutex>

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
		std::vector<std::wstring> *wordSymbol;
	}AsrShareResource, *AsrShareResourcePst;



	typedef struct WaveDataInfo_old {
		int chunk_length;
		BaseFloat traceback_period_secs;
		int sample_rate;
		std::queue<short> waveQueue;
	}WaveDataInfo_old, *WaveDataInfoPst;
	

	struct WaveSpliceData {
		int length;
		float *data;
		int num_record;
		WaveSpliceData(){
			length = 0;
			data = NULL;
			num_record = 0;
		}
		WaveSpliceData(const WaveSpliceData &src) {
			this->data = (float*)malloc(length*sizeof(float));
			memcpy(this->data, src.data, length*sizeof(float));
		}

		WaveSpliceData& operator =(const WaveSpliceData& src) {
			this->data = (float*)malloc(length * sizeof(float));
			memcpy(this->data, src.data, length * sizeof(float));
		}

		~WaveSpliceData() {
			if (data != NULL)
				free(data);
		}
	};



	struct WaveDataInfo {
		int chunk_length;
		BaseFloat traceback_period_secs;
		int sample_rate;
		int num_pushed;
		bool eos;//客户端主动结束
		bool flag_end; //长静音，引擎识别结束。

	};


	struct DecoderSaveState {
		SingleUtteranceNnet3Decoder *decoder;//something wrong.
		OnlineNnet2FeaturePipeline *feature_pipeline;
		int num_done;
	};
	struct ONE_CONSUMER {
		WaveDataInfo *waveDataInfo;
		DecoderSaveState *decoderSaveState;
		std::queue<WaveSpliceData> waveData;
	};

	typedef struct Asr_Init_RESOURCE_STRU {
		AsrShareResource *asrShareResource;
		AsrShareOpt *asrShareOpt;
		ThreadPool *pool;
		std::map<std::string, ONE_CONSUMER> *task_all;
		std::mutex *mtxMap;
		int stop; //0 stands for not need   ;      1 stands for need stop            ; 2 stands for stop done;
	}Asr_Init_RESOURCE_STRU, *Asr_Init_RESOURCE_PST;



	int asrSegmentSplice(AsrShareOpt *asrShareOpt, AsrShareResource *asrShareResource, WaveSpliceData waveSpliceData, DecoderSaveState *decoderState, WaveDataInfo *waveDataInfo);


	int asrSegment(bool *more_data, AsrShareOpt *asrShareOpt, AsrShareResource *asrShareResource, WaveDataInfo_old *waveDataInfo);
	int asrOnlineLoop(AsrShareOpt *asrShareOpt, AsrShareResource *asrShareResource, WaveDataInfo_old *waveDataInfo);

	int asrLoadResource(const char* wordsName, const char*modelName,const char* wfstName, AsrShareResource *asrShareResource);
	int asrSetWaveInfo(WaveDataInfo_old *waveDataInfo);
	int asrSetShareOpt(AsrShareOpt *asrShareOpt, AsrShareResource* asrShareResource);

}//namespace
#endif // !_ASR_TEST_API_H__
