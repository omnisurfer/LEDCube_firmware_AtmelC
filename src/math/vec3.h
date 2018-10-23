/*
 * vec.h
 *
 * Created: 10/21/2018 11:01:25 AM
 *  Author: omnis
 */ 


#ifndef VEC_H_
#define VEC_H_

#include <math.h>
#include <stdlib.h>

typedef struct vec3 vec3;

struct vec3
{	
	union {
		uint8_t x;
		uint8_t r;
	};
	union {
		uint8_t y;
		uint8_t g;
	};
	union {
		uint8_t z;
		uint8_t b;
	};
};


inline float vec3_length(vec3 *vIn) {	
	return sqrt(vIn->x*vIn->x + vIn->y*vIn->y + vIn->z*vIn->z);
	//return 0.0;
}

inline float vec3_squared_length(vec3 *vIn) {
		return vIn->x*vIn->x + vIn->y*vIn->y + vIn->z*vIn->z;
	}

inline void vec3_make_unit_vector(vec3 *vIn) {
	float k = 1.0 / sqrt(vIn->x*vIn->x + vIn->y*vIn->y + vIn->z*vIn->z);

	vIn->x *= k; vIn->y *= k; vIn->z *= k;
}

inline vec3* vec3_add(const vec3 *v1, const vec3 *v2) {
	vec3 *resultVector;
	
	resultVector->x = (v1->x + v2->x);
	resultVector->y = (v1->y + v2->y);
	resultVector->z = (v1->z + v2->z);
	
	//return vec3(v1->x + v2->x, v1->y + v2->y, v1->z + v2->z);
	return resultVector;
}

inline vec3* vec3_subtract(const vec3 *v1, const vec3 *v2) {
	vec3 *resultVector;
		
	resultVector->x = (v1->x - v2->x);
	resultVector->y = (v1->y - v2->y);
	resultVector->z = (v1->z - v2->z);
	
	//return vec3(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
	return resultVector;
}

inline vec3* vec3_multiply(const vec3 *v1, const vec3 *v2) {
	vec3 *resultVector;
		
	resultVector->x = (v1->x * v2->x);
	resultVector->y = (v1->y * v2->y);
	resultVector->z = (v1->z * v2->z);
		
	//return vec3(v1->x * v2->x, v1->y * v2->y, v1->z * v2->z);
	return resultVector;
}

inline vec3* vec3_divide(const vec3 *v1, const vec3 *v2) {
	vec3 *resultVector;
	
	resultVector->x = (v1->x / v2->x);
	resultVector->y = (v1->y / v2->y);
	resultVector->z = (v1->z / v2->z);	

	//return vec3(v1->x / v2->x, v1->y / v2->y, v1->z / v2->z);
	return resultVector;
}

inline vec3* vec3_multiplyScalar(const vec3 *v, float t) {	
	vec3 *resultVector;
		
	resultVector->x = (v->x * t);
	resultVector->y = (v->y * t);
	resultVector->z = (v->z * t);
	
	//return vec3(t*v->x, t*v->y, t*v->z);
	return resultVector;
}

inline vec3* vec3_divideScalar(const vec3 *v, float t) {
	vec3 *resultVector;
	
	resultVector->x = (v->x / t);
	resultVector->y = (v->y / t);
	resultVector->z = (v->z / t);
		
	//return vec3(v->x / t, v->y / t, v->z / t);
	return resultVector;
}

inline float vec3_dot(const vec3 *v1, const vec3 *v2) {
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

inline vec3* vec3_cross(const vec3 *v1, const vec3 *v2) {
	vec3 *resultVector;
	
	resultVector->x = v1->y * v2->z - v1->z * v2->y;
	resultVector->y = -(v1->x * v2->z - v1->z * v2->x);
	resultVector->z = v1->x * v2->y - v1->y * v2->x;
	/*
	return vec3((v1->y * v2->z - v1->z * v2->y),
	(-(v1->x * v2->z - v1->z * v2->x)),
	(v1->x * v2->y - v1->y * v2->x)
	);
	*/
	return resultVector;
}
//*/

//inline vec3 unit_vector(vec3 vIn) {
//	return divide(vIn / length(vIn));
//}

/*
inline vec3& vec3::operator+=(const vec3 &v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const vec3 &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3 &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3 &v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/=(const float t) {
	float k = 1.0 / t;
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}
*/

#endif /* VEC_H_ */