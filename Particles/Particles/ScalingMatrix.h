#pragma once
#include "Matrices.h"

using namespace Matrices;
class ScalingMatrix : public Matrix {
public:
	ScalingMatrix(double scale) : Matrix({ {scale, 0}, {0, scale} }) {};
};