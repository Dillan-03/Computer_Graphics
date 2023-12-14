#ifndef CONE_HPP_CB812C27_5E45_4ED9_9A7F_D66774954C29
#define CONE_HPP_CB812C27_5E45_4ED9_9A7F_D66774954C29

#include <vector>

#include <cstdlib>

#include "simple_mesh.hpp"

#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"
#include "../vmlib/mat33.hpp"

/* Declaring make_cone function, this takes in four parameters:
1. bool aCapped = whether the cone is closed at the base (true by default)
2. std::size_t aSubdivs = number of subdivisions around the cone (default is 16)
3. Vec3f aColor = color of the cone 
4. aPreTransform = 4x4 transformation matrix */

SimpleMeshDataNoTexture make_cone(
	bool aCapped = true,
	std::size_t aSubdivs = 16,
	Vec3f aColor = { 1.f, 1.f, 1.f },
	Mat44f aPreTransform = kIdentity44f
);

#endif // CONE_HPP_CB812C27_5E45_4ED9_9A7F_D66774954C29
