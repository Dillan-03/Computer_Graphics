#ifndef CYLINDER_HPP_E4D1E8EC_6CDA_4800_ABDD_264F643AF5DB
#define CYLINDER_HPP_E4D1E8EC_6CDA_4800_ABDD_264F643AF5DB

#include <vector>

#include <cstdlib>

#include "simple_mesh.hpp"

#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"

/* Declaring make_cylinder function, which takes in 4 parameters
1. bool aCapped = whether the cylinder is closed at the ends (true by default)
2. std::size_t aSubdivs = number of subdivisions around the cylinder (default is 16)
3. Vec3f aColor = color of the cylinder 
4. aPreTransform = 4x4 transformation matrix */

SimpleMeshDataNoTexture make_cylinder(
	bool aCapped = true,
	std::size_t aSubdivs = 16,
	Vec3f aColor = { 1.f, 1.f, 1.f },
	Mat44f aPreTransform = kIdentity44f
);

#endif // CYLINDER_HPP_E4D1E8EC_6CDA_4800_ABDD_264F643AF5DB
