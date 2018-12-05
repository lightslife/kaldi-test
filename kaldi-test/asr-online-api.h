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


	//资源初始化
	int asr_online_resource_init(
								void **pHandle,
								const char*acModel ,
								const char*wordsFile ,
								const char*decoderGraph 
								);

	//开启识别服务
	int asr_online_start_server(void *pHandle,int num_thread);

	//初始化某客户的识别请求
	int asr_online_consumer_init(const char *userId,void *pHandle);

	//进行客户语音识别，每次送入一定长度的short数据
	int asr_online_consumer_decode(const char *userId, short *srcdata, int length, void *pHandle);

	//结束该客户的识别
	int asr_online_consumer_finish(const char *userId, void *pHandle);

	//停止识别服务
	int asr_online_stop_server(void *pHandle);
	//释放所有资源
	int asr_online_release_resource(void *pHandle);

	void asr_online_partial_callback(void *userId, stdchar* result_text);

	void asr_online_final_callback(void *userId, stdchar* result_text,float start_time, float end_time);

#ifdef __cplusplus
}
#endif


#endif //__ASR_TEST_API_H__