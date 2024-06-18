#pragma once
#include "Surface.h"

class Box : Surface {

	virtual float intersection(Ray ray) const = 0;

};
