//-----------------------------------------------------------------------------
// lighting_dir_point_spot.vert by Steve Jones 
// Copyright (c) 2015-2016 Game Institute. All Rights Reserved.
//
// Vertex shader for multiple lights
//-----------------------------------------------------------------------------
#version 330 core

layout (location = 0) in vec3 pos;			
layout (location = 1) in vec3 normal;	
layout (location = 2) in vec2 texCoord;

uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix

out vData
{
	vec3 FragPos;
	vec4 FragPos2;
	vec3 Normal;
	vec2 TexCoord;
}vertex;


bool fuzzyCompare(float l, float r);
void main()
{
	vertex.FragPos = vec3(model * vec4(pos, 1.0f));			// vertex position in world space
	vertex.Normal = mat3(transpose(inverse(model))) * normal;	// normal direction in world space

	vertex.TexCoord = texCoord;

	gl_Position =  projection * view *  model * vec4(pos, 1.0f);
	vertex.FragPos2 = gl_Position;
}

bool fuzzyCompare(float l, float r)
{
	float delta = abs(l - r);
	if (delta <= 0.01)
		return true;
	else
		return false;
	return false;
}