#include "cone.hpp"
 
/* This code contains a function that generates a simple cone in 3D, which is used to create part of the space vehicle in the
main function */

SimpleMeshDataNoTexture make_cone( bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform )
{
    // Vertex and Normals
	std::vector<Vec3f> pos;
    std::vector<Vec3f> normals;

    // Loop to create each side of the cone
    for (std::size_t i = 0; i < aSubdivs; ++i) {
        // Calculate the current and next angle for the vertices bases on subdivisions
        float const angle = (i / float(aSubdivs)) * 2.f * 3.1415926f;
        float nextAngle = ((i + 1) / float(aSubdivs)) * 2.f * 3.1415926f;

        float y1 = std::cos(angle);
        float z1 = std::sin(angle);
        float y2 = std::cos(nextAngle);
        float z2 = std::sin(nextAngle);

        // Vertex positions for each of the cone sides 
        // Two vertices are on the base and one on the apex
        // Cone's side - triangle from base to apex
        pos.emplace_back(Vec3f{ 0.f, y1, z1 }); // Base
        pos.emplace_back(Vec3f{ 1.f, 0.f, 0.f }); // Apex
        pos.emplace_back(Vec3f{ 0.f, y2, z2 }); // Base

        // Calculates the normal for the side of the cone by averaging the y and z positions of the base vertices
        // Result is normalized
        Vec3f nor = normalize(Vec3f{ 1.f, (y1 + y2) / 2, (z1 + z2) / 2 });
        // Same normal is used for all three vertices of the triangle
        normals.insert(normals.end(), {nor, nor, nor});
    }

    if (aCapped) {
        // Generate base cap (same way)
        for (std::size_t i = 0; i < aSubdivs; ++i) {
            float const angle = (i / float(aSubdivs)) * 2.f * 3.1415926f;
            float nextAngle = ((i + 1) / float(aSubdivs)) * 2.f * 3.1415926f;

            float y1 = std::cos(angle);
            float z1 = std::sin(angle);
            float y2 = std::cos(nextAngle);
            float z2 = std::sin(nextAngle);

            // Base cap vertices 
            pos.emplace_back(Vec3f{ 0.f, y1, z1 });
            pos.emplace_back(Vec3f{ 0.f, y2, z2 });
            // Center point
            pos.emplace_back(Vec3f{ 0.f, 0.f, 0.f }); 
            // Normals are all pointing towards negative y-direction
            normals.insert(normals.end(), { Vec3f{ -1.f, 0.f, 0.f }, Vec3f{ -1.f, 0.f, 0.f }, Vec3f{ -1.f, 0.f, 0.f } });
        }
    }

    // Applies aPretransform to each position in pos
    // Convert from 3x3 vector to 4x4, applies transformation
    // Converts back to 3x3 	for( auto& p : pos )
	for( auto& p : pos )
    {
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;
		p = Vec3f{ t.x, t.y, t.z };
	}

     // Transform and normalize normals
    Mat33f matNor = mat44_to_mat33(transpose(invert(aPreTransform)));
    for (auto& n : normals) {
        // Each normal is normalized
        n = matNor * n;
        n = normalize(n);
    }

    // Creating the colour vector
    std::vector col(pos.size(), aColor);
    return SimpleMeshDataNoTexture{ std::move(pos), std::move(col), std::move(normals) };
}