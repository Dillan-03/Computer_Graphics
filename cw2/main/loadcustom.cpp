#include "loadcustom.hpp"

#include <rapidobj/rapidobj.hpp>

#include "../support/error.hpp"

#include <stb_image.h>

SimpleMeshData load_wavefront_obj( char const* aPath )
{
	// Ask rapidobj to load the requested file
	auto result = rapidobj::ParseFile( aPath );
	if( result.error )
		throw Error( "Unable to load OBJ file ’%s’: %s", aPath, result.error.code.message
		().c_str() );

	// OBJ files can define faces that are not triangles. However, OpenGL will only render triangles (and lines
	// and points), so we must triangulate any faces that are not already triangles. Fortunately, rapidobj can do
	// this for us.
	rapidobj::Triangulate( result );

	// Convert the OBJ data into a SimpleMeshData structure. For now, we simply turn the object into a triangle
	// soup, ignoring the indexing information that the OBJ file contains.
	SimpleMeshData ret;

	for( auto const& shape : result.shapes )
	{
		for( std::size_t i = 0; i < shape.mesh.indices.size(); ++i )
		{
			auto const& idx = shape.mesh.indices[i];

			ret.positions.emplace_back( Vec3f{
			result.attributes.positions[idx.position_index*3+0],
			result.attributes.positions[idx.position_index*3+1],
			result.attributes.positions[idx.position_index*3+2]
			} );

		// Always triangles, so we can find the face index by dividing the vertex index by three
			auto const& mat = result.materials[shape.mesh.material_ids[i/3]];


			// Just replicate the material ambient color for each vertex...
			ret.colors.emplace_back( Vec3f{
				mat.ambient[0],
				mat.ambient[1],
				mat.ambient[2]
				} );

			// extract normal vector data from obj file and add it to the mesh data structure:
			ret.normals.emplace_back(Vec3f{
           		result.attributes.normals[idx.normal_index * 3 + 0],
            	result.attributes.normals[idx.normal_index * 3 + 1],
            	result.attributes.normals[idx.normal_index * 3 + 2]
                });


			ret.texcoords.emplace_back(Vec2f{
           		result.attributes.texcoords[idx.texcoord_index * 2 + 0],
            	result.attributes.texcoords[idx.texcoord_index * 2 + 1]
                }); 

		}
	}
	
	return ret;
	
}


GLuint load_texture_world( char const* aPath )
{
	assert( aPath );

	// Load image first
	// This may fail (e.g., image does not exist), so there’s no point in
	// allocating OpenGL resources ahead of time.
	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
	throw Error( "Unable to load image ’%s’\n", aPath );

	// Generate texture object and initialize texture with image
	GLuint tex = 0;
	glGenTextures( 1, &tex );
	glBindTexture( GL_TEXTURE_2D, tex );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr );

	stbi_image_free( ptr );
	// Generate mipmap hierarchy
	glGenerateMipmap( GL_TEXTURE_2D );
	// Configure texture
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 6.f );

	return tex;
}


SimpleMeshDataNoTexture load_noTexture( char const* aPath )
{
	// Ask rapidobj to load the requested file
	auto result = rapidobj::ParseFile( aPath );
	if( result.error )
		throw Error( "Unable to load OBJ file ’%s’: %s", aPath, result.error.code.message
		().c_str() );

	// OBJ files can define faces that are not triangles. However, OpenGL will only render triangles (and lines
	// and points), so we must triangulate any faces that are not already triangles. Fortunately, rapidobj can do
	// this for us.
	rapidobj::Triangulate( result );

	// Convert the OBJ data into a SimpleMeshData structure. For now, we simply turn the object into a triangle
	// soup, ignoring the indexing information that the OBJ file contains.
	SimpleMeshDataNoTexture ret;

	for( auto const& shape : result.shapes )
	{
		for( std::size_t i = 0; i < shape.mesh.indices.size(); ++i )
		{
			auto const& idx = shape.mesh.indices[i];

			ret.positions.emplace_back( Vec3f{
			result.attributes.positions[idx.position_index*3+0],
			result.attributes.positions[idx.position_index*3+1],
			result.attributes.positions[idx.position_index*3+2]
			} );

		// Always triangles, so we can find the face index by dividing the vertex index by three
			auto const& mat = result.materials[shape.mesh.material_ids[i/3]];


			// Just replicate the material ambient color for each vertex...
			ret.colors.emplace_back( Vec3f{
				mat.ambient[0],
				mat.ambient[1],
				mat.ambient[2]
				} );

			// extract normal vector data from obj file and add it to the mesh data structure:
			ret.normals.emplace_back(Vec3f{
           		result.attributes.normals[idx.normal_index * 3 + 0],
            	result.attributes.normals[idx.normal_index * 3 + 1],
            	result.attributes.normals[idx.normal_index * 3 + 2]
                });
		}
	}
	
	return ret;
	
}