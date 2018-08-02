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

	//ģ���������룬����ʹ�������̷ֿ߳���������ʶ��
	WaveData wave_data;
	const char * wavename = "../16k-model/test.wav";
	std::filebuf wavefile;
	wavefile.open(wavename, std::ios::in | std::ios::binary);
	std::istream iswave(&wavefile);
	wave_data.ReadQueue(iswave, &(waveDataInfo.waveQueue));




	//������Դ��������Դ
	asrLoadResource("words.txt", "../16k-model/final.mdl", "../16k-model/HCLG.fst.vector", &asrShareResource);
	asrSetWaveInfo(&waveDataInfo);
	asrSetShareOpt(&asrShareOpt, &asrShareResource);


	//��ʼʶ��
	asrOnlineLoop(&asrShareOpt, &asrShareResource, &waveDataInfo);



	//system("pause");
	return 0;
}



//��Դ��ʼ��
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
