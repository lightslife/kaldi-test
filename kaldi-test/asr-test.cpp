// kaldi-test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "asr-online-api.h"
#include <locale.h>
#include <iostream>
void asr_online_partial_callback(void *userId, stdchar* result_text) {
	setlocale(LC_ALL, "chs");
	//printf("userId is : %s", userId);
	//wprintf(L"partial text is: %s \r", result_text);
}

void asr_online_final_callback(void *userId, stdchar* result_text , float start_time, float end_time) {
	setlocale(LC_ALL, "chs");
	printf("userId is : %s. start time is %f, end time is %f\n", userId, start_time,end_time);
	wprintf(L"\nfinal text is: %s \n", result_text);

	FILE *f_res = fopen("res_info.txt", "a");
	fprintf(f_res,"%s %.2f %.2f ", (char *)userId, start_time, end_time);
	fwprintf(f_res, L"%s\n", result_text);
	fclose(f_res);
}

int main()
{


	//初始化模型
	void *pHandle=NULL;
	const char*acModel = "../8k-model/final.mdl";
	const char*wordsFile = "words.txt";
	const char*decoderGraph = "../8k-model/HCLG.fst.vector";
	int ret=asr_online_resource_init(&pHandle, acModel, wordsFile, decoderGraph);

	//启动识别服务，参数2为线程池数量，可用作多并发情况，普通情况可设置为1.
	ret=asr_online_start_server(pHandle, 1);

	//进行某客户的识别，userId同一时刻唯一。
	//每个客户的识别均分为，初始化，识别和结束。
	ret=asr_online_consumer_init("taskId-001", pHandle);

	//模拟语音送入，可以使用两个线程分开送语音和识别
	const char * wavename = "../8k-model/test3.wav";
	FILE *fp;
	fp = fopen(wavename, "rb");
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp)-44;  //默认为标准wav头，可根据实际情况送入short语音数据
	fseek(fp, 44, SEEK_SET);
	short *data = new short[length / 2];
	fread(data, sizeof(short), length / 2, fp);

	int length_splice = 3200;//每次送入的数据长度，可变化。
	int i = 0;
	while (i < length / 2-length_splice) {
		asr_online_consumer_decode("taskId-001",data + i, length_splice, pHandle);
		i += length_splice;
	}
	//剩余数据
	asr_online_consumer_decode("taskId-001", data + i, length/2-i, pHandle);
	asr_online_consumer_finish("taskId-001", pHandle);


	//释放所有资源
	asr_online_stop_server(pHandle);
	asr_online_release_resource(pHandle);


	system("pause");
    return 0;
}

