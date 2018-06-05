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


	//template<typename Real>
	//void MatrixBase<Real>::CopyRows(const MatrixBase<Real> &src,
	//	const MatrixIndexT *indices) {
	//	KALDI_ASSERT(NumCols() == src.NumCols());
	//	MatrixIndexT num_rows = num_rows_, num_cols = num_cols_,
	//		this_stride = stride_;
	//	Real *this_data = this->data_;

	//	for (MatrixIndexT r = 0; r < num_rows; r++, this_data += this_stride) {
	//		MatrixIndexT index = indices[r];
	//		if (index < 0) memset(this_data, 0, sizeof(Real) * num_cols_);
	//		else cblas_Xcopy(num_cols, src.RowData(index), 1, this_data, 1);
	//	}
	//}

	int cblas_Xcopy(int numCols, const float * srcData, int a, float *data, int b);
	int cblas_Xcopy(int numCols, const double * srcData, int a, double *data, int b);





	//template <typename Real>
	//Real TraceMatMat(const MatrixBase<Real> &A,
	//	const MatrixBase<Real> &B,
	//	MatrixTransposeType trans) {  // tr(A B), equivalent to sum of each element of A times same element in B'
	//	MatrixIndexT aStride = A.stride_, bStride = B.stride_;
	//	if (trans == kNoTrans) {
	//		KALDI_ASSERT(A.NumRows() == B.NumCols() && A.NumCols() == B.NumRows());
	//		Real ans = 0.0;
	//		Real *adata = A.data_, *bdata = B.data_;
	//		MatrixIndexT arows = A.NumRows(), acols = A.NumCols();
	//		for (MatrixIndexT row = 0; row < arows; row++, adata += aStride, bdata++)
	//			ans += cblas_Xdot(acols, adata, 1, bdata, bStride);
	//		return ans;
	//	}
	//	else {


	int cblas_Xdot(int acols, const float*aData, int a, float * bData, int bStride);
	int cblas_Xdot(int acols, const double*aData, int a, double * bData, int bStride);






	//template<typename Real>
	//void VectorBase<Real>::AddVecVec(Real alpha, const VectorBase<Real> &v,
	//	const VectorBase<Real> &r, Real beta) {
	//	KALDI_ASSERT(v.data_ != this->data_ && r.data_ != this->data_);
	//	// We pretend that v is a band-diagonal matrix.
	//	KALDI_ASSERT(dim_ == v.dim_ && dim_ == r.dim_);
	//	cblas_Xgbmv(kNoTrans, dim_, dim_, 0, 0, alpha, v.data_, 1,
	//		r.data_, 1, beta, this->data_, 1);
	//}

	int cblas_Xgbmv(int kNoTrans, int aDim, int rDim, int a, int b, float alpha, const float * aData, int c, const float * rData, int d, float beta, float * data_, int e);

	int cblas_Xgbmv(int kNoTrans, int aDim, int rDim, int a, int b, double alpha, const double * aData, int c, const double * rData, int d, double beta, double *data_, int e);


	//void MatrixBase<double>::AddVecVec(const double alpha,
	//	const VectorBase<double> &a,
	//	const VectorBase<double> &rb) {
	//	KALDI_ASSERT(a.Dim() == num_rows_ && rb.Dim() == num_cols_);
	//	if (num_rows_ == 0) return;
	//	cblas_Xger(a.Dim(), rb.Dim(), alpha, a.Data(), 1, rb.Data(),
	//		1, data_, stride_);
	//}

	int cblas_Xger(int aDim, int rbDim, float alpha, const float * aData, int a, const float * rbData, int b, float *data_, int stride);
	int cblas_Xger(int aDim, int rbDim, double alpha, const double * aData, int a, const double * rbData, int b, double *data_, int stride);

	//template<typename Real>
	//Real VecVec(const VectorBase<Real> &a,
	//	const VectorBase<Real> &b) {
	//	MatrixIndexT adim = a.Dim();
	//	KALDI_ASSERT(adim == b.Dim());
	//	return cblas_Xdot(adim, a.Data(), 1, b.Data(), 1);
	//}

	int cblas_Xdot(int aDim, const float * aData, int a,  const float *bData, int b);
	int cblas_Xdot(int aDim, const double * aData, int a,  const double *bData, int b);







}


#endif // ! __CBLAS_FUNCTION_H__
