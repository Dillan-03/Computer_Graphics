#ifndef LOADOBJ_HPP_2CF735BE_6624_413E_B6DC_B5BBA337F96F
#define LOADOBJ_HPP_2CF735BE_6624_413E_B6DC_B5BBA337F96F

#include "simple_mesh.hpp"

/* Declaring 3 functions, which take in the paths of the files 
1. Returns SimpleMeshData object (contains vertex, normal, colour, and texture data for a 3D mesh)
2. Returns a GLuint (used to reference texture objects)
3. Returns SimpleMeshDataNoTexture object (same as 1 but without texture information) */

SimpleMeshData load_wavefront_obj( char const* aPath );

GLuint load_texture_world( char const* aPath );

SimpleMeshDataNoTexture load_noTexture( char const* aPath );


#endif // LOADOBJ_HPP_2CF735BE_6624_413E_B6DC_B5BBA337F96F
