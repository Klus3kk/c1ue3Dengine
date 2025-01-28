#include "Vectors.h"
#include <math.h>
Vector4 vector4(float x, float y, float z, float w) {
	Vector4 v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
}

Vector3 vector(float x, float y, float z) {
	Vector3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

Vector3 vector_add(Vector3 v1, Vector3 v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Vector3 vector_sub(Vector3 a, Vector3 b) {
	return (Vector3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3 vector_normalize(Vector3 v) {
	float mag = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return (Vector3) { v.x / mag, v.y / mag, v.z / mag };
}

Vector3 vector_cross(Vector3 a, Vector3 b) {
	return (Vector3) {
		a.y* b.z - a.z * b.y,
			a.z* b.x - a.x * b.z,
			a.x* b.y - a.y * b.x
	};
}

float vector_dot(Vector3 a, Vector3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


Vector3 vector_scale(Vector3 v, float scale) {
	Vector3 result = { v.x * scale, v.y * scale, v.z * scale };
	return result;
}

Vector3 vector_negate(Vector3 v) {
	return vector(-v.x, -v.y, -v.z);
}

Vector3 reflect(Vector3 I, Vector3 N) {
	float dotIN = 2 * vector_dot(I, N);
	Vector3 result = vector_sub(I, vector_scale(N, dotIN));
	return result;
}

float vector_length(Vector3 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
