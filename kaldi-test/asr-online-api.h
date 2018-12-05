#ifndef __ASR_TEST_API_H__
#define __ASR_TEST_API_H__


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
	typedef 	wchar_t stdchar;
#else 
	typedef 	char stdchar;
#endif // _WIN32


	//��Դ��ʼ��
	int asr_online_resource_init(
								void **pHandle,
								const char*acModel ,
								const char*wordsFile ,
								const char*decoderGraph 
								);

	//����ʶ�����
	int asr_online_start_server(void *pHandle,int num_thread);

	//��ʼ��ĳ�ͻ���ʶ������
	int asr_online_consumer_init(const char *userId,void *pHandle);

	//���пͻ�����ʶ��ÿ������һ�����ȵ�short����
	int asr_online_consumer_decode(const char *userId, short *srcdata, int length, void *pHandle);

	//�����ÿͻ���ʶ��
	int asr_online_consumer_finish(const char *userId, void *pHandle);

	//ֹͣʶ�����
	int asr_online_stop_server(void *pHandle);
	//�ͷ�������Դ
	int asr_online_release_resource(void *pHandle);

	void asr_online_partial_callback(void *userId, stdchar* result_text);

	void asr_online_final_callback(void *userId, stdchar* result_text,float start_time, float end_time);

#ifdef __cplusplus
}
#endif


#endif //__ASR_TEST_API_H__