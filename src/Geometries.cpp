#include "Geometries.h"
#include <iomanip>

// --------------------------------------------------------------------------- 

Matrix2D::Matrix2D() {
	data = NULL;
	Init(0, 0, NULL);
}

// --------------------------------------------------------------------------- 

Matrix2D::Matrix2D(const Matrix2D& src) {
	data = NULL;
	Init(src.nrows, src.ncols, src.data);
}

// --------------------------------------------------------------------------- 

Matrix2D::Matrix2D(const int nrows, const int ncols) {
	data = NULL;
	Init(nrows, ncols, NULL);
}

// --------------------------------------------------------------------------- 

Matrix2D::Matrix2D(const int nrows, const int ncols, const float *data) {
	data = NULL;
	Init(nrows, ncols, data);
}

// --------------------------------------------------------------------------- 

void Matrix2D::Init(const int nrows, const int ncols, const float *data) {
	this->nrows = nrows;
	this->ncols = ncols;

	if (this->data) {
		delete this->data;
	}

	this->data = new float[nrows * ncols];

	if (data) {
		memcpy(this->data, data, sizeof(float) * nrows * ncols);
	}
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
		Init(m.nrows, m.ncols, m.data);
	}

	return *this;

}

// --------------------------------------------------------------------------- 

std::ostream& operator<<(std::ostream& stream, const Matrix2D& m) {
	
	int r, c;

	stream << std::fixed << std::showpoint << std::setprecision(4);
	for (r = 0; r < m.nrows; r++) {
		for (c = 0; c < m.ncols; c++) {
			stream << std::setw(10) << m.data[c + r * m.ncols] << " ";
		}
		stream << std::endl;
	}

	return stream;
}

// --------------------------------------------------------------------------- 

