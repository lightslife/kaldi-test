// kaldi-test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "asr-test-api.h"
 
int main()
{


	//初始化模型
	void *pHandle=NULL;
	const char*acModel = "../16k-model/final.mdl";
	const char*wordsFile = "words.txt";
	const char*decoderGraph = "../16k-model/HCLG.fst.vector";
	asr_online_resource_init(&pHandle, acModel, wordsFile, decoderGraph);




	//模拟语音送入，可以使用两个线程分开送语音和识别
	const char * wavename = "../16k-model/down42_2.wav";
	FILE *fp;
	fp = fopen(wavename, "rb");
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp)-44;
	fseek(fp, 44, SEEK_SET);

	short *data = new short[length / 2];
	fread(data, sizeof(short), length / 2, fp);






	//test();
 

	//system("pause");
    return 0;
}

