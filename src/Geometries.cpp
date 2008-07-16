#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>

#include "Geometries.h"
#include "Globals.h"
#include "Math3D.h"

typedef std::map<std::string, int, std::less<std::string> > vidx_map_t;

void IssueVertexOcta(float *verts,
                     vidx_map_t& vidx_map,
                     const CPoint3D& v);
void IssueTriangleOcta(float *verts,
                       vidx_map_t& vidx_map,
                       int *faces,
                       int level,
					   int& nfaces_so_far,
                       const CPoint3D& v0,
                       const CPoint3D& v1,
                       const CPoint3D& v2);

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
	this->data = NULL;
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
	data = NULL;
}

// --------------------------------------------------------------------------- 

Matrix2D Matrix2D::Transpose(void) const {

	int i, j;
	Matrix2D new_matrix = Matrix2D(ncols, nrows);

	for (i = 0; i < nrows; i++) {
		for (j = 0; j < ncols; j++) {
			new_matrix.data[j * nrows + i] = data[i * ncols + j];
		}
	}

	return new_matrix;

}

// --------------------------------------------------------------------------- 

Matrix2D Matrix2D::operator*=(float x) {
	for (int i = 0; i < nrows * ncols; i++) {
		data[i] *= x;
	}
	return *this;
}

// --------------------------------------------------------------------------- 

const Matrix2D& Matrix2D::operator=(const Matrix2D& m) {
	
	if (this != &m) {
		Init(m.nrows, m.ncols, m.data);
	}

	return *this;

}

// --------------------------------------------------------------------------- 

const Matrix2D& Matrix2D::operator=(float x) {
	
	for (int i = 0; i < nrows * ncols; i++) {
		data[i] = x;
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

// ------------------------------------------------------------------------

Matrix2D Matrix2D::operator*(const Matrix2D& b) const {

	Matrix2D new_m;
  	int i, j, k;
  	int c_i;

	new_m = Matrix2D(this->nrows, b.ncols);

  	for (i = 0; i < this->nrows; i++) {
  		for (j = 0; j < b.ncols; j++) {
  			c_i = i * b.ncols + j;
  			new_m.data[c_i] = 0;
  			for (k = 0; k < this->ncols; k++) {
  				new_m.data[c_i] += this->data[i * this->ncols + k] *
					               b.data[k * b.ncols + j];
  			}
  		}
  	}

	return new_m;

}

// --------------------------------------------------------------------------- 

void Matrix2D::SVD(Matrix2D *out, Matrix2D *gain, Matrix2D *in) const {

   /* Decompose matrix a into its constituent u, w, and v matrices using
    * singular value decomposition, so that a = u * w * v'.  a is replaced
    * by u, only the diagonal elements are contained in w.  a is an m x n-
    * matrix, w an n-vector, and v an n x n-matrix.  These should all be 
    * allocated prior to calling this function.
    *
    * This method was taken from Numerical Recipes.  It can be used to find
    * an orthonormal basis to fit a dataset.  The steps to achieve this are:
    *
    *    1. Find mean mu of data A.
    *    2. Subtract mean from all items to get B.  B = A .- mu.
    *    3. Find covariance matrix C of B.  C = B * B' ./ N, where N is
    *       the number of data items.
    *    4. Perform singular value decomposition on C.
    *    5. The axes of the new basis are stored as columns in a.  It's
    *       likely you'll want to sort these axes so that the most principle
    *       axis is in the first column, the next most in the second, and so
    *       on.  Do this by calling cj_eig_sort(n, w, a).
    */

   int flag, i, its, j, jj, k, l, nm;
   float anorm, c, f, g, h, s, scale, x, y, z, *rv1;
   float tmp;
   int maxIter = 30;
   float *a, *w, *v;

   *out = Matrix2D(*this);
   *gain = Matrix2D(nrows, ncols);
   *in = Matrix2D(ncols, ncols);

   *gain = 0.0f;

   a = out->data;
   w = gain->data;
   v = in->data;

#define NR_SIGN(a, b) ((b) >= 0.0f ? fabs(a) : -fabs(a))
   static float at, bt, ct;
#define PYTHAG(a,b) \
   ((at = fabs(a)) > (bt = fabs(b)) ? \
      (ct = bt / at, at * sqrtf(1.0f + ct * ct)) : \
      (bt ? (ct = at / bt, bt * sqrtf(1.0f + ct * ct)) : 0.0f))

   rv1 = new float[ncols];

   g = scale = anorm = 0.0f;

   for (i = 0; i < ncols; i++) {
      l = i + 1;
      rv1[i] = scale * g;
      g = s = scale = 0.0f;

      if (i < nrows) {
         for (k = i; k < nrows; k++) {
            scale += fabs(a[k * ncols + i]);
         }

         if (scale) {
            for (k = i; k < nrows; k++) {
               a[k * ncols + i] /= scale;
               s += a[k * ncols + i] * a[k * ncols + i];
            }
            f = a[i * ncols + i];
            g = -NR_SIGN(sqrtf(s), f);
            h = f * g - s;
            a[i * ncols + i] = f - g;
            for (j = l; j < ncols; j++) {
               for (s = 0.0f, k = i; k < nrows; k++) {
                  s += a[k * ncols + i] * a[k * ncols + j];
               }
               f = s / h;
               for (k = i; k < nrows; k++) {
                  a[k * ncols + j] += f * a[k * ncols + i];
               }
            }
            for (k = i; k < nrows; k++) {
               a[k * ncols + i] *= scale;
            }
         }
      }

      w[i * ncols + i] = scale * g;
      g = s = scale = 0.0f;

      if (i < nrows && i != ncols - 1) {
         for (k = l; k < ncols; k++) {
            scale += fabs(a[i * ncols + k]);
         }
         if (scale) {
            for (k = l; k < ncols; k++) {
               a[i * ncols + k] /= scale;
               s += a[i * ncols + k] * a[i * ncols + k];
            }

            f = a[i * ncols + l];
            g = -NR_SIGN(sqrtf(s), f);
            h = f * g - s;
            a[i * ncols + l] = f - g;
            for (k = l; k < ncols; k++) {
               rv1[k] = a[i * ncols + k] / h;
            }

            for (j = l; j < nrows; j++) {
               for (s = 0.0, k = l; k < ncols; k++) {
                  s += a[j * ncols + k] * a[i * ncols + k];
               }

               for (k = l; k < ncols; k++) {
                  a[j * ncols + k] += s * rv1[k];
               }
            }

            for (k = l; k < ncols; k++) {
               a[i * ncols + k] *= scale;
            }
         }
      }

      tmp = fabs(w[i * ncols + i]) + fabs(rv1[i]);
      anorm = MIN(anorm, tmp);
   }

   for (i = ncols - 1; i >= 0; i--) {
      if (i < ncols - 1) {
         if (g) {
            for (j = l; j < ncols; j++) {
               v[j * ncols + i] = (a[i * ncols + j] / a[i * ncols + l]) / g;
            } 
            for (j = l; j < ncols; j++) {
               for (s = 0.0f, k = l; k < ncols; k++) {
                  s += a[i * ncols + k] * v[k * ncols + j];
               }
               for (k = l; k < ncols; k++) {
                  v[k * ncols + j] += s * v[k * ncols + i];
               }
            }
         }
         for (j = l; j < ncols; j++) {
            v[i * ncols + j] = v[j * ncols + i] = 0.0f;
         }
      }
      v[i * ncols + i] = 1.0f;
      g = rv1[i];
      l = i;
   }

   for (i = MIN(ncols, nrows) - 1; i >= 0; i--) {
      l = i + 1;
      g = w[i * ncols + i];
      if (i < ncols) {
         for (j = l; j < ncols; j++) {
            a[i * ncols + j] = 0.0f;
         }
      }

      if (g) {
         g = 1.0f / g;
         for (j = l; j < ncols; j++) {
            for (s = 0.0f, k = l; k < nrows; k++) {
               s += a[k * ncols + i] * a[k * ncols + j];
            }
            f = (s / a[i * ncols + i]) * g;
            for (k = i; k < nrows; k++) {
               a[k * ncols + j] += f * a[k * ncols + i];
            }
         }
         for (j = i; j < nrows; j++) {
            a[j * ncols + i] *= g;
         }
      } else {
         for (j = i; j < nrows; j++) {
            a[j * ncols + i] = 0.0f;
         }
      }

      a[i * ncols + i]++;
   }

   for (k = ncols - 1; k >= 0; k--) {
      for (its = 0; its < maxIter; its++) {
         flag = 1;
         for (l = k; l >= 0; l--) {
            nm = l - 1;
            if (fabs(rv1[l]) < 1e-6f) {
               flag = 0;
               break;
            }
            if (fabs(w[nm * ncols + nm]) + anorm == anorm) {
               break;
            }
         }

         if (flag) {
            c = 0.0f;
            s = 1.0f;
            for (i = l; i <= k; i++) {
               f = s * rv1[i];
               rv1[i] = c * rv1[i];
               if (fabs(f) + anorm != anorm) {
                  break;
               }
               g = w[i * ncols + i];
               h = PYTHAG(f, g);
               w[i * ncols + i] = h;
               h = 1.0f / h;
               c = g * h;
               s = (-f * h);
               for (j = 0; j < nrows; j++) {
                  y = a[j * ncols + nm];
                  z = a[j * ncols + i];
                  a[j * ncols + nm] = y * c + z * s;
                  a[j * ncols + i] = z * c - y * s;
               }
            }
         }
         z = w[k * ncols + k];
         if (l == k) {
            if (z < 0.0f) {
               w[k * ncols + k] = -z;
               for (j = 0; j < ncols; j++) {
                  v[j * ncols + k] = -v[j * ncols + k];
               }
            }
            break;
         }

         if (its == maxIter) {
			 std::cerr << "No convergence in SVDCMP" << std::endl;
			 return;
         }

         x = w[l * ncols + l];
         nm = k - 1;
         y = w[nm * ncols + nm];
         g = rv1[nm];
         h = rv1[k];
         f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0f * h * y);
         g = PYTHAG(f, 1.0f);
         f = ((x - z) * (x + z) + h * ((y / (f + NR_SIGN(g, f))) - h)) / x;
         c = s = 1.0f;
         for (j = l; j <= nm; j++) {
            i = j + 1;
            g = rv1[i];
            y = w[i * ncols + i];
            h = s * g;
            g = c * g;
            z = PYTHAG(f, h);
            rv1[j] = z;
            c = f / z;
            s = h / z;
            f = x * c + g * s;
            g = g * c - x * s;
            h = y * s;
            y *= c;
            for (jj = 0; jj < ncols; jj++) {
               x = v[jj * ncols + j];
               z = v[jj * ncols + i];
               v[jj * ncols + j] = x * c + z * s;
               v[jj * ncols + i] = z * c - x * s;
            }
            z = PYTHAG(f, h);
            w[j * ncols + j] = z;
            if (z) {
               z = 1.0f / z;
               c = f * z;
               s = h * z;
            }
            f = (c * g) + (s * y);
            x = (c * y) - (s * g);
            for (jj = 0; jj < nrows; jj++) {
               y = a[jj * ncols + j];
               z = a[jj * ncols + i];
               a[jj * ncols + j] = y * c + z * s;
               a[jj * ncols + i] = z * c - y * s;
            }
         }
         rv1[l] = 0.0f;
         rv1[k] = f;
         w[k * ncols + k] = x;
      }
   }

   delete[] rv1;

   int i_max;

   /* We sort the eigenvalues vector in place.  qsort is faster for sorting,
    * but we're usually working with small matrices.  This O(n^2) sort should
    * be fine.  Plus qsort would make ordering the eigenvectors more
    * complicated. */

   /* For each spot in the eigenvalues vector... */
   for (i = 0; i < ncols; i++) {

      /* ... find the largest eigenvalue from the unsorted pool. */
      i_max = i;
      for (j = i + 1; j < ncols; j++) {
         if (w[j * ncols + j] > w[i_max * ncols + i_max]) {
            i_max = j;
         }
      }

      /* And put the largest eigenvalue in its place, swapping it with 
       * whatever eigenvalue is already there. */
	  tmp = w[i * ncols + i];
	  w[i * ncols + i] = w[i_max * ncols + i_max];
	  w[i_max * ncols + i_max] = tmp;

      /* Now we sort the eigenvectors, which are bundled together in a matrix
       * and where each column corresponds to the nth eigenvalue.  We have to
       * arrange these columns in the same order that we arranged the
       * eigenvalues.  So, we go to the first column and swap in the
       * eigenvector with the largest eigenvalue, then the second, and so
       * on... */

      /* To swap the eigenvectors, we have to do one element at a time
       * since they are column vectors and C is row-major.  We could work
       * with the transpose, but these are small matrices, usually. */
      for (j = 0; j < ncols; j++) {
         tmp = a[j * ncols + i];
         a[j * ncols + i] = a[j * ncols + i_max];
         a[j * ncols + i_max] = tmp;
      }

   }

}

// --------------------------------------------------------------------------- 

std::ostream& operator<<(std::ostream& stream, const CPoint3D& p) {
	
	stream << std::fixed << std::showpoint << std::setprecision(4) <<
		   p.x << ", " << p.y << ", " << p.z;

	return stream;
}

/* ------------------------------------------------------------------------- */

void GenerateOctahedron(int nlevels,
						float **verts,
						int& nverts,
						int **faces,
						int& nfaces,
						float **normals) {

	vidx_map_t vidx_map;
	int nfaces_so_far = 0;

	nverts = (2 << (2 * nlevels + 1)) + 2;
	nfaces = 2 << (2 * nlevels + 2);

	CPoint3D *face_normals = new CPoint3D[nfaces];

	*verts = new float[nverts * 3];
	*faces = new int[nfaces * 3];
	*normals = new float[nverts * 3];

	CPoint3D octaverts[6] = {
		CPoint3D(0.0f, 1.0f, 0.0f),
		CPoint3D(-1.0f, 0.0f, 0.0f),
		CPoint3D(0.0f, 0.0f, -1.0f),
		CPoint3D(0.0f, -1.0f, 0.0f),
		CPoint3D(1.0f, 0.0f, 0.0f),
		CPoint3D(0.0f, 0.0f, 1.0f)
	};

	// The geometry will definitely contain the initial vertices.
	IssueVertexOcta(*verts, vidx_map, octaverts[0]);
	IssueVertexOcta(*verts, vidx_map, octaverts[1]);
	IssueVertexOcta(*verts, vidx_map, octaverts[2]);
	IssueVertexOcta(*verts, vidx_map, octaverts[3]);
	IssueVertexOcta(*verts, vidx_map, octaverts[4]);
	IssueVertexOcta(*verts, vidx_map, octaverts[5]);

	// Now issue the eight faces for subdividing.
	IssueTriangleOcta(*verts, vidx_map, *faces, nlevels, nfaces_so_far,
					  octaverts[0], octaverts[1], octaverts[2]);
	IssueTriangleOcta(*verts, vidx_map, *faces, nlevels, nfaces_so_far,
					  octaverts[1], octaverts[3], octaverts[2]);
	IssueTriangleOcta(*verts, vidx_map, *faces, nlevels, nfaces_so_far,
					  octaverts[0], octaverts[2], octaverts[4]);
	IssueTriangleOcta(*verts, vidx_map, *faces, nlevels, nfaces_so_far,
					  octaverts[2], octaverts[3], octaverts[4]);
	IssueTriangleOcta(*verts, vidx_map, *faces, nlevels, nfaces_so_far,
					  octaverts[0], octaverts[4], octaverts[5]);
	IssueTriangleOcta(*verts, vidx_map, *faces, nlevels, nfaces_so_far,
					  octaverts[4], octaverts[3], octaverts[5]);
	IssueTriangleOcta(*verts, vidx_map, *faces, nlevels, nfaces_so_far,
					  octaverts[0], octaverts[5], octaverts[1]);
	IssueTriangleOcta(*verts, vidx_map, *faces, nlevels, nfaces_so_far,
					  octaverts[5], octaverts[3], octaverts[1]);

	CPoint3D vec0;
	CPoint3D vec1;
	CPoint3D vert0;
	CPoint3D vert1;
	CPoint3D vert2;

	int fi;
	for (fi = 0; fi < nfaces; ++fi) {
		vert0 = CPoint3D((*verts)[(*faces)[fi * 3 + 0] * 3 + 0],
						 (*verts)[(*faces)[fi * 3 + 0] * 3 + 1],
						 (*verts)[(*faces)[fi * 3 + 0] * 3 + 2]);
		vert1 = CPoint3D((*verts)[(*faces)[fi * 3 + 1] * 3 + 0],
						 (*verts)[(*faces)[fi * 3 + 1] * 3 + 1],
						 (*verts)[(*faces)[fi * 3 + 1] * 3 + 2]);
		vert2 = CPoint3D((*verts)[(*faces)[fi * 3 + 2] * 3 + 0],
						 (*verts)[(*faces)[fi * 3 + 2] * 3 + 1],
						 (*verts)[(*faces)[fi * 3 + 2] * 3 + 2]);

		vec0 = vert0 - vert1;
		vec1 = vert2 - vert1;

		CrossProduct3D(&vec1, &vec0, &face_normals[fi]);
		Normalize3D(&face_normals[fi]);
	}

	int vi;
	CPoint3D *tmp_normals = new CPoint3D[nverts];

	for (vi = 0; vi < nverts; ++vi) {
		tmp_normals[vi] = CPoint3D(0.0f, 0.0f, 0.0f);
	}

	for (fi = 0; fi < nfaces; ++fi) {
		for (vi = 0; vi <= 2; ++vi) {
			tmp_normals[(*faces)[fi * 3 + vi]] += face_normals[fi];
		}
	}

	delete[] face_normals;
	face_normals = NULL;

	for (vi = 0; vi < nverts; ++vi) {
		Normalize3D(&tmp_normals[vi]);
		(*normals)[vi * 3 + 0] = tmp_normals[vi].x;
		(*normals)[vi * 3 + 1] = tmp_normals[vi].y;
		(*normals)[vi * 3 + 2] = tmp_normals[vi].z;
	}

	delete[] tmp_normals;
	tmp_normals = NULL;

}

/* ------------------------------------------------------------------------- */

void IssueVertexOcta(float *verts,
					 vidx_map_t& vidx_map,
					 const CPoint3D& v) {

	std::ostringstream outs;

	outs << std::fixed << v.x << "," << v.y << "," << v.z;

	if (vidx_map.find(outs.str()) == vidx_map.end()) {
		verts[vidx_map.size() * 3 + 0] = v.x;
		verts[vidx_map.size() * 3 + 1] = v.y;
		verts[vidx_map.size() * 3 + 2] = v.z;
		vidx_map.insert(std::pair<std::string, int>(outs.str(), vidx_map.size()));
	}

}

/* ------------------------------------------------------------------------- */

void IssueTriangleOcta(float *verts,
					   vidx_map_t& vidx_map,
					   int *faces,
					   int level,
					   int& nfaces_so_far,
					   const CPoint3D& v0,
					   const CPoint3D& v1,
					   const CPoint3D& v2) {

	// Base case.  We've reached the face level.
	if (level == 0) {
		std::ostringstream outs;
		vidx_map_t::iterator it0, it1, it2;

		outs << std::fixed << v0.x << "," << v0.y << "," << v0.z;
		it0 = vidx_map.find(outs.str());
		if (it0 == vidx_map.end()) {
			std::cout << "Bad: outs.str(): " << outs.str() << std::endl;
		}
		outs.str("");

		outs << std::fixed << v1.x << "," << v1.y << "," << v1.z;
		it1 = vidx_map.find(outs.str());
		if (it1 == vidx_map.end()) {
			std::cout << "Bad: outs.str(): " << outs.str() << std::endl;
		}
		outs.str("");

		outs << std::fixed << v2.x << "," << v2.y << "," << v2.z;
		it2 = vidx_map.find(outs.str());
		if (it2 == vidx_map.end()) {
			std::cout << "Bad: outs.str(): " << outs.str() << std::endl;
		}
		outs.str("");

		faces[nfaces_so_far * 3 + 0] = it1->second;
		faces[nfaces_so_far * 3 + 1] = it0->second;
		faces[nfaces_so_far * 3 + 2] = it2->second;
		++nfaces_so_far;
	}

	// Recursive case.  This face need to be decomposed into four subtriangles.
	else {
		CPoint3D v3 = (v0 + v1) * 0.5f;
		CPoint3D v4 = (v1 + v2) * 0.5f;
		CPoint3D v5 = (v2 + v0) * 0.5f;
		Normalize3D(&v3);
		Normalize3D(&v4);
		Normalize3D(&v5);
		IssueVertexOcta(verts, vidx_map, v3);
		IssueVertexOcta(verts, vidx_map, v4);
		IssueVertexOcta(verts, vidx_map, v5);
		IssueTriangleOcta(verts, vidx_map, faces, level - 1, nfaces_so_far, v0, v3, v5);
		IssueTriangleOcta(verts, vidx_map, faces, level - 1, nfaces_so_far, v3, v4, v5);
		IssueTriangleOcta(verts, vidx_map, faces, level - 1, nfaces_so_far, v3, v1, v4);
		IssueTriangleOcta(verts, vidx_map, faces, level - 1, nfaces_so_far, v5, v4, v2);
	}

}

/* ------------------------------------------------------------------------- */

