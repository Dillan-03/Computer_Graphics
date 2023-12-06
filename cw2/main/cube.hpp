#ifndef CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
#define CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B

#include <vector>
#include <cstdlib>

#include "simple_mesh.hpp"
#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"

SimpleMeshDataNoTexture make_cube(std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform);


constexpr float const kCubePositions[] = {
	+1.f, +1.f, -1.f,
	-1.f, +1.f, -1.f,
	-1.f, +1.f, +1.f,
	+1.f, +1.f, -1.f,
	-1.f, +1.f, +1.f,
	+1.f, +1.f, +1.f,

	+1.f, -1.f, +1.f,
	+1.f, +1.f, +1.f,
	-1.f, +1.f, +1.f,
	+1.f, -1.f, +1.f,
	-1.f, +1.f, +1.f,
	-1.f, -1.f, +1.f,

	-1.f, -1.f, +1.f,
	-1.f, +1.f, +1.f,
	-1.f, +1.f, -1.f,
	-1.f, -1.f, +1.f,
	-1.f, +1.f, -1.f,
	-1.f, -1.f, -1.f,

	-1.f, -1.f, -1.f,
	+1.f, -1.f, -1.f,
	+1.f, -1.f, +1.f,
	-1.f, -1.f, -1.f,
	+1.f, -1.f, +1.f,
	-1.f, -1.f, +1.f,

	+1.f, -1.f, -1.f,
	+1.f, +1.f, -1.f,
	+1.f, +1.f, +1.f,
	+1.f, -1.f, -1.f,
	+1.f, +1.f, +1.f,
	+1.f, -1.f, +1.f,

	-1.f, -1.f, -1.f,
	-1.f, +1.f, -1.f,
	+1.f, +1.f, -1.f,
	-1.f, -1.f, -1.f,
	+1.f, +1.f, -1.f,
	+1.f, -1.f, -1.f,
};

// This defines the vertex data for a colored unit cube.
constexpr float const kCubeColors[] = {
	+1.f, +0.f, +0.f,
	+1.f, +0.f, +0.f,
	+1.f, +0.f, +0.f,
	+1.f, +0.f, +0.f,
	+1.f, +0.f, +0.f,
	+1.f, +0.f, +0.f,

	+0.f, +1.f, +0.f,
	+0.f, +1.f, +0.f,
	+0.f, +1.f, +0.f,
	+0.f, +1.f, +0.f,
	+0.f, +1.f, +0.f,
	+0.f, +1.f, +0.f,

	+0.f, +0.f, +1.f,
	+0.f, +0.f, +1.f,
	+0.f, +0.f, +1.f,
	+0.f, +0.f, +1.f,
	+0.f, +0.f, +1.f,
	+0.f, +0.f, +1.f,

	+1.f, +0.f, +1.f,
	+1.f, +0.f, +1.f,
	+1.f, +0.f, +1.f,
	+1.f, +0.f, +1.f,
	+1.f, +0.f, +1.f,
	+1.f, +0.f, +1.f,

	+1.f, +1.f, +0.f,
	+1.f, +1.f, +0.f,
	+1.f, +1.f, +0.f,
	+1.f, +1.f, +0.f,
	+1.f, +1.f, +0.f,
	+1.f, +1.f, +0.f,

	+0.f, +1.f, +1.f,
	+0.f, +1.f, +1.f,
	+0.f, +1.f, +1.f,
	+0.f, +1.f, +1.f,
	+0.f, +1.f, +1.f,
	+0.f, +1.f, +1.f
};

#endif // CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
