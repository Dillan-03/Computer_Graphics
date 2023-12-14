#include "cylinder.hpp"
#include "../vmlib/mat33.hpp"

/* This code contains a function that generates a simple cylinder in 3D, which is used to create part of the space vehicle in the
main function */

SimpleMeshDataNoTexture make_cylinder( bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform )
{

    // Vertex and Normals
	std::vector<Vec3f> pos;
	std::vector<Vec3f> normals;

    // Variables for the first angle in the trigonometric circle, this is used to calculate the initial vertices
	float prevY = std::cos( 0.f );
	float prevZ = std::sin( 0.f );

    // 3x3 matrix by inverting and transposing 'aPreTransform'
	Mat33f const N = mat44_to_mat33( transpose(invert(aPreTransform)) );

	// Loop generating cylinder sides (aSubdivs times)
	for( std::size_t i = 0; i < aSubdivs; ++i )
	{
        // Calculate angle for each subdivision 
		float const angle = (i+1) / float(aSubdivs) * 2.f * 3.1415926f;
        // Corresponding y and z coordinates
		float y = std::cos( angle );
		float z = std::sin( angle );	
		
        // Two triangles (= 3*2 positions)
        // Every two triangles form a rectangular segment for each subdivision of the cylinder shell
        // pos adds the vertex positions for the triangles
        // normals adds the corresponding normals 
        pos.emplace_back( Vec3f{ 0.f, prevY, prevZ } );
        normals.emplace_back( Vec3f{ 0.f, prevY, prevZ } );

        pos.emplace_back( Vec3f{ 0.f, y, z } );
        normals.emplace_back( Vec3f{ 0.f, y, z } );

        pos.emplace_back( Vec3f{ 1.f, prevY, prevZ } );
        normals.emplace_back( Vec3f{ 0.f, prevY, prevZ } );

        pos.emplace_back( Vec3f{ 0.f, y, z } );
        normals.emplace_back( Vec3f{ 0.f, y, z } );

        pos.emplace_back( Vec3f{ 1.f, y, z } );
        normals.emplace_back( Vec3f{ 0.f, y, z } );

        pos.emplace_back( Vec3f{ 1.f, prevY, prevZ } );
        normals.emplace_back( Vec3f{0.f, prevY, prevZ } );

	    if (aCapped) {
            // Generate caps
            // Top cap            
            // Center point
            pos.emplace_back(Vec3f{ 0.f, 0.f, 0.f });  
            normals.emplace_back( Vec3f{ -1.f, 0.f, 0.f } );

            pos.emplace_back(Vec3f{ 0.f, y, z });
            normals.emplace_back( Vec3f{ -1.f, 0.f, 0.f } );

            pos.emplace_back(Vec3f{ 0.f, prevY, prevZ });
            normals.emplace_back( Vec3f{ -1.f, 0.f, 0.f } );

            // Bottom cap
            // Center point           
            pos.emplace_back(Vec3f{ 1.f, 0.f, 0.f });  
            normals.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );

            pos.emplace_back(Vec3f{ 1.f, y, z });
            normals.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );


            pos.emplace_back(Vec3f{ 1.f, prevY, prevZ });
            normals.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );
        }
        // Update 
        prevY = y;
        prevZ = z;
    }
    

    // Applies aPretransform to each position in pos
    // Convert from 3x3 vector to 4x4, applies transformation
    // Converts back to 3x3 
    for( auto& p : pos )
	{
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;
		p = Vec3f{ t.x, t.y, t.z };
	}

	// Normalising
	for (auto& n : normals) {
        // Transform the normal using N and normalize it
        n = N * n ;
        n = normalize(n);
    }    

    // Creating the colour vector
	std::vector col( pos.size(), aColor );
	return SimpleMeshDataNoTexture{ std::move(pos), std::move(col),std::move(normals) };
}
