//-----------------------------------------------------------------------------
// lighting_dir_point_spot_GEO.c by Eyüp Fýndýklý	 
// Copyright (c) Adeko Tech.
//
// Geometry shader with pick
//-----------------------------------------------------------------------------
#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in vData
{
	vec3 FragPos;
	vec4 FragPos2;
	vec3 Normal;
	vec2 TexCoord;
}vertex[];

out fData
{
	vec3 FragPos;
	vec4 FragPos2;
	vec3 Normal;
	vec2 TexCoord;
	vec2 intersected;
}frag;

uniform vec3 rayOrj;
uniform vec3 rayDir;

float PointInOrOn(vec3 P1, vec3 P2, vec3 A, vec3 B)
{
	vec3 CP1 = cross(B - A, P1 - A);
	vec3 CP2 = cross(B - A, P2 - A);
	return step(0.0, dot(CP1, CP2));
}

bool PointInTriangle(vec3 px, vec3 p0, vec3 p1, vec3 p2)
{
	return 0.0f != (
	PointInOrOn(px, p0, p1, p2) *
	PointInOrOn(px, p1, p2, p0) *
	PointInOrOn(px, p2, p0, p1));
}
//And another function that intersects a plane(which is defined by 3 points, by a ray :

struct Ray
{
	vec3 Origin;
	vec3 Direction;
};

vec3 IntersectPlane(Ray ray, vec3 p0, vec3 p1, vec3 p2)
{
	vec3 D = ray.Direction;
	vec3 N = cross(p1 - p0, p2 - p0);
	vec3 X = ray.Origin + D * dot(p0 - ray.Origin, N) / dot(D, N);

	return X;
}
//Find the intersection point and evaluate if it is in the triangle :

bool IntersectTriangle(Ray ray, vec3 p0, vec3 p1, vec3 p2)
{
	vec3 X = IntersectPlane(ray, p0, p1, p2);
	return PointInTriangle(X, p0, p1, p2);
}

void main()
{
	
	bool intersectTest = IntersectTriangle(Ray(rayOrj, rayDir), vec3(vertex[0].FragPos), vec3(vertex[1].FragPos), vec3(vertex[2].FragPos));

	for (int i = 0; i < 3; i++)
	{
		frag.FragPos = vertex[i].FragPos;
		frag.FragPos2 = vertex[i].FragPos2;
		frag.Normal = vertex[i].Normal;
		frag.TexCoord = vertex[i].TexCoord;
		if (intersectTest == true)
		{
			frag.intersected = vec2(1, 0);
		}
		else
		{
			frag.intersected = vec2(0, 0);
		}
			gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}