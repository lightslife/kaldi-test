// kaldi-test.cpp : 定义控制台应用程序的入口点。
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
int main()
{


	using namespace kaldi;

	typedef kaldi::int32 int32;
	typedef kaldi::int64 int64;

	std::string nnet3_rxfilename = "final.mdl";
	TransitionModel trans_model;
	nnet3::AmNnetSimple am_nnet;
	{
		bool binary;
		Input ki(nnet3_rxfilename, &binary);
		trans_model.Read(ki.Stream(), binary);
		am_nnet.Read(ki.Stream(), binary);
		SetBatchnormTestMode(true, &(am_nnet.GetNnet()));
		SetDropoutTestMode(true, &(am_nnet.GetNnet()));
		//nnet3::CollapseModel(nnet3::CollapseModelConfig(), &(am_nnet.GetNnet()));
	}


	OnlineNnet2FeaturePipelineConfig feature_opts;
	nnet3::NnetSimpleLoopedComputationOptions decodable_opts;
	LatticeFasterDecoderConfig decoder_opts;
	OnlineEndpointConfig endpoint_opts;

	BaseFloat chunk_length_secs = 0.18;
	bool do_endpointing = false;
	bool online = true;

    return 0;
}

