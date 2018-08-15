#include "stdafx.h"
#include <thread>
#include <chrono>
#include "asr-online-api.h"
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
#include "outputText.h"
#include "asr-segment.h"

using namespace kaldi;
typedef kaldi::int32 int32;
typedef kaldi::int64 int64;
template std::map<std::string, ONE_CONSUMER>;


int test()
{


//AsrShareResource asrShareResource;
//AsrShareOpt asrShareOpt;
//WaveDataInfo_old waveDataInfo;

////模拟语音送入，可以使用两个线程分开送语音和识别
//WaveData wave_data;
//const char * wavename = "../16k-model/down42_2.wav";
//std::filebuf wavefile;
//wavefile.open(wavename, std::ios::in | std::ios::binary);
//std::istream iswave(&wavefile);
//wave_data.ReadQueue(iswave, &(waveDataInfo.waveQueue));




////加载资源，共享资源
//asrLoadResource("words.txt", "../16k-model/final.mdl", "../16k-model/HCLG.fst.vector", &asrShareResource);
//asrSetWaveInfo(&waveDataInfo);
//asrSetShareOpt(&asrShareOpt, &asrShareResource);


////开始识别
//asrOnlineLoop(&asrShareOpt, &asrShareResource, &waveDataInfo);



AsrShareResource asrShareResource;
AsrShareOpt asrShareOpt;

//加载资源，共享资源
asrLoadResource("words.txt", "../16k-model/final.mdl", "../16k-model/HCLG.fst.vector", &asrShareResource);
asrSetShareOpt(&asrShareOpt, &asrShareResource);


for (int i = 0; i < 20; i++) {

	WaveDataInfo_old waveDataInfo;

	//模拟语音送入，可以使用两个线程分开送语音和识别
	const char * wavename = "../16k-model/test.wav";
	FILE *fp;
	fp = fopen(wavename, "rb");
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	short *data = new short[length / 2];
	fread(data, sizeof(short), length / 2, fp);

	for (int i = 0; i < length / 2; i++)
	{
		waveDataInfo.waveQueue.push(data[i]);
	}
	asrSetWaveInfo(&waveDataInfo);
	//开始识别
	asrOnlineLoop(&asrShareOpt, &asrShareResource, &waveDataInfo);

}


//system("pause");
return 0;
}



//资源初始化
int asr_online_resource_init(
	void **pHandle,
	const char*acModel,
	const char*wordsFile,
	const char*decoderGraph
) {
	Asr_Init_RESOURCE_STRU *asr_Resource = new Asr_Init_RESOURCE_STRU();
	AsrShareResource *asrShareResource =new AsrShareResource();
	AsrShareOpt *asrShareOpt=new AsrShareOpt();
	asr_Resource->asrShareResource = asrShareResource;
	asr_Resource->asrShareOpt = asrShareOpt;

	asrLoadResource(wordsFile, acModel, decoderGraph, (asr_Resource->asrShareResource));

	asrSetShareOpt((asr_Resource->asrShareOpt), (asr_Resource->asrShareResource));

	*pHandle = asr_Resource;

	return 0;

}

void server_asr_pool(void *pHandle) {
	Asr_Init_RESOURCE_STRU *asr_Resource = (Asr_Init_RESOURCE_STRU *)pHandle;
	ThreadPool &pool = *asr_Resource->pool;

	std::map<std::string, ONE_CONSUMER> &task_all = *(asr_Resource->task_all);
	std::map<std::string, ONE_CONSUMER>::const_iterator iterMap;
	int &stop = asr_Resource->stop;

	while (stop == 0 || task_all.size() > 0) {
		iterMap = task_all.begin();

		for (; iterMap != task_all.end(); ++iterMap) {
			std::string key = iterMap->first;

			WaveDataInfo *&waveDataInfo = task_all[key].waveDataInfo;

			if (waveDataInfo->flag_end  ) { 
				//释放资源
				task_all.erase(iterMap);
				break;
			}

			if (task_all[key].waveData.size() == 0) {
				continue;
			}

			WaveSpliceData &wave_splice = task_all[key].waveData.front();
			DecoderSaveState *decoder_splice = task_all[key].decoderSaveState;

			if (wave_splice.num_record == decoder_splice->num_done+1) {

				pool.enqueue(asrSegmentSplice, asr_Resource->asrShareOpt, asr_Resource->asrShareResource, wave_splice, decoder_splice, waveDataInfo);

				task_all[key].waveData.pop();
			}
		}//map size loop

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}//while loop

	stop = 2;
}


int asr_online_start_server(void *pHandle, int num_thread) {
	Asr_Init_RESOURCE_STRU *asr_Resource = (Asr_Init_RESOURCE_STRU *)pHandle;
	ThreadPool *pool = new ThreadPool(num_thread);
	std::map<std::string, ONE_CONSUMER> *task_all = new std::map<std::string, ONE_CONSUMER>();
	std::mutex *mtxMap = new std::mutex();

	asr_Resource->pool = pool;
	asr_Resource->task_all = task_all;
	asr_Resource->mtxMap = mtxMap;
	std::thread server_pool(server_asr_pool, pHandle);
	server_pool.detach();

	return 0;
}

int asr_online_consumer_init(const char *userId, void *pHandle) {
	Asr_Init_RESOURCE_STRU *asr_Resource = (Asr_Init_RESOURCE_STRU *)pHandle;
	std::unique_lock<std::mutex> lock(*asr_Resource->mtxMap);

	std::map<std::string, ONE_CONSUMER> *task_all = asr_Resource->task_all;

	ONE_CONSUMER one_people_task;

	//(*task_all)[userId].decoderSaveState = new DecoderSaveState();
	//(*task_all)[userId].waveDataInfo = new WaveDataInfo();


	OnlineNnet2FeaturePipeline *feature_pipeline = new OnlineNnet2FeaturePipeline(*(asr_Resource->asrShareOpt->feature_info));
	SingleUtteranceNnet3Decoder *decoder = new SingleUtteranceNnet3Decoder(*(asr_Resource->asrShareOpt->decoder_opts), *(asr_Resource->asrShareResource->trans_model),
		*(asr_Resource->asrShareOpt->decodable_info),
		asr_Resource->asrShareResource->wfst, feature_pipeline);

	DecoderSaveState  *decoderSaveState = new DecoderSaveState();
	decoderSaveState->num_done = 0;
	decoderSaveState->decoder = decoder;
	decoderSaveState->feature_pipeline = feature_pipeline;
	decoderSaveState->last_trackback = 0.0;
	decoderSaveState->num_seconds_decoded = 0.0;

	WaveDataInfo * waveDataInfo = new WaveDataInfo();
	waveDataInfo->chunk_length = 400;
	waveDataInfo->eos = false;
	waveDataInfo->flag_end = false;
	waveDataInfo->num_pushed = 0;
	waveDataInfo->sample_rate = 16000;
	waveDataInfo->traceback_period_secs = 0.40;
	
	one_people_task.decoderSaveState = decoderSaveState;
	one_people_task.waveDataInfo = waveDataInfo;

	task_all->emplace(std::pair<std::string, ONE_CONSUMER>(userId, one_people_task));
	return 0;
}

int asr_online_consumer_decode(const char *userId, short *srcdata, int length, void *pHandle) {
	Asr_Init_RESOURCE_STRU *asr_Resource = (Asr_Init_RESOURCE_STRU *)pHandle;
	std::unique_lock<std::mutex> lock(*asr_Resource->mtxMap);

	std::map<std::string, ONE_CONSUMER> &task_all = *(asr_Resource->task_all);
	ONE_CONSUMER &this_people = task_all[userId];

	int &num_record = task_all[userId].waveDataInfo->num_pushed;
	//short to float 

	float *wavedata = (float*)malloc(length * sizeof(float));

	for (int i = 0; i < length; i++) {
		wavedata[i] = (float)srcdata[i];
	}
	WaveSpliceData waveSpliceData;
	waveSpliceData.data = wavedata;
	waveSpliceData.length = length;
	waveSpliceData.num_record = ++num_record; //from 1 2 3...
	this_people.waveData.emplace(waveSpliceData);
	return 0;
}


int asr_online_consumer_finish(const char *userId, void *pHandle) {
	Asr_Init_RESOURCE_STRU *asr_Resource = (Asr_Init_RESOURCE_STRU *)pHandle;
	std::unique_lock<std::mutex> lock(*asr_Resource->mtxMap);

	std::map<std::string, ONE_CONSUMER> &task_all = *(asr_Resource->task_all);
	ONE_CONSUMER &this_people = task_all[userId];
	this_people.waveDataInfo->eos = true;
	//TODO 发送结束标识

	int &num_record = task_all[userId].waveDataInfo->num_pushed;
	//short to float 

	float *wavedata = (float*)malloc(3* sizeof(float));
	wavedata[0] =wavedata[1]= wavedata[2] = 0;
 
	WaveSpliceData waveSpliceData;
	waveSpliceData.data = wavedata;
	waveSpliceData.length = 3;
	waveSpliceData.num_record = ++num_record; //from 1 2 3...
	this_people.waveData.emplace(waveSpliceData);






	return 0;
}

int asr_online_stop_server(void *pHandle) {
	Asr_Init_RESOURCE_STRU *asr_Resource = (Asr_Init_RESOURCE_STRU *)pHandle;
	std::unique_lock<std::mutex> lock(*asr_Resource->mtxMap);

	int  &stop = asr_Resource->stop;

	stop = 1;
	while (stop != 2) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}

int asr_online_release_resource(void *pHandle) {
	Asr_Init_RESOURCE_STRU *asr_Resource = (Asr_Init_RESOURCE_STRU *)pHandle;

	if (asr_Resource->asrShareOpt != NULL) {
		delete asr_Resource->asrShareOpt;
		asr_Resource->asrShareOpt = NULL;
	}
	if (asr_Resource->asrShareResource != NULL) {
		delete asr_Resource->asrShareResource;
		asr_Resource->asrShareResource = NULL;
	}
	return 0;
}