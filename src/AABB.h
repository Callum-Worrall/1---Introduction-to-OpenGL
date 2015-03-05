#ifndef __AABB_H__
#define __AABB_H__

#include <vector>
#include "gl_core_4_4.h"
#include "glm_include.h"

class AABB
{
public:

	AABB()
	{
		Reset();
	}

	~AABB()
	{

	}

	void Reset()
	{
		min.x = min.y = min.z = 1e37f;
		max.x = max.y = max.z = -1e37f;
	}

	void Fit(const std::vector<vec3> &points)
	{
		for(auto& p : points)
		{
			if (p.x < min.x) min.x = p.x;
			if (p.y < min.y) min.y = p.y;
			if (p.z < min.z) min.z = p.z;
			if (p.x > max.x) max.x = p.x;
			if (p.y > max.y) max.y = p.y;
			if (p.z > max.z) max.z = p.z;
		}
	}

private:

	vec3 min, max;

};

#endif