#include "../shader.h"

int CubemapIndex(vec3 v, vec2& uv)
{
	int i = 0;
	double x_abs = fabs(v.x);
	double y_abs = fabs(v.y);
	double z_abs = fabs(v.z);
	int w = 1;
	// top or bottom
	if (y_abs > x_abs && y_abs > z_abs)
	{
		w = y_abs;
		if (v.y > 0.0)
		{
			i = 0;
			uv.x = v.x;
			uv.y = v.z;
		}
		else
		{
			i = 1;
			uv.x = v.x;
			uv.y = -v.z;
		}
		//i = v.y > 0.0 ? 0 : 1;
	}

	// left or right
	if (x_abs > y_abs && x_abs > z_abs)
	{
		w = x_abs;
		if (v.x > 0.0)
		{
			i = 2;
			uv.x = v.z;
			uv.y = v.y;
		}
		else
		{
			i = 3;
			uv.x = -v.z;
			uv.y = v.y;
		}
		//i = v.x > 0.0 ? 2 : 3;
	}

	// front or back
	if (z_abs > x_abs && z_abs > y_abs)
	{
		w = z_abs;
		if (v.z > 0.0)
		{
			i = 4;
			uv.x = -v.x;
			uv.y = v.y;
		}
		else
		{
			i = 5;
			uv.x = v.x;
			uv.y = v.y;
		}
		//i = v.z > 0.0 ? 4 : 5;
	}

	uv.x = (uv.x + 1.0) / 2.0;
	uv.y = (uv.y + 1.0) / 2.0;
	return i;
}

vec4 SkyboxShader::VS(const int iface, const int nthvert)
{
	vec3 vert_model = uniform.model->Vert(iface, nthvert);
	vary.uv[nthvert] = vert_model;
	vec4 vert_clip = uniform.projectionMat * uniform.viewMat * to_vec4(vert_model, 1.0);
	vary.pos_clip[nthvert] = vert_clip;
	//vec4 res = vec4(vert_clip.x, vert_clip.y, vert_clip.w, vert_clip.w);
	//vary.uv[nthvert] = vec3(vert_clip.x / vert_clip.w, vert_clip.y / vert_clip.w, vert_clip.z / vert_clip.w);
	return vert_clip;
}

bool SkyboxShader::FS(vec3 bc, vec4& fragColor)
{
	double z = 1.0 / (bc[0] / vary.pos_clip[0].w + bc[1] / vary.pos_clip[1].w + bc[2] / vary.pos_clip[2].w);
	attrib.uv = (vary.uv[0] * bc[0] / vary.pos_clip[0].w + vary.uv[1] * bc[1] / vary.pos_clip[1].w + vary.uv[2] * bc[2] / vary.pos_clip[2].w) * z;
	vec2 uv;
	int index = CubemapIndex(attrib.uv, uv);
	fragColor = to_vec4(uniform.model->CubeMap(index, uv), 1.0);
	return false;
}