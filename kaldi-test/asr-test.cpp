// kaldi-test.cpp : �������̨Ӧ�ó������ڵ㡣
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


	//��ʼ��ģ��
	void *pHandle=NULL;
	const char*acModel = "../8k-model/final.mdl";
	const char*wordsFile = "words.txt";
	const char*decoderGraph = "../8k-model/HCLG.fst.vector";
	int ret=asr_online_resource_init(&pHandle, acModel, wordsFile, decoderGraph);

	//����ʶ����񣬲���2Ϊ�̳߳��������������ಢ���������ͨ���������Ϊ1.
	ret=asr_online_start_server(pHandle, 1);

	//����ĳ�ͻ���ʶ��userIdͬһʱ��Ψһ��
	//ÿ���ͻ���ʶ�����Ϊ����ʼ����ʶ��ͽ�����
	ret=asr_online_consumer_init("taskId-001", pHandle);

	//ģ���������룬����ʹ�������̷ֿ߳���������ʶ��
	const char * wavename = "../8k-model/test3.wav";
	FILE *fp;
	fp = fopen(wavename, "rb");
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp)-44;  //Ĭ��Ϊ��׼wavͷ���ɸ���ʵ���������short��������
	fseek(fp, 44, SEEK_SET);
	short *data = new short[length / 2];
	fread(data, sizeof(short), length / 2, fp);

	int length_splice = 3200;//ÿ����������ݳ��ȣ��ɱ仯��
	int i = 0;
	while (i < length / 2-length_splice) {
		asr_online_consumer_decode("taskId-001",data + i, length_splice, pHandle);
		i += length_splice;
	}
	//ʣ������
	asr_online_consumer_decode("taskId-001", data + i, length/2-i, pHandle);
	asr_online_consumer_finish("taskId-001", pHandle);


	//�ͷ�������Դ
	asr_online_stop_server(pHandle);
	asr_online_release_resource(pHandle);


	system("pause");
    return 0;
}

