#include "cube.hpp"

SimpleMeshDataNoTexture make_cube(std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform) {
    std::vector<Vec3f> pos;
    std::vector<Vec3f> normals;

    // Iterate over each face of the cube
    for (std::size_t face = 0; face < 6; ++face) {
        for (std::size_t i = 0; i < aSubdivs; ++i) {
            for (std::size_t j = 0; j < aSubdivs; ++j) {
                // Compute the vertices and normals for each face
                // Here, we assume the face is subdivided into a grid of aSubdivs * aSubdivs
                // The actual implementation will depend on how you define the vertices and normals for each face
                // This is just a placeholder logic for demonstration
                Vec3f v1, v2, v3, v4; // Vertices for a quad
                Vec3f n; // Normal for the face

                // Compute vertices and normal based on face index and subdivision indices (i, j)
                // ... (compute v1, v2, v3, v4, n here based on face, i, j)

                // Create two triangles for each quad
                pos.emplace_back(v1);
                pos.emplace_back(v2);
                pos.emplace_back(v3);

                pos.emplace_back(v3);
                pos.emplace_back(v4);
                pos.emplace_back(v1);

                // Add normals for each vertex
                for (int k = 0; k < 6; ++k) {
                    normals.emplace_back(n);
                }
            }
        }
    }

    // Apply the pre-transform to each vertex
    for (auto& p : pos) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = aPreTransform * p4;
        t /= t.w;
        p = Vec3f{ t.x, t.y, t.z };
    }

    // Normalize normals
    for (auto& n : normals) {
        n = normalize(n);
    }

    std::vector col(pos.size(), aColor);
    return SimpleMeshDataNoTexture{ std::move(pos), std::move(col), std::move(normals) };
}
