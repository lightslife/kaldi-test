// kaldi-test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "asr-online-api.h"
#include <locale.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

FILE *f_res;

void asr_online_partial_callback(void *userId, stdchar* result_text) {
#ifdef _WIN32
	setlocale(LC_ALL, "chs");
	printf("userId is : %s", userId);
	wprintf(L"partial text is: %s \r", result_text);
#else 
	//setlocale(LC_ALL, "chs");
	//printf("userId is : %s", userId);
	printf("partial text is: %s \r", result_text);
#endif // DEBUG

}

void asr_online_final_callback(void *userId, stdchar* result_text , float start_time, float end_time) {

#ifdef _WIN32

	setlocale(LC_ALL, "chs");
	printf("userId is : %s. start time is %f, end time is %f\n", userId, start_time, end_time);
	wprintf(L"\nfinal text is: %s \n", result_text);

	int strLen=wcslen(result_text);
	if (result_text != L""  && strLen!=0) {
		fprintf(f_res, "%s %.2f %.2f ", userId, start_time, end_time);
		fwprintf(f_res, L"%s\n", result_text);
	}

#else 
	printf("final text is: %s \n", result_text);
 
	if (result_text != ""  && strlen(result_text) != 0) {
		fprintf(f_res, "%s %.2f %.2f ", userId, start_time, end_time);
		fprintf(f_res, "%s\n", result_text);
	}

#endif // DEBUG



}


void asr_online_wave_final_callback(void *userId, stdchar* result_text) {

#ifdef _WIN32

	setlocale(LC_ALL, "chs");
	
	wprintf(L"\n wave final text is: %s \n", result_text);

	int strLen = wcslen(result_text);
	if (result_text != L""  && strLen != 0) {
		fprintf(f_res, "%s  ", userId);
		fwprintf(f_res, L"%s\n", result_text);
	}

#else 
	printf("wave final text is: %s \n", result_text);

	if (result_text != ""  && strlen(result_text) != 0) {
		fprintf(f_res, "%s ", userId);
		fprintf(f_res, "%s\n", result_text);
	}

#endif // DEBUG



}


int main(int argc, char *argv[])
{
	if (argc != 4) {
		printf("usage: asr-test waveName textdir modelDir");
		return -1;
	}

	std::string waveName = argv[1];
	std::string textName = argv[2];
	std::string modelDir = argv[3];

	int spos= waveName.find_last_of("/");
	int epos = waveName.find_last_of(".");
	std::string name = waveName.substr(spos+1,epos- spos-1);

	printf("prefix name is %s", name.c_str());
	std::string wfile = textName+name + ".text";
 
	f_res = fopen(wfile.c_str(), "wt");

	//初始化模型
	void *pHandle=NULL;
	std::string acModel = modelDir+"/final.mdl";
	//std::string acModel = "../cnn/final.mdl";
	std::string wordsFile = modelDir+"/words.txt";
	std::string decoderGraph = modelDir+"/HCLG.fst.vector";
	int ret=asr_online_resource_init(&pHandle, acModel.c_str(), wordsFile.c_str(), decoderGraph.c_str());

	//启动识别服务，参数2为线程池数量，可用作多并发情况，普通情况可设置为1.
	ret=asr_online_start_server(pHandle, 1);

	//进行某客户的识别，userId同一时刻唯一。
	//每个客户的识别均分为，初始化，识别和结束。
	ret=asr_online_consumer_init(name.c_str(), pHandle);

	//模拟语音送入，可以使用两个线程分开送语音和识别
	//const char * wavename = "../wav/talk2_8k.wav";
	const char * wavename = waveName.c_str();
	FILE *fp;
	fp = fopen(wavename, "rb");
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp)-44;  //默认为标准wav头，可根据实际情况送入short语音数据
	fseek(fp, 44, SEEK_SET);
	short *data = new short[length / 2];
	fread(data, sizeof(short), length / 2, fp);

	int length_splice = 2400;//每次送入的数据长度，可变化。
	int i = 0;
	while (i < length / 2-length_splice) {
		asr_online_consumer_decode(name.c_str(),data + i, length_splice, pHandle);
		i += length_splice;
	}
	//剩余数据
	asr_online_consumer_decode(name.c_str(), data + i, length/2-i, pHandle);
	asr_online_consumer_finish(name.c_str(), pHandle);


	//释放所有资源
	asr_online_stop_server(pHandle);
	asr_online_release_resource(pHandle);

	fclose(f_res);
	//system("pause");
    return 0;
}

