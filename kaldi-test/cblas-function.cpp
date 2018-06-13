
#include "stdafx.h"
#include "cblas-function.h"
#define SSE_OPT
#ifdef  SSE_OPT
#include <xmmintrin.h>
#endif //  SSE_OPT
#include "matrix-common.h"
namespace kaldi {



	int cblas_Xgemm(float alpha, int transA, float *aData, int aRows, int aCols, int aStride, int transB, float *bData, int bStride, float beta, float* data, int nRows, int nCols, int stride) {
		
		if (transA == kNoTrans && transB == kNoTrans) {
			for (int i = 0; i < nRows; i++) {
				for (int j = 0; j < nCols; j++) {
					float temp = 0.0;
					for (int k = 0; k < aCols; k++) {
						temp += aData[i*aStride + k] * bData[k*bStride + j];
					}
					data[i*stride + j] = beta*data[i*stride + j] + alpha*temp;
				}
			}
		}
		else   if (transA == kTrans && transB == kNoTrans) {
			for (int i = 0; i < nRows; i++) {
				for (int j = 0; j < nCols; j++) {
					float temp = 0.0;
					for (int k = 0; k < aCols; k++) {
						temp += aData[k*aStride + i] * bData[k*bStride + j];
					}
					data[i*stride + j] = beta*data[i*stride + j] + alpha*temp;
				}
			}
		}else   if (transA == kNoTrans && transB == kTrans) {
			for (int i = 0; i < nRows; i++) {
				for (int j = 0; j < nCols; j++) {
					float temp = 0.0;
					for (int k = 0; k < aCols; k++) {
						temp += aData[i*aStride + k] * bData[j*bStride + k];
					}
					data[i*stride + j] = beta*data[i*stride + j] + alpha*temp;
				}
			}
		}else //  if (transA == kTrans && transB == kNoTrans)
		{
			for (int i = 0; i < nRows; i++) {
				for (int j = 0; j < nCols; j++) {
					float temp = 0.0;
					for (int k = 0; k < aCols; k++) {
						temp += aData[k*aStride + i] * bData[j*bStride + k];
					}
					data[i*stride + j] = beta*data[i*stride + j] + alpha*temp;
				}
			}
		}

		return 0;
	}
	int cblas_Xgemm(double alpha, int transA, double *aData, int aRows, int aCols, int aStride, int transB, double *bData, int bStride, float beta, double* data, int nRows, int nCols, int stride) {

		if (transA == kNoTrans && transB == kNoTrans) {
			for (int i = 0; i < nRows; i++) {
				for (int j = 0; j < nCols; j++) {
					double temp = 0.0;
					for (int k = 0; k < aCols; k++) {
						temp += aData[i*aStride + k] * bData[k*bStride + j];
					}
					data[i*stride + j] = beta*data[i*stride + j] + alpha*temp;
				}
			}
		}
		else   if (transA == kTrans && transB == kNoTrans) {
			for (int i = 0; i < nRows; i++) {
				for (int j = 0; j < nCols; j++) {
					double temp = 0.0;
					for (int k = 0; k < aCols; k++) {
						temp += aData[k*aStride + i] * bData[k*bStride + j];
					}
					data[i*stride + j] = beta*data[i*stride + j] + alpha*temp;
				}
			}
		}
		else   if (transA == kNoTrans && transB == kTrans) {
			for (int i = 0; i < nRows; i++) {
				for (int j = 0; j < nCols; j++) {
					double temp = 0.0;
					for (int k = 0; k < aCols; k++) {
						temp += aData[i*aStride + k] * bData[j*bStride + k];
					}
					data[i*stride + j] = beta*data[i*stride + j] + alpha*temp;
				}
			}
		}
		else //  if (transA == kTrans && transB == kNoTrans)
		{
			for (int i = 0; i < nRows; i++) {
				for (int j = 0; j < nCols; j++) {
					double temp = 0.0;
					for (int k = 0; k < aCols; k++) {
						temp += aData[k*aStride + i] * bData[j*bStride + k];
					}
					data[i*stride + j] = beta*data[i*stride + j] + alpha*temp;
				}
			}
		}

		return 0;
	}


 
  
	int cblas_Xaxpy(::size_t sz, const float alpha, const float* mData, int  a, float *data_, int  b) {
		for (int i = 0; i < sz; i++) {
			data_[i] += alpha*mData[i];
		}
		return 0;
	}
	int cblas_Xaxpy(::size_t  sz, const double alpha, const double* mData, int a, double *data_, int b) {
		for (int i = 0; i < sz; i++) {
			data_[i] += alpha*mData[i];
		}
		return 0;
	}




	int cblas_Xscal(int nCols, float alpha, float  *data, int a) {
		for (int i = 0; i < nCols; i++) {
			data[i] = alpha*data[i];
		}
		return 0;
	}
	int cblas_Xscal(int nCols, float alpha, double *data, int a) {
		for (int i = 0; i < nCols; i++) {
			data[i] = alpha*data[i];
		}
		return 0;
	}

	int cblas_Xcopy(int numCols, const float * srcData, int a, float *data, int b) {
		for (int i = 0; i < numCols; i++) {
			data[i] = srcData[i];
		}
		return 0;
	}
	int cblas_Xcopy(int numCols, const double * srcData, int a, double *data, int b) {
		for (int i = 0; i < numCols; i++) {
			data[i] = srcData[i];
		}
		return 0;
	}

	float cblas_Xdot(int dim, const float*aData, int a,  const float * bData, int bStride) {
		float sum = 0.0;
		//dim 5,7,10+ -- 40
		if (bStride == 1) {
			for (int i = 0; i < dim; i++) {
				sum += aData[i] * bData[i];
			}
		}
		else {
			for (int i = 0; i < dim; i++) {
				sum += aData[i] * bData[bStride*i + i];
			}
		}
		return sum;
	}

	double cblas_Xdot(int dim, const double*aData, int a, const  double * bData, int bStride) {
		float sum = 0.0;
		//dim 5,7,10+ -- 40
		if (bStride == 1) {
			for (int i = 0; i < dim; i++) {
				sum += aData[i] * bData[i];
			}
		}
		else {
			for (int i = 0; i < dim; i++) {
				sum += aData[i] * bData[bStride*i + i];
			}
		}
		return sum;
	}

	int cblas_Xger(int aDim, int rbDim, float alpha, const float * aData, int a, const float * rbData, int b, float *data_, int stride) {
		for (int i = 0; i < aDim; i++, data_ += stride) {
			float alpha_a = alpha*aData[i];
			for (int j = 0; j < rbDim; j++) {
				data_[j] += alpha_a*rbData[j];
			}
		}
		return 0;
	}
	int cblas_Xger(int aDim, int rbDim, double alpha, const double * aData, int a, const double * rbData, int b, double *data_, int stride) {
		for (int i = 0; i < aDim; i++, data_ += stride) {
			float alpha_a = alpha*aData[i];
			for (int j = 0; j < rbDim; j++) {
				data_[j] += alpha_a*rbData[j];
			}
		}
		return 0;
	}
 
//	int cblas_Xdot(int aDim, const float * aData, int a, const float *bData, int b){
//		//TODO
//		return 0;
//}
//	int cblas_Xdot(int aDim, const double * aData, int a, const double *bData, int b){
//		//TODO
//		return 0;
//	}

	int cblas_Xgemv(int trans, int mRows, int mCols, float alpha, const float * mData, int mStride, const float * vData, int a, float beta, float *data_, int b) {
		if (trans == kNoTrans) {
		
			for (int i = 0; i < mRows; i++) {
				float temp = 0.0;
				for (int j = 0; j < mRows; j++) {
					temp += mData[i*mStride + j] * vData[j];
				}
				data_[i] = beta*data_[i] + alpha*temp;
			}

		}else {
		

			for (int i = 0; i < mCols; i++) {
				float temp = 0.0;
				for (int j = 0; j < mRows; j++) {
					temp += mData[j*mStride + i] * vData[j];
				}
				data_[i] = beta*data_[i] + alpha*temp;
			}
		}
		return 0;
	}
	int cblas_Xgemv(int trans, int mRows, int mCols, float alpha, const double * mData, int mStride, const double * vData, int a, float beta, double *data_, int b) {
		if (trans == kNoTrans) {

			for (int i = 0; i < mRows; i++) {
				float temp = 0.0;
				for (int j = 0; j < mRows; j++) {
					temp += mData[i*mStride + j] * vData[j];
				}
				data_[i] = beta*data_[i] + alpha*temp;
			}

		}
		else {


			for (int i = 0; i < mCols; i++) {
				float temp = 0.0;
				for (int j = 0; j < mRows; j++) {
					temp += mData[j*mStride + i] * vData[j];
				}
				data_[i] = beta*data_[i] + alpha*temp;
			}
		}
		return 0;
	}


}//kaldi