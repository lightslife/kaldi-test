#pragma once
#ifndef __OUTPUT_H__
#define __OUTPUT_H__
#include <vector>
#include <string>
 
#ifdef _WIN32
typedef std::wstring stdstring;
#else 
typedef std::string stdstring;
#endif // _WIN32

namespace kaldi {
	int outputText(std::vector<stdstring> *wordSymbol, std::vector<int> olabel, std::vector<stdstring> *resultText);
	int readSymbol(const char* filename, std::vector<stdstring> *wordSymbol);


}//namespace 
#endif // !__OUTPUT_H__
