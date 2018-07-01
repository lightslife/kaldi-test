// kaldi-test.cpp : �������̨Ӧ�ó������ڵ㡣
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
#include "outputText.h"
#include "asr-test-api.h"
 



int main()
{


	using namespace kaldi;
	typedef kaldi::int32 int32;
	typedef kaldi::int64 int64;


	AsrShareResource asrShareResource;
	AsrShareOpt asrShareOpt ;
	WaveDataInfo waveDataInfo ;

	//ģ���������룬����ʹ�������̷ֿ߳���������ʶ��
	WaveData wave_data;
	const char * wavename = "../8k-model/test.wav";
	std::filebuf wavefile;
	wavefile.open(wavename, std::ios::in | std::ios::binary);
	std::istream iswave(&wavefile);	 
	wave_data.ReadQueue(iswave, &(waveDataInfo.waveQueue));

 


	//������Դ��������Դ
	asrLoadResource("words.txt", "../8k-model/final.mdl", "../8k-model/HCLG.fst.vector", &asrShareResource);
	asrSetWaveInfo(&waveDataInfo);
	asrSetShareOpt(&asrShareOpt, &asrShareResource);

 
	//��ʼʶ��
	asrOnlineLoop(&asrShareOpt, &asrShareResource, &waveDataInfo);



	system("pause");
    return 0;
}

