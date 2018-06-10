
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

	int cblas_Xcopy(int numCols, const float * srcData, int a, float *data, int b) {
		//TODO
		return 0;
	}
	int cblas_Xcopy(int numCols, const double * srcData, int a, double *data, int b) {
		//TODO
		return 0;
	}

	int cblas_Xdot(int acols, const float*aData, int a, float * bData, int bStride) {
		//TODO
		return 0;
	}
	int cblas_Xdot(int acols, const double*aData, int a, double * bData, int bStride) {
		//TODO
		return 0;
	}

	int cblas_Xgbmv(int kNoTrans, int aDim, int rDim, int a, int b, float alpha, const float * aData, int c, const float * rData, int d, float beta, float * data_, int e) {
		//TODO
		return 0;
	}

	int cblas_Xgbmv(int kNoTrans, int aDim, int rDim, int a, int b, double alpha, const double * aData, int c, const double * rData, int d, double beta, double *data_, int e) {
		//TODO
		return 0;
	}
	int cblas_Xger(int aDim, int rbDim, float alpha, const float * aData, int a, const float * rbData, int b, float *data_, int stride) {
		//TODO
		return 0;
	}
	int cblas_Xger(int aDim, int rbDim, double alpha, const double * aData, int a, const double * rbData, int b, double *data_, int stride) {
		//TODO
		return 0;
	}
 
	int cblas_Xdot(int aDim, const float * aData, int a, const float *bData, int b){
		//TODO
		return 0;
}
	int cblas_Xdot(int aDim, const double * aData, int a, const double *bData, int b){
		//TODO
		return 0;
	}

	int cblas_Xgemv(int trans, int mRows, int mCols, float alpha, const float * mData, int mStride, const float * vData, int a, float beta, float *data_, int b) {
		//TODO
		return 0;
	}
	int cblas_Xgemv(int trans, int mRows, int mCols, float alpha, const double * mData, int mStride, const double * vData, int a, float beta, double *data_, int b) {
		//TODO
		return 0;
	}


}//kaldi