#include "stdafx.h"
#include "outputText.h"

#include <codecvt>
#include<locale>  
#include <iostream>
#include <fstream>

namespace kaldi {


	int outputText(std::vector<std::wstring> wordSymbol, std::vector<int> olabel,std::vector<std::wstring> *resultText) {

		std::locale china("chs");
		std::wcout.imbue(china);
		for (int i = olabel.size() - 1; i >= 0; i--) {
			int idState = olabel[i];
			resultText->push_back(wordSymbol[idState]);
			std::wcout << wordSymbol[idState] << " ";
		}
		std::cout << std::endl;
		return 0;
	}


	int readSymbol(const char* filename, std::vector<std::wstring> *wordSymbol) {

		std::string temp;
		std::ifstream wordsFile(filename);
		std::locale china("chs");
		std::wcout.imbue(china);
		std::wstring wtemp;
		std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;

		while (std::getline(wordsFile, temp)) {
			wtemp = strCnv.from_bytes(temp);
			wordSymbol->push_back(wtemp);
			//std::wcout << wtemp << std::endl;
		}
		return 0;
	}
}//namespace