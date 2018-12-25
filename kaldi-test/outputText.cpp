#include "stdafx.h"
#include "outputText.h"

#include <codecvt>
#include<locale>  
#include <iostream>
#include <fstream>

namespace kaldi {


	int outputText(std::vector<stdstring> *wordSymbol, std::vector<int> olabel,std::vector<stdstring> *resultText) {

#ifdef _WIN32
		std::locale china("chs");
		std::wcout.imbue(china);
		for (int i = olabel.size() - 1; i >= 0; i--) {
			int idState = olabel[i];
			if (idState > 11) {
				stdstring temp = (*wordSymbol)[idState] + L" ";
				resultText->push_back(temp);
				//std::wcout << (*wordSymbol)[idState] ;
			}
		}
#else
 
		for (int i = olabel.size() - 1; i >= 0; i--) {
			int idState = olabel[i];
			if (idState > 11) {
				resultText->push_back((*wordSymbol)[idState] + " ");
				//std::wcout << (*wordSymbol)[idState] ;
			}
		}
#endif
 
		return 0;
	}


	int readSymbol(const char* filename, std::vector<stdstring> *wordSymbol) {

		std::string temp;
		std::ifstream wordsFile(filename);

#ifdef _WIN32
		std::locale china("chs");
		std::wcout.imbue(china);
		stdstring wtemp;
		std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;

		while (std::getline(wordsFile, temp)) {
			wtemp = strCnv.from_bytes(temp);
			wordSymbol->push_back(wtemp);
			//std::wcout << wtemp << std::endl;
		}
#else 
		while (std::getline(wordsFile, temp)) {
			wordSymbol->push_back(temp);
			//std::wcout << wtemp << std::endl;
		}
#endif
		return 0;
	}
}//namespace