#pragma once

#include <cmath>
#include <limits>

#define M_PI 3.14159265358979323846

/* vec2 struct */
struct vec2
{
	union {
		struct { double x, y; };
		double e[2];
	};

	vec2();
	vec2(double X, double Y);

	double& operator[](int i);
	double operator[] (int i) const;

	vec2 operator-() const;
	vec2& operator+=(const vec2& v);
	vec2& operator*=(const double t);
	vec2& operator/=(const double t);

	double norm() const;
	double norm_squared() const;
};

/* vec3 struct*/
struct vec3
{
	union {
		struct { double x, y, z; };
		double e[3];
	};

	vec3();
	vec3(double X, double Y, double Z);

	double& operator[](int i);
	double operator[] (int i) const;

	vec3 operator-() const;
	vec3& operator+=(const vec3& v);
	vec3& operator*=(const double t);
	vec3& operator/=(const double t);

	double norm() const;
	double norm_squared() const;
};

/* vec4 struct*/
struct vec4
{
	union {
		struct { double x, y, z, w; };
		double e[4];
	};

	vec4();
	vec4(double X, double Y, double Z, double W);

	double& operator[](int i);
	double operator[] (int i) const;

	vec4 operator-() const;
	vec4& operator+=(const vec4& v);
	vec4& operator*=(const double t);
	vec4& operator/=(const double t);
};

/* mat3 class */
class mat3 {
public:
	mat3();

	vec3& operator[](int i);
	vec3 operator[](int i) const;

	static mat3 identity();
	mat3 transpose() const;
	mat3 inverse() const;
	mat3 inverse_transpose() const;

public:
	vec3 rows[3];
};

/* mat4 class */
class mat4 {
public:
	mat4();

	vec4& operator[](int i);
	vec4 operator[](int i) const;

	static mat4 identity();
	mat4 transpose() const;
	mat4 inverse() const;
	mat4 inverse_transpose() const;

public:
	vec4 rows[4];
};

/* vec2 related functions */
vec2 operator+(const vec2& u, const vec2& v);
vec2 operator-(const vec2& u, const vec2& v);
vec2 operator*(const vec2& u, const vec2& v);
vec2 operator*(double t, const vec2& v);
vec2 operator*(const vec2& v, double t);
vec2 operator/(vec2 v, double t);

/* vec3 related functions */
vec3 operator+(const vec3& u, const vec3& v);
vec3 operator-(const vec3& u, const vec3& v);
vec3 operator*(const vec3& u, const vec3& v);
vec3 operator*(double t, const vec3& v);
vec3 operator*(const vec3& v, double t);
vec3 operator/(vec3 v, double t);
double dot(const vec3& u, const vec3& v);
vec3 cross(const vec3& u, const vec3& v);
vec3 normalize(const vec3& v);
vec3 to_vec3(const vec4& u);

/* vec4 related functions */
vec4 operator+(const vec4& u, const vec4& v);
vec4 operator-(const vec4& u, const vec4& v);
vec4 operator*(double t, const vec4& v);
vec4 operator*(const vec4& v, double t);
vec4 to_vec4(const vec3& u, float w);

/* mat3 related functions */
mat3 adjoint(const mat3& m);
double determinant(const mat3& m);
vec3 operator*(const mat3& m, const vec3 v);
mat3 to_mat3(const mat4& m);

/* mat4 related functions */
double minor(const mat4& m, int r, int c);
double cofactor(const mat4& m, int r, int c);
mat4 adjoint(const mat4& m);
vec4 operator*(const mat4& m, const vec4 v);
mat4 operator*(const mat4& m1, const mat4& m2);

/* transform functions */
mat4 translate(double x, double y, double z);
mat4 rotate(double x, double y, double z);
mat4 scale(double x, double y, double z);
mat4 lookat(vec3 eye, vec3 target, vec3 up);
mat4 ortho(double left, double right, double bottom, double top, double zNear, double zFar);
mat4 perspective(double fov, double aspect_ratio, double zNear, double zFar);

/* utility functions */
double radian(double degree);
vec3 reflect(vec3 L, vec3 N);
vec3 barycentric2D(vec2* v, vec2 p);
double clamp(double x, double mi, double ma);