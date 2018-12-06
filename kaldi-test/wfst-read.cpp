
#include "stdafx.h"
#include "wfst-read.h"

namespace kaldi {
	std::istream & Wfst::ReadHead(std::istream &strm) {
		strm.read(reinterpret_cast<char*> (&(fsthead.magic_num)), sizeof(fsthead.magic_num));
		//TODO read failed
		ReadType(strm, &(fsthead.fsttype_));
		ReadType(strm, &(fsthead.arctype_));

		strm.read(reinterpret_cast<char*> (&(fsthead.version_)), sizeof(fsthead.version_));
		strm.read(reinterpret_cast<char*> (&(fsthead.flags_)), sizeof(fsthead.flags_));
		strm.read(reinterpret_cast<char*> (&(fsthead.properties_)), sizeof(fsthead.properties_));
		strm.read(reinterpret_cast<char*> (&(fsthead.start_)), sizeof(fsthead.start_));
		strm.read(reinterpret_cast<char*> (&(fsthead.numstates_)), sizeof(fsthead.numstates_));
		strm.read(reinterpret_cast<char*> (&(fsthead.numarcs_)), sizeof(fsthead.numarcs_));
		return strm;


	}
	int Wfst::wfstRead(std::istream &strm) {
		startState = fsthead.start_;
		numStates = fsthead.numstates_;
		int StateSize = numStates * sizeof(State);
		//pStates = (void*) malloc(StateSize);
		pStates = new State[numStates];

		for (int64 j = 0; j < fsthead.numstates_; j++) {
			State *state = ( State*)pStates+j;
			strm.read(reinterpret_cast<char *>(&((*state).final)), sizeof((*state).final));
			int64 narcs;
			strm.read(reinterpret_cast<char*>(&narcs), sizeof(narcs));

			state->num_arc = (int)narcs;

			state->arcs = (void*)malloc(state->num_arc * sizeof(Arc));

			strm.read(reinterpret_cast<char*> (state->arcs), narcs*sizeof(Arc));
			//for (int i = 0; i < narcs; i++) {
			//	Arc *temp_arc = (Arc*)(state->arcs) + i;
			//	ReadArc(strm, temp_arc);
			//	//(*state).arcs.push_back(arc);
			//}
			//pStates.push_back(state);
		}
		return 0;
	}

	int Wfst::printFstHead() {

		std::cout << "magic_num = " << fsthead.magic_num << std::endl;
		std::cout << "fsttype_ = " << fsthead.fsttype_ << std::endl;
		std::cout << "arctype_ = " << fsthead.arctype_ << std::endl;
		std::cout << "version_ = " << fsthead.version_ << std::endl;
		std::cout << "flags_ = " << fsthead.flags_ << std::endl;
		std::cout << "properties_ = " << fsthead.properties_ << std::endl;
		std::cout << "start_ = " << fsthead.start_ << std::endl;
		std::cout << "numstates_ = " << fsthead.numstates_ << std::endl;
		std::cout << "numarcs_ = " << fsthead.numarcs_ << std::endl;

		return 0;
	}

}//namespace