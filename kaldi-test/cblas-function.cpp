
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
#ifdef  SSE_OPT
			int k, k_loop = aCols - 3;
			int j, j_loop = nCols - 3;
			const float *pa, *pb;
			const float *pb1, *pb2,*pb3;
			float *pout, *pout1;

			float sum[4], sum1[4], sum2[4], sum3[4];

			float temp, temp1, temp2, temp3;
			__m128 x, y, z;
			__m128 y1, z1;
			__m128 y2, z2;
			__m128 y3, z3;
			pa = aData;
			pout = data;

			for (int i = 0; i < nRows; i++) {
				pb = bData;
				pb1 = pb + bStride;
				pb2 = pb1 + bStride;
				pb3 = pb2 + bStride;
				pout1 = pout;
				for (j = 0; j < j_loop; j += 4) {
					z = _mm_setzero_ps();
					z1 = _mm_setzero_ps();
					z2 = _mm_setzero_ps();
					z3 = _mm_setzero_ps();
					for (k = 0; k < k_loop; k += 4) {
						x = _mm_load_ps(pa + k);
						y = _mm_load_ps(pb + k);
						y = _mm_mul_ps(x, y);
						z = _mm_add_ps(z, y);

						y1 = _mm_load_ps(pb1 + k);
						y1 = _mm_mul_ps(x, y1);
						z1 = _mm_add_ps(z1, y1);

						y2 = _mm_load_ps(pb2 + k);
						y2 = _mm_mul_ps(x, y2);
						z2 = _mm_add_ps(z2, y2);

						y3 = _mm_load_ps(pb3 + k);
						y3 = _mm_mul_ps(x, y3);
						z3 = _mm_add_ps(z3, y3);
					}
					temp = temp1 = temp2 = temp3 = 0.0f;
					for (; k < aCols; k++) {
						temp += pa[k] * pb[k];
						temp1 += pa[k] * pb1[k];
						temp2 += pa[k] * pb2[k];
						temp3 += pa[k] * pb3[k];
					}
					_mm_storeu_ps(sum, z);
					_mm_storeu_ps(sum1, z1);
					_mm_storeu_ps(sum2, z2);
					_mm_storeu_ps(sum3, z3);
					for (k = 0; k < 4; k++) {
						temp += sum[k];
						temp1 += sum1[k];
						temp2 += sum2[k];
						temp3 += sum3[k];
					}
					(*pout1) = beta*(*pout1) + alpha*temp;
					pout1++;

					(*pout1) = beta*(*pout1) + alpha*temp1;
					pout1++;
					(*pout1) = beta*(*pout1) + alpha*temp2;
					pout1++;
					(*pout1) = beta*(*pout1) + alpha*temp3;
					pout1++;

					pb = pb3 + bStride;
					pb1 = pb + bStride;
					pb2 = pb1 + bStride;
					pb3 = pb2 + bStride;
				}
				for (; j < nCols; j++) {
					z = _mm_setzero_ps();
					for (k = 0; k < k_loop; k += 4) {
						x = _mm_loadu_ps(pa + k);
						y = _mm_loadu_ps(pb + k);
						y = _mm_mul_ps(x, y);
						z = _mm_add_ps(z, y);
					}
					for (; k < aCols; k++) {
						temp += aData[i*aStride + k] * bData[j*bStride + k];
					}
					_mm_storeu_ps(sum, z);
					for (k = 0; k < 4; k++) {
						temp += sum[k];
					}
					*(pout1) = beta* (*pout1) + alpha*temp;
					pout1++;
					pb = pb + bStride;
				}
				pout += stride;
				pa = pa + aStride;
			}









#elif
			for (int i = 0; i < nRows; i++) {
				for (int j = 0; j < nCols; j++) {
					float temp = 0.0;
					for (int k = 0; k < aCols; k++) {
						temp += aData[i*aStride + k] * bData[j*bStride + k];
					}
					data[i*stride + j] = beta*data[i*stride + j] + alpha*temp;
				}
		}

#endif //  SSE_OPT


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