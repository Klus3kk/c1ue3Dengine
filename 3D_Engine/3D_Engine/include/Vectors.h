#pragma once

#include <math.h>

typedef struct {
	float x, y, z, w;
} Vector4;

typedef struct {
	float x, y, z;
} Vector3;

typedef struct {
	float x, y;
} Vector2;

typedef struct {
	float data[4][4];
} Matrix4x4;

typedef struct {
	float position[3]; // x, y, z
	float normal[3];   // nx, ny, nz
	float texCoords[2]; // s, t
} Vertex;


Vector4 vector4(float x, float y, float z, float w);

Vector3 vector(float x, float y, float z);

Vector3 vector_add(Vector3 v1, Vector3 v2);

Vector3 vector_sub(Vector3 v1, Vector3 v2);

Vector3 vector_scale(Vector3 v, float scale);

float vector_dot(Vector3 v1, Vector3 v2);

Vector3 vector_cross(Vector3 v1, Vector3 v2);

Vector3 vector_normalize(Vector3 v);

Vector3 vector_negate(Vector3 v);

Vector3 reflect(Vector3 I, Vector3 N);

float vector_length(Vector3 v);