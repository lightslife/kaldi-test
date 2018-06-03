#pragma once

#ifndef  __CBLAS_FUNCTION_H__
#define __CBLAS_FUNCTION_H__

namespace kaldi {

 
	//cblas_Xgemm(alpha, transA, A.data_, A.num_rows_, A.num_cols_, A.stride_,
	//	transB, B.data_, B.stride_, beta, data_, num_rows_, num_cols_, stride_);

	int cblas_Xgemm(float alpha, int transA, float *aData, int aRows, int aCols, int aStride, int transB, float *bData, int bStride, float beta, float* data, int nRows, int nCols, int stride );

	int cblas_Xgemm(double alpha, int transA, double *aData, int aRows, int aCols, int aStride, int transB, double *bData, int bStride, float beta, double* data, int nRows, int nCols, int stride);






	//template<typename Real>
	//void PackedMatrix<Real>::AddPacked(const Real alpha, const PackedMatrix<Real> &rMa) {
	//	KALDI_ASSERT(num_rows_ == rMa.NumRows());
	//	size_t nr = num_rows_,
	//		sz = (nr * (nr + 1)) / 2;
	//	cblas_Xaxpy(sz, alpha, rMa.Data(), 1, data_, 1);
	//}


	int cblas_Xaxpy(::size_t sz,   const float alpha,  const float* mData, int  a, float *data_, int  b);
	int cblas_Xaxpy(::size_t  sz, const double alpha,  const double* mData, int a, double *data_, int b);



	//void MatrixBase<float>::CopyFromSp(const SpMatrix<float> & M) {
	//	KALDI_ASSERT(num_rows_ == M.NumRows() && num_cols_ == num_rows_);
	//	MatrixIndexT num_rows = num_rows_, stride = stride_;
	//	const float *Mdata = M.Data();
	//	float *row_data = data_, *col_data = data_;
	//	for (MatrixIndexT i = 0; i < num_rows; i++) {
	//		cblas_scopy(i + 1, Mdata, 1, row_data, 1); // copy to the row.
	//		cblas_scopy(i, Mdata, 1, col_data, stride); // copy to the column.
	//		Mdata += i + 1;
	//		row_data += stride;
	//		col_data += 1;
	//	}
	//}


	int cblas_scopy(int i, const float* mData, int a, float *rowData, int b);
	int cblas_scopy(int i, const double* mData, int a, double *rowData, int b);


	//void MatrixBase<double>::CopyFromSp(const SpMatrix<double> & M) {
	//	KALDI_ASSERT(num_rows_ == M.NumRows() && num_cols_ == num_rows_);
	//	MatrixIndexT num_rows = num_rows_, stride = stride_;
	//	const double *Mdata = M.Data();
	//	double *row_data = data_, *col_data = data_;
	//	for (MatrixIndexT i = 0; i < num_rows; i++) {
	//		cblas_dcopy(i + 1, Mdata, 1, row_data, 1); // copy to the row.
	//		cblas_dcopy(i, Mdata, 1, col_data, stride); // copy to the column.
	//		Mdata += i + 1;
	//		row_data += stride;
	//		col_data += 1;
	//	}
	//}

	int cblas_dcopy(int i, const float* mData, int a, float *rowData, int b);
	int cblas_dcopy(int i, const double* mData, int a, double *rowData, int b);





	//template<typename Real> void MatrixBase<Real>::Scale(Real alpha) {
	//	if (alpha == 1.0) return;
	//	if (num_rows_ == 0) return;
	//	if (num_cols_ == stride_) {
	//		cblas_Xscal(static_cast<size_t>(num_rows_) * static_cast<size_t>(num_cols_),
	//			alpha, data_, 1);
	//	}
	//	else {
	//		Real *data = data_;
	//		for (MatrixIndexT i = 0; i < num_rows_; ++i, data += stride_) {
	//			cblas_Xscal(num_cols_, alpha, data, 1);
	//		}
	//	}
	//}

	int cblas_Xscal(int nCols, float alpha, float  *data, int a);
	int cblas_Xscal(int nCols, float alpha, double *data, int a);








}


#endif // ! __CBLAS_FUNCTION_H__
