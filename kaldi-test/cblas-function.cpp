
#include "stdafx.h"
#include "cblas-function.h"
namespace kaldi {



	int cblas_Xgemm(float alpha, int transA, float *aData, int aRows, int aCols, int aStride, int transB, float *bData, int bStride, float beta, float* data, int nRows, int nCols, int stride) {
		//TODO
		return 0;
	}
	int cblas_Xgemm(double alpha, int transA, double *aData, int aRows, int aCols, int aStride, int transB, double *bData, int bStride, float beta, double* data, int nRows, int nCols, int stride) {
		//DOTO
		return 0;
	}


 
  
	int cblas_Xaxpy(::size_t sz, const float alpha, const float* mData, int  a, float *data_, int  b) {
		//TODO
		return 0;
	}
	int cblas_Xaxpy(::size_t  sz, const double alpha, const double* mData, int a, double *data_, int b) {
		//TODO
		return 0;
	}



	int cblas_scopy(int i, const float* mData, int a, float *rowData, int b) {
		//TODO
		return 0;
	}
	int cblas_scopy(int i, const double* mData, int a, double *rowData, int b) {
		//TODO
		return 0;
	}

	int cblas_dcopy(int i, const float* mData, int a, float *rowData, int b) {
		//TODO
		return 0;
	}
	int cblas_dcopy(int i, const double* mData, int a, double *rowData, int b) {
		//TODO
		return 0;
	}


	int cblas_Xscal(int nCols, float alpha, float  *data, int a) {
		//TODO 
		return 0;
	}
	int cblas_Xscal(int nCols, float alpha, double *data, int a) {
		//TODO
		return 0;
	}





}//kaldi