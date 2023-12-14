#include "cube.hpp"
#include "../vmlib/mat33.hpp"

/* This code contains a function that generates a simple cube in 3D, which is used to create part of the space vehicle in the
main function */

SimpleMeshDataNoTexture make_cube(std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform) {
    // Vectors to store the position, normals and colours 
    std::vector<Vec3f> pos;
    std::vector<Vec3f> normals;
    std::vector<Vec3f> colors;

    // 3x3 matrix by inverting and transposing 'aPreTransform'
    Mat33f const N = mat44_to_mat33( transpose(invert(aPreTransform)) );

    // Iterate over each vertex in kCubePositions
    for (std::size_t i = 0; i < sizeof(kCubePositions) / sizeof(kCubePositions[0]); i += 3) {

        // Extract each vertex position
        Vec3f vertex = Vec3f{
            kCubePositions[i],
            kCubePositions[i + 1],
            kCubePositions[i + 2]
        };
     
        // Add the corresponding vertex from kCubePositions
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

    // Creating the colour vector
    std::vector col(pos.size(),aColor);
    return SimpleMeshDataNoTexture{ std::move(pos), std::move(col), std::move(normals) };
}
