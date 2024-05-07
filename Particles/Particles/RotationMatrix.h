#pragma once
#include "Matrices.h"

using namespace Matrices;
class RotationMatrix : public Matrix {
public:
	RotationMatrix(double theta) : Matrix({ { cos(theta), -sin(theta) }, { sin(theta), cos(theta) } }) {
	};
};