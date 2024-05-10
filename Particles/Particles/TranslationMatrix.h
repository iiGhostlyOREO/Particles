#pragma once
#include "Matrices.h"

using namespace Matrices;
class TranslationMatrix : public Matrix {
public:
    TranslationMatrix(double xShift, double yShift, int nCols);
};
