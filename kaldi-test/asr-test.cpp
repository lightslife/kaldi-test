// kaldi-test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "asr-online-api.h"
#include "asr-segment.h"
int main()
{

	//��ʼ��ģ��
	void *pHandle=NULL;
	const char*acModel = "../16k-model/final.mdl";
	const char*wordsFile = "words.txt";
	const char*decoderGraph = "../16k-model/HCLG.fst.vector";
	asr_online_resource_init(&pHandle, acModel, wordsFile, decoderGraph);

	//kaldi::Asr_Init_RESOURCE_STRU *asr_Resource = (kaldi::Asr_Init_RESOURCE_STRU*)pHandle;
	
	asr_online_start_server(pHandle, 2);

	asr_online_consumer_init("speaker1", pHandle);


	//ģ���������룬����ʹ�������̷ֿ߳���������ʶ��
	const char * wavename = "../16k-model/test.wav";
	FILE *fp;
	fp = fopen(wavename, "rb");
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp)-44;
	fseek(fp, 44, SEEK_SET);

	short *data = new short[length / 2];
	fread(data, sizeof(short), length / 2, fp);

	int length_splice = 6000;
	int i = 0;
	while (i < length / 2-length_splice) {
		asr_online_consumer_decode("speaker1",data + i, length_splice, pHandle);
		i += length_splice;
	}
	//ʣ������
	asr_online_consumer_decode("speaker1", data + i, length/2-i, pHandle);
	asr_online_consumer_finish("speaker1", pHandle);


	asr_online_stop_server(pHandle);
	asr_online_release_resource(pHandle);

	//test();
 

	//system("pause");
    return 0;
}

