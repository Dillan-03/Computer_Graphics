#ifndef CUBE_HPP_PX_120623
#define CUBE_HPP_PX_120623

#include <vector>
#include <cstdlib>

#include "simple_mesh.hpp"
#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"

SimpleMeshDataNoTexture make_cube(
    std::size_t aSubdivs = 1,
    Vec3f aColor = { 1.f, 1.f, 1.f },
    Mat44f aPreTransform = kIdentity44f
);

#endif // CUBE_HPP_PX_120623
