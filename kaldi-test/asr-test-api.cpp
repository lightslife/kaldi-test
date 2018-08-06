#include "stdafx.h"

#include "asr-test-api.h"
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



int test()
{


	AsrShareResource asrShareResource;
	AsrShareOpt asrShareOpt;
	WaveDataInfo_old waveDataInfo;

	//模拟语音送入，可以使用两个线程分开送语音和识别
	WaveData wave_data;
	const char * wavename = "../16k-model/down42_2.wav";
	std::filebuf wavefile;
	wavefile.open(wavename, std::ios::in | std::ios::binary);
	std::istream iswave(&wavefile);
	wave_data.ReadQueue(iswave, &(waveDataInfo.waveQueue));




	//加载资源，共享资源
	asrLoadResource("words.txt", "../16k-model/final.mdl", "../16k-model/HCLG.fst.vector", &asrShareResource);
	asrSetWaveInfo(&waveDataInfo);
	asrSetShareOpt(&asrShareOpt, &asrShareResource);


	//开始识别
	asrOnlineLoop(&asrShareOpt, &asrShareResource, &waveDataInfo);



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

	//asrSetShareOpt(&asrShareOpt, &asrShareResource);


	//asrSetWaveInfo(&waveDataInfo);
	////开始识别
	//asrOnlineLoop(&asrShareOpt, &asrShareResource, &waveDataInfo);



	//for (int i = 0; i < 20; i++) {

	//	WaveDataInfo_old waveDataInfo;

	//	//模拟语音送入，可以使用两个线程分开送语音和识别
	//	WaveData wave_data;
	//	const char * wavename = "../16k-model/down42_2.wav";

	//	FILE *fp;
	//	fp = fopen(wavename, "rb");
	//	fseek(fp, 0, SEEK_END);
	//	int length = ftell(fp)-44;
	//	fseek(fp, 42, SEEK_SET);

	//	short *data = new short[length / 2];
	//	fread(data, sizeof(short), length / 2, fp);

	//	for (int i = 0; i < length / 2; i++)
	//	{
	//		waveDataInfo.waveQueue.push(data[i]);
	//	}
	//	asrSetWaveInfo(&waveDataInfo);
	//	//开始识别
	//	asrOnlineLoop(&asrShareOpt, &asrShareResource, &waveDataInfo);

	//}


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
	Asr_Init_RESOURCE_STRU *asr_Resource=new Asr_Init_RESOURCE_STRU;

	asrLoadResource(wordsFile,acModel,decoderGraph,&(asr_Resource->asrShareResource) );

	asrSetShareOpt(& (asr_Resource->asrShareOpt), &(asr_Resource->asrShareResource));

	*pHandle = asr_Resource;

	return 0;

}
