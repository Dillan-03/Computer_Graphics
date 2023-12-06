#include "cube.hpp"

#include "../vmlib/mat33.hpp"

SimpleMeshDataNoTexture make_cube(std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform) {
    // Vectors to store the position, normals and colours 
    std::vector<Vec3f> pos;
    std::vector<Vec3f> normals;
    std::vector<Vec3f> colors;

    Mat33f const N = mat44_to_mat33( transpose(invert(aPreTransform)) );

    // Iterate over each vertex in kCubePositions
    for (std::size_t i = 0; i < sizeof(kCubePositions) / sizeof(kCubePositions[0]); i += 3) {

        //Add the corresponding vertex from kCubePositions
        Vec3f vertex = Vec3f{
            kCubePositions[i],
            kCubePositions[i + 1],
            kCubePositions[i + 2]
        };
     
        pos.push_back({vertex.x,vertex.y,vertex.z});


        // Add matching colour from kCubeColors
        colors.push_back(Vec3f{
            kCubeColors[i],
            kCubeColors[i + 1],
            kCubeColors[i + 2]
        });

        // Calculate normals for each vertex 
        normals.push_back((normalize(N * vertex )));
    }

    for( auto& p : pos )
    {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;
		p = Vec3f{ t.x, t.y, t.z };
    }

    std::vector col(pos.size(),aColor);

    return SimpleMeshDataNoTexture{ std::move(pos), std::move(col), std::move(normals) };
}
