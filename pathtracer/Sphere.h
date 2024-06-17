#pragma once
#include "Surface.h"
class Sphere : public Surface
{


private:
	float intersection(Ray ray) const override;
};

