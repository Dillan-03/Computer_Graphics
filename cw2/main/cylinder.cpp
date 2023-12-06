#include "cylinder.hpp"

#include "../vmlib/mat33.hpp"

SimpleMeshDataNoTexture make_cylinder( bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform )
{

	std::vector<Vec3f> pos;
	std::vector<Vec3f> normals; // Added for storing normals

	float prevY = std::cos( 0.f );
	float prevZ = std::sin( 0.f );

	Mat33f const N = mat44_to_mat33( transpose(invert(aPreTransform)) );

	
	for( std::size_t i = 0; i < aSubdivs; ++i )
	{
		float const angle = (i+1) / float(aSubdivs) * 2.f * 3.1415926f;

		float y = std::cos( angle );
		float z = std::sin( angle );	
		

        // Two triangles (= 3*2 positions) create one segment of the cylinder’s shell.
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
            pos.emplace_back(Vec3f{ 0.f, 0.f, 0.f });  // Center point
            normals.emplace_back( Vec3f{ -1.f, 0.f, 0.f } );

            pos.emplace_back(Vec3f{ 0.f, y, z });
            normals.emplace_back( Vec3f{ -1.f, 0.f, 0.f } );

            pos.emplace_back(Vec3f{ 0.f, prevY, prevZ });
            normals.emplace_back( Vec3f{ -1.f, 0.f, 0.f } );


            // Bottom cap           
            pos.emplace_back(Vec3f{ 1.f, 0.f, 0.f });  // Center point
            normals.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );

            pos.emplace_back(Vec3f{ 1.f, y, z });
            normals.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );


            pos.emplace_back(Vec3f{ 1.f, prevY, prevZ });
            normals.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );
        }

        prevY = y;
        prevZ = z;
	
    }
    


    for( auto& p : pos )
	{
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;

		p = Vec3f{ t.x, t.y, t.z };
	}

	//Normalising
	for (auto& n : normals) {
       
        // Transform the normal using N and normalize it
        n = N * n ;
        n = normalize(n);
    }    

    
	std::vector col( pos.size(), aColor );
	return SimpleMeshDataNoTexture{ std::move(pos), std::move(col),std::move(normals) };
}
