#include "cone.hpp"

SimpleMeshData make_cone( bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform )
{
	//TODO: implement me
	std::vector<Vec3f> pos;

    for (std::size_t i = 0; i < aSubdivs; ++i) {
        float const angle = (i / float(aSubdivs)) * 2.f * 3.1415926f;
        float nextAngle = ((i + 1) / float(aSubdivs)) * 2.f * 3.1415926f;

        float y1 = std::cos(angle);
        float z1 = std::sin(angle);
        float y2 = std::cos(nextAngle);
        float z2 = std::sin(nextAngle);

        // Cone's side - triangle from base to apex
        pos.emplace_back(Vec3f{ 0.f, y1, z1 }); // Base
        pos.emplace_back(Vec3f{ 1.f, 0.f, 0.f }); // Apex
        pos.emplace_back(Vec3f{ 0.f, y2, z2 }); // Base
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
        }
    }

	for( auto& p : pos )
	{
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;

		p = Vec3f{ t.x, t.y, t.z };
	}

    std::vector col(pos.size(), aColor);
    return SimpleMeshData{ std::move(pos), std::move(col) };
}

