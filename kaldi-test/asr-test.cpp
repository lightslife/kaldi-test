// kaldi-test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "asr-test-api.h"
 
int main()
{



	void *pHandle=NULL;
	const char*acModel = "../16k-model/final.mdl";
	const char*wordsFile = "words.txt";
	const char*decoderGraph = "../16k-model/HCLG.fst.vector";
	asr_online_resource_init(&pHandle, acModel, wordsFile, decoderGraph);
	const char* waveName = "../16k-model/test.wav";


	test();
 

	//system("pause");
    return 0;
}

