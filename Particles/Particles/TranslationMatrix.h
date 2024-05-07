#pragma once
#include "Matrices.h"

using namespace Matrices;
class TranslationMatrix : public Matrix {
public:
	TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols) {
		for (unsigned short i = 0; i < nCols; i++) {
			(*this)(0, i) = xShift;
			(*this)(1, i) = yShift;
		}
	}
};