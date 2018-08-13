#pragma once
#ifndef __OUTPUT_H__
#define __OUTPUT_H__
#include <vector>

namespace kaldi {

	int outputText(std::vector<std::wstring> *wordSymbol, std::vector<int> olabel, std::vector<std::wstring> *resultText);
	int readSymbol(const char* filename, std::vector<std::wstring> *wordSymbol);



}//namespace 
#endif // !__OUTPUT_H__
