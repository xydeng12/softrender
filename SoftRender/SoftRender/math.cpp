#include "math.h"

/* vec2 member functions */
vec2::vec2() : x(0.0), y(0.0) {}
vec2::vec2(double X, double Y) : x(X), y(Y) {}
double& vec2::operator[](int i) { return e[i]; }
double vec2::operator[](int i) const { return e[i]; }
vec2 vec2::operator-() const { return vec2(-x, -y); }
vec2& vec2::operator+=(const vec2& v) { x += v.x; y += v.y; return *this; }
vec2& vec2::operator*=(const double t) { x *= t; y *= t; return *this; }
vec2& vec2::operator/=(const double t) { return *this *= 1.0 / t; }
double vec2::norm() const { return sqrt(norm_squared()); }
double vec2::norm_squared() const { return x * x + y * y; }

/* vec2 related functions */
vec2 operator+(const vec2& u, const vec2& v)
{
	return vec2(u.x + v.x, u.y + v.y);
}

vec2 operator-(const vec2& u, const vec2& v)
{
	return vec2(u.x - v.x, u.y - v.y);
}

vec2 operator*(const vec2& u, const vec2& v)
{
	return vec2(u.x * v.x, u.y * v.y);
}

vec2 operator*(double t, const vec2& v)
{
	return vec2(t * v.x, t * v.y);
}

vec2 operator*(const vec2& v, double t)
{
	return t * v;
}

vec2 operator/(vec2 v, double t)
{
	return (1 / t) * v;
}


/* vec3 member functions */
vec3::vec3() : x(0.0), y(0.0), z(0.0) {}
vec3::vec3(double X, double Y, double Z) : x(X), y(Y), z(Z) {}
double& vec3::operator[](int i) { return e[i]; }
double vec3::operator[](int i) const { return e[i]; }
vec3 vec3::operator-() const { return vec3(-x, -y, -z); }
vec3& vec3::operator+=(const vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
vec3& vec3::operator*=(const double t) { x *= t; y *= t; z *= t; return *this; }
vec3& vec3::operator/=(const double t) { return *this *= 1.0 / t; }
double vec3::norm() const { return sqrt(norm_squared()); }
double vec3::norm_squared() const { return x * x + y * y + z * z; }

/* vec3 related functions */
vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

vec3 operator*(double t, const vec3& v)
{
	return vec3(t * v.x, t * v.y, t * v.z);
}

vec3 operator*(const vec3& v, double t)
{
	return t * v;
}

vec3 operator/(vec3 v, double t)
{
	return (1.0 / t) * v;
}

double dot(const vec3& u, const vec3& v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

vec3 cross(const vec3& u, const vec3& v)
{
	return vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

vec3 normalize(const vec3& v)
{
	return v / v.norm();
}

vec3 to_vec3(const vec4& u)
{
	return vec3(u.x, u.y, u.z);
}


/* vec4 member functions */
vec4::vec4() : x(0.0), y(0.0), z(0.0), w(0.0) {}
vec4::vec4(double X, double Y, double Z, double W) : x(X), y(Y), z(Z), w(W) {}
double& vec4::operator[](int i) { return e[i]; }
double vec4::operator[](int i) const { return e[i]; }
vec4 vec4::operator-() const { return vec4(-x, -y, -z, -w); }
vec4& vec4::operator+=(const vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
vec4& vec4::operator*=(const double t) { x *= t; y *= t; z *= t; w *= t; return *this; }
vec4& vec4::operator/=(const double t) { return *this *= 1.0 / t; }

/* vec4 related functions */
vec4 operator+(const vec4& u, const vec4& v)
{
	return vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

vec4 operator-(const vec4& u, const vec4& v)
{
	return vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}

vec4 operator*(double t, const vec4& v)
{
	return vec4(t * v.x, t * v.y, t * v.z, t * v.w);
}

vec4 operator*(const vec4& v, double t)
{
	return t * v;
}

vec4 to_vec4(const vec3& u, float w)
{
	return vec4(u.x, u.y, u.z, w);
}


/* mat3 member functions */
mat3::mat3() {}
vec3& mat3::operator[](int i) { return rows[i]; }
vec3 mat3::operator[](int i) const { return rows[i]; }

mat3 mat3::identity()
{
	mat3 m;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			m[i][j] = (i == j);
	return m;
}

mat3 mat3::transpose() const
{
	mat3 trans;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			trans[i][j] = rows[j][i];
	return trans;
}

mat3 mat3::inverse() const
{
	mat3 inv;
	mat3 adj = adjoint(*this);
	double det = determinant(*this);
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			inv[i][j] = adj[i][j] / det;
	return inv;
}

mat3 mat3::inverse_transpose() const
{
	return ((*this).inverse()).transpose();
}

/* mat3 related functions */
mat3 adjoint(const mat3& m)
{
	mat3 adj;
	adj[0][0] = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	adj[0][1] = m[0][2] * m[2][1] - m[0][1] * m[2][2];
	adj[0][2] = m[0][1] * m[1][2] - m[1][1] * m[0][2];
	adj[1][0] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	adj[1][1] = m[0][0] * m[2][2] - m[2][0] * m[0][2];
	adj[1][2] = m[1][0] * m[0][2] - m[0][0] * m[1][2];
	adj[2][0] = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	adj[2][1] = m[0][1] * m[2][0] - m[0][0] * m[2][1];
	adj[2][2] = m[0][0] * m[1][1] - m[1][0] * m[0][1];
	return adj;
}

double determinant(const mat3& m)
{
	return m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
		- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
		+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
}

vec3 operator*(const mat3& m, const vec3 v)
{
	vec3 res;
	for (int i = 0; i < 3; ++i)
	{
		res[i] = m[i][0] * v[0] + m[i][1] * v[1] + m[i][2] * v[2];
	}
	return res;
}

mat3 to_mat3(const mat4& m)
{
	mat3 res;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			res[i][j] = m[i][j];
	return res;
}


/* mat4 member functions */
mat4::mat4() {}
vec4& mat4::operator[](int i) { return rows[i]; }
vec4 mat4::operator[](int i) const { return rows[i]; }

mat4 mat4::identity()
{
	mat4 m;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = (i == j);
	return m;
}

mat4 mat4::transpose() const
{
	mat4 trans;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			trans[i][j] = rows[j][i];
	return trans;
}

mat4 mat4::inverse() const
{
	mat4 inv;
	double det = 0.0;
	mat4 adj = adjoint(*this);
	for (int i = 0; i < 4; ++i)
	{
		det += rows[0][i] * adj[0][i];
	}
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			inv[i][j] = adj[i][j] / det;
	return inv;
}

mat4 mat4::inverse_transpose() const
{
	return ((*this).inverse()).transpose();
}

/* mat4 related functions */
double minor(const mat4& m, int r, int c)
{
	mat3 m3;
	for(int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		{
			int row = i < r ? i : i + 1;
			int col = j < c ? j : j + 1;
			m3[i][j] = m[row][col];
		}
	return determinant(m3);
}

double cofactor(const mat4& m, int r, int c)
{
	double sign = ((r + c) & 1) == 0 ? 1.0 : -1.0;
	return sign * minor(m, r, c);
}

mat4 adjoint(const mat4& m)
{
	mat4 adj;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			adj[i][j] = cofactor(m, j, i);
	return adj;
}

vec4 operator*(const mat4& m, const vec4 v)
{
	vec4 res;
	for (int i = 0; i < 4; ++i)
	{
		res[i] = m[i][0] * v[0] + m[i][1] * v[1] + m[i][2] * v[2] + m[i][3] * v[3];
	}
	return res;
}

mat4 operator*(const mat4& m1, const mat4& m2)
{
	mat4 m;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			for (int k = 0; k < 4; ++k)
				m[i][j] += m1[i][k] * m2[k][j];
	return m;
}

mat4 translate(double x, double y, double z)
{
	mat4 m = mat4::identity();
	m[0][3] = x;
	m[1][3] = y;
	m[2][3] = z;
	return m;
}

mat4 rotate(double x, double y, double z)
{
	mat4 r;
	mat4 x_axis, y_axis, z_axis;

	double x_radian = radian(x);
	double c = cos(x_radian);
	double s = sin(x_radian);
	x_axis[0][0] = 1.0;
	x_axis[1][1] = c;
	x_axis[1][2] = -s;
	x_axis[2][1] = s;
	x_axis[2][2] = c;
	x_axis[3][3] = 1.0;

	double y_radian = radian(y);
	c = cos(y_radian);
	s = sin(y_radian);
	y_axis[0][0] = c;
	y_axis[0][2] = s;
	y_axis[1][1] = 1.0;
	y_axis[2][0] = -s;
	y_axis[2][2] = c;
	y_axis[3][3] = 1.0;

	double z_radian = radian(z);
	c = cos(z_radian);
	s = sin(z_radian);
	z_axis[0][0] = c;
	z_axis[0][1] = -s;
	z_axis[1][0] = s;
	z_axis[1][1] = c;
	z_axis[2][2] = 1.0;
	z_axis[3][3] = 1.0;

	r = z_axis * y_axis * x_axis;
	return r;
}

mat4 scale(double x, double y, double z)
{
	mat4 m = mat4::identity();
	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
	return m;
}

mat4 lookat(vec3 eye, vec3 target, vec3 up)
{
	mat4 m = mat4::identity();

	vec3 D = normalize(eye - target);
	vec3 R = normalize(cross(up, D));
	vec3 U = normalize(cross(D, R));

	m[0][0] = R[0];
	m[0][1] = R[1];
	m[0][2] = R[2];

	m[1][0] = U[0];
	m[1][1] = U[1];
	m[1][2] = U[2];

	m[2][0] = D[0];
	m[2][1] = D[1];
	m[2][2] = D[2];

	m[0][3] = -dot(R, eye);
	m[1][3] = -dot(U, eye);
	m[2][3] = -dot(D, eye);

	return m;
}

mat4 ortho(double left, double right, double bottom, double top, double zNear, double zFar)
{
	mat4 m = mat4::identity();
	double x_range = right - left;
	double y_range = top - bottom;
	double z_range = zNear - zFar;
	m[0][0] = 2.0 / x_range;
	m[1][1] = 2.0 / y_range;
	m[2][2] = 2.0 / z_range;
	m[0][3] = -(left + right) / x_range;
	m[1][3] = -(bottom + top) / y_range;
	m[2][3] = -(zNear + zFar) / z_range;
	return m;
}

mat4 perspective(double fov, double aspect_ratio, double zNear, double zFar)
{
	mat4 m = mat4::identity();
	double halftan = tan(radian(fov) / 2.0);

	m[0][0] = 1.0 / (aspect_ratio * halftan);
	m[1][1] = 1.0 / halftan;
	m[2][2] = (zNear + zFar) / (zNear - zFar);
	m[2][3] = 2 * zNear * zFar / (zNear - zFar);
	m[3][2] = -1.0;
	m[3][3] = 0;

	return m;
}

double radian(double degree)
{
	return M_PI / 180.0 * degree;
}

vec3 reflect(vec3 L, vec3 N)
{
	return L - 2.0 * dot(N, L) * N;
}

vec3 barycentric2D(vec2* v, vec2 p)
{
	double alpha = ((v[1].y - v[2].y) * p.x + (v[2].x - v[1].x) * p.y + (v[1].x * v[2].y - v[2].x * v[1].y)) / ((v[1].y - v[2].y) * v[0].x + (v[2].x - v[1].x) * v[0].y + (v[1].x * v[2].y - v[2].x * v[1].y));
	double beta = ((v[0].y - v[2].y) * p.x + (v[2].x - v[0].x) * p.y + (v[0].x * v[2].y - v[2].x * v[0].y)) / ((v[0].y - v[2].y) * v[1].x + (v[2].x - v[0].x) * v[1].y + (v[0].x * v[2].y - v[2].x * v[0].y));
	double gamma = 1 - alpha - beta;

	return vec3(alpha, beta, gamma);
}

double clamp(double x, double mi, double ma)
{
	if (x < mi) x = mi;
	if (x > ma) x = ma;
	return x;
}

