#include "cone.hpp"


SimpleMeshDataNoTexture make_cone( bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform )
{
	//TODO: implement me
	std::vector<Vec3f> pos;
    std::vector<Vec3f> normals;

    //loop to create each side of the cone
    for (std::size_t i = 0; i < aSubdivs; ++i) {
        //calculate the current and next angle for the vertices bases on subdivisions
        float const angle = (i / float(aSubdivs)) * 2.f * 3.1415926f;
        float nextAngle = ((i + 1) / float(aSubdivs)) * 2.f * 3.1415926f;

        float y1 = std::cos(angle);
        float z1 = std::sin(angle);
        float y2 = std::cos(nextAngle);
        float z2 = std::sin(nextAngle);

        //cone's side - triangle from base to apex
        pos.emplace_back(Vec3f{ 0.f, y1, z1 }); // Base
        pos.emplace_back(Vec3f{ 1.f, 0.f, 0.f }); // Apex
        pos.emplace_back(Vec3f{ 0.f, y2, z2 }); // Base

        //calculates the normal for the side of the cone by averaging the y and z positions of the base vertices
        //result is normalized
        Vec3f nor = normalize(Vec3f{ 1.f, (y1 + y2) / 2, (z1 + z2) / 2 });
        //same normal is used for all three vertices of the triangle
        normals.insert(normals.end(), {nor, nor, nor});
    }

    if (aCapped) {
        // Generate base cap
        for (std::size_t i = 0; i < aSubdivs; ++i) {
            float const angle = (i / float(aSubdivs)) * 2.f * 3.1415926f;
            float nextAngle = ((i + 1) / float(aSubdivs)) * 2.f * 3.1415926f;

            float y1 = std::cos(angle);
            float z1 = std::sin(angle);
            float y2 = std::cos(nextAngle);
            float z2 = std::sin(nextAngle);

            // Base cap
            pos.emplace_back(Vec3f{ 0.f, y1, z1 });
            pos.emplace_back(Vec3f{ 0.f, y2, z2 });
            pos.emplace_back(Vec3f{ 0.f, 0.f, 0.f }); // Center point

            normals.insert(normals.end(), { Vec3f{ -1.f, 0.f, 0.f }, Vec3f{ -1.f, 0.f, 0.f }, Vec3f{ -1.f, 0.f, 0.f } });
        }
    }

	for( auto& p : pos )
	{
        //convert position vectors into 4D vector
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;
        //back to 3D
		p = Vec3f{ t.x, t.y, t.z };
	}

     //transform and normalize normals
    Mat33f matNor = mat44_to_mat33(transpose(invert(aPreTransform)));
    for (auto& n : normals) {
        //each normal is normalized
        n = matNor * n;
        n = normalize(n);
    }

    std::vector col(pos.size(), aColor);
    return SimpleMeshDataNoTexture{ std::move(pos), std::move(col), std::move(normals) };
}