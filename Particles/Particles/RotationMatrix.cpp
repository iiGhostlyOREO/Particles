#include "RotationMatrix.h"

RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2) {
    (*this)(0, 0) = cos(theta);
    (*this)(0, 1) = -sin(theta);
    (*this)(1, 0) = sin(theta);
    (*this)(1, 1) = cos(theta);
}