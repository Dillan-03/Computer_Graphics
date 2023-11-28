#include "cylinder.hpp"

SimpleMeshData make_cylinder( bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform )
{
	//TODO: implement me

	std::vector<Vec3f> pos;

	float prevY = std::cos( 0.f );
	float prevZ = std::sin( 0.f );

	for( std::size_t i = 0; i < aSubdivs; ++i )
	{
		float const angle = (i+1) / float(aSubdivs) * 2.f * 3.1415926f;

		float y = std::cos( angle );
		float z = std::sin( angle );



		// Two triangles (= 3*2 positions) create one segment of the cylinder’s shell.
		pos.emplace_back( Vec3f{ 0.f, prevY, prevZ } );
		pos.emplace_back( Vec3f{ 0.f, y, z } );
		pos.emplace_back( Vec3f{ 1.f, prevY, prevZ } );


		pos.emplace_back( Vec3f{ 0.f, y, z } );
		pos.emplace_back( Vec3f{ 1.f, y, z } );
		pos.emplace_back( Vec3f{ 1.f, prevY, prevZ } );

		prevY = y;
		prevZ = z;
	}

	if (aCapped) {
        // Generate caps
        for (std::size_t i = 0; i < aSubdivs; ++i) {
            float const angle = (i / float(aSubdivs)) * 2.f * 3.1415926f;
            float nextAngle = ((i + 1) / float(aSubdivs)) * 2.f * 3.1415926f;

            float y1 = std::cos(angle);
            float z1 = std::sin(angle);
            float y2 = std::cos(nextAngle);
            float z2 = std::sin(nextAngle);

            // Top cap
            pos.emplace_back(Vec3f{ 0.f, y1, z1 });
            pos.emplace_back(Vec3f{ 0.f, y2, z2 });
            pos.emplace_back(Vec3f{ 0.f, 0.f, 0.f });  // Center point

            // Bottom cap
            pos.emplace_back(Vec3f{ 1.f, y1, z1 });
            pos.emplace_back(Vec3f{ 1.f, y2, z2 });
            pos.emplace_back(Vec3f{ 1.f, 0.f, 0.f });  // Center point
        }
    }

	

	std::vector col( pos.size(), aColor );
	return SimpleMeshData{ std::move(pos), std::move(col) };
}
