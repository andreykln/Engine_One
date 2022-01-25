#pragma once
#include "Graphics.h"
#include "GeometryGenerator.h"
#include "Camera.h"
#include "MathHelper.h"
#include <fstream>
#include <memory>
#include <typeinfo>
#include <cassert>
#include "UtilityStructures.h"
class Shape
{
public:
	Shape()	{}
	Shape(const Shape&) = delete;
	virtual ~Shape() = default;
};

