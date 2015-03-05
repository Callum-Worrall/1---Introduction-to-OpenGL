#ifndef __BOUNDING_SPHERE_H__
#define __BOUNDING_SPHERE_H__

#include <vector>
#include "gl_core_4_4.h"
#include "glm_include.h"

class BoundingSphere
{
public:
	BoundingSphere() : centre(0), radius(0)
	{
	
	}

	~BoundingSphere()
	{

	}

	void fit(const std::vector<vec3> &points)
	{
		vec3 min(1e37f), max(-1e37f);

		for (auto &p : points)
		{
			if (p.x < min.x) min.x = p.x;
			if (p.y < min.y) min.y = p.y;
			if (p.z < min.z) min.z = p.z;
			if (p.x > max.x) max.x = p.x;
			if (p.y > max.y) max.y = p.y;
			if (p.z > max.z) max.z = p.z;
		}
		centre = (min + max) * 0.5f;
		radius = glm::distance(min, centre);
	}

	vec3 centre;
	float radius;
};

#endif