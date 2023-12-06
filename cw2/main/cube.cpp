#include "cube.hpp"

SimpleMeshDataNoTexture make_cube(std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform) {
    std::vector<Vec3f> pos;
    std::vector<Vec3f> normals;
    std::vector<Vec3f> colors;

    // Iterate over each vertex defined in kCubePositions
    for (std::size_t i = 0; i < sizeof(kCubePositions) / sizeof(kCubePositions[0]); i += 3) {
        Vec3f vertex = Vec3f{
            kCubePositions[i],
            kCubePositions[i + 1],
            kCubePositions[i + 2]
        };

        // Apply the pre-transform to each vertex
        Vec4f p4{ vertex.x, vertex.y, vertex.z, 1.f };
        Vec4f transformed = aPreTransform * p4;
        transformed /= transformed.w;
        pos.push_back(Vec3f{ transformed.x, transformed.y, transformed.z });

        // Add corresponding color from kCubeColors
        colors.push_back(Vec3f{
            kCubeColors[i],
            kCubeColors[i + 1],
            kCubeColors[i + 2]
        });

        // Calculate normal for each vertex (for simplicity, using face normals)
        Vec3f normal = cross(pos[pos.size() - 2] - vertex, pos[pos.size() - 1] - vertex);
        normals.push_back(normalize(normal));
    }

    std::vector col(pos.size(),aColor);
    return SimpleMeshDataNoTexture{ std::move(pos), std::move(col), std::move(normals) };
}
