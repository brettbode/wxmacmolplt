#include "Geometries.h"

// --------------------------------------------------------------------------- 

Matrix2D::Matrix2D() {
	data = NULL;
	nrows = 0;
	ncols = 0;
}

// --------------------------------------------------------------------------- 

Matrix2D::Matrix2D(const Matrix2D& src) {
	Matrix2D(src.nrows, src.ncols, src.data);
}

// --------------------------------------------------------------------------- 

Matrix2D::Matrix2D(const int nrows, const int ncols) {
	data = new float[nrows * ncols];
	this->nrows = nrows;
	this->ncols = ncols;
}

// --------------------------------------------------------------------------- 

Matrix2D::Matrix2D(const int nrows, const int ncols, const float *data) {
	Matrix2D(nrows, ncols);
	memcpy(this->data, data, sizeof(float) * nrows * ncols);
}

// --------------------------------------------------------------------------- 

Matrix2D::~Matrix2D() {
	delete data;
}

// --------------------------------------------------------------------------- 

Matrix2D Matrix2D::Transpose(void) const {

	int i, j;
	Matrix2D new_matrix = Matrix2D(nrows, ncols);

	for (i = 0; i < nrows; i++) {
		for (j = 0; j < ncols; j++) {
			new_matrix.data[j * nrows + i] = data[i * ncols + j];
		}
	}

	return new_matrix;

}

// --------------------------------------------------------------------------- 

const Matrix2D& Matrix2D::operator=(const Matrix2D& m) {
	
	if (this != &m) {
		if (data) {
			delete data;
		}
		Matrix2D(m);
	}

	return *this;

}

// --------------------------------------------------------------------------- 

