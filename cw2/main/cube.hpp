#ifndef CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
#define CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B

#include <vector>
#include <cstdlib>

#include "simple_mesh.hpp"
#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"

/* Declaring make_cube function, which takes in 3 parameters
1. aSubdivs = number of subdivisions in the cube
2. aColor = colour of the cube
3. aPreTransform = 4x4 transformation matrix 

- The kCubePositions array contains the positions of the cube's vertices
- The kCubeColors array contains the colour values for the cube's vertices */

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
