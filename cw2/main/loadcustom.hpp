#ifndef LOADOBJ_HPP_2CF735BE_6624_413E_B6DC_B5BBA337F96F
#define LOADOBJ_HPP_2CF735BE_6624_413E_B6DC_B5BBA337F96F

#include "simple_mesh.hpp"

SimpleMeshData load_wavefront_obj( char const* aPath );

GLuint load_texture_world( char const* aPath );

SimpleMeshData concatenate( SimpleMeshData aM, SimpleMeshData const& aN );


#endif // LOADOBJ_HPP_2CF735BE_6624_413E_B6DC_B5BBA337F96F
