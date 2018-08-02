#ifndef __ASR_TEST_API_H__
#define __ASR_TEST_API_H__

#pragma once
#include "stdafx.h"


#ifdef __cplusplus
extern "C" {
#endif

	int test();

	//资源初始化
	int asr_online_resource_init(
								void **pHandle,
								const char*acModel ,
								const char*wordsFile ,
								const char*decoderGraph 
								);

	int asr_online_start_server();
	int asr_online_consumer_init();
	int asr_online_consumer_decode();
	int asr_online_consumer_finish();
	int asr_online_stop_server();
	int asr_online_release_resource();


#ifdef __cplusplus
}
#endif


#endif //__ASR_TEST_API_H__