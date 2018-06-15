#pragma once

#ifndef _WFST_READ_H_
#define _WFST_READ_H_

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <cstdlib>
#include <cmath>

#include "kaldi-types.h"
namespace kaldi {
	struct Arc {
		typedef int Label;
		typedef float Weight;
		typedef int32 StateId;

		Label ilabel;
		Label olabel;
		Weight weight;
		StateId nextstate;
	};

	struct State {
		void* arcs;
		int num_arc;
		float final;
	};


	class Wfst {
	public:
		struct FstHead {
			int magic_num;
			std::string fsttype_;
			std::string arctype_;
			int32 version_;
			int32 flags_;
			uint64 properties_;
			int64 start_;
			int64 numstates_;
			int64 numarcs_;

		};
		void * pStates;
		int64 startState;
		int64 numStates;
		FstHead fsthead;
		int wfstRead(std::istream &strm);
		std::istream &ReadHead(std::istream &strm);
		int printFstHead();

	private:
		static const int kFstMagicNum = 2125659606;
		inline std::istream &ReadArc(std::istream & strm, Arc *arc) {
			strm.read(reinterpret_cast<char*>(&(arc->ilabel)), sizeof(arc->ilabel));
			strm.read(reinterpret_cast<char*>(&(arc->olabel)), sizeof(arc->olabel));
			strm.read(reinterpret_cast<char*>(&(arc->weight)), sizeof(arc->weight));
			strm.read(reinterpret_cast<char*>(&(arc->nextstate)), sizeof(arc->nextstate));
			return strm;
		}
		inline std::istream &ReadType(std::istream &strm, std::string *s) {
			s->clear();
			int32 ns = 0;
			strm.read(reinterpret_cast<char*> (&ns), sizeof(ns));
			for (int i = 0; i < ns; i++) {
				char c;
				strm.read(&c, 1);
				*s += c;
			}
			return strm;
		}
	};


}//namespace
#endif // !_WFST_READ_H_
