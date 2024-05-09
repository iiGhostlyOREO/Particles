#include "ScalingMatrix.h"

ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2) {
    (*this)(0, 0) = scale;
    (*this)(0, 1) = 0;
    (*this)(1, 0) = 0;
    (*this)(1, 1) = scale;
}