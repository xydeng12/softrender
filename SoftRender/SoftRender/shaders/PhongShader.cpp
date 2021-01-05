#include "../shader.h"

vec4 PhongShader::VS(const int iface, const int nthvert)
{
	vec3 vert_model = uniform.model->Vert(iface, nthvert);
	vec3 norm_model = uniform.model->Normal(iface, nthvert);
	vec2 uv = uniform.model->UV(iface, nthvert);

	vec4 vert_clip = uniform.projectionMat * uniform.viewMat * uniform.modelMat * to_vec4(vert_model, 1.0);

	vary.pos[nthvert] = to_vec3(uniform.modelMat * to_vec4(vert_model, 1.0));
	vary.normal[nthvert] = to_vec3(uniform.modelMat.inverse_transpose() * to_vec4(norm_model, 0.0));
	vary.uv[nthvert] = uv;
	vary.pos_lightSpace[nthvert] = uniform.lightSpaceMat * to_vec4(vary.pos[nthvert], 1.0);

	return vert_clip;
}

bool PhongShader::FS(vec3 bc, vec4& fragColor)
{
	attrib.fragPos = vary.pos[0] * bc[0] + vary.pos[1] * bc[1] + vary.pos[2] * bc[2];
	attrib.normal = normalize(vary.normal[0] * bc[0] + vary.normal[1] * bc[1] + vary.normal[2] * bc[2]);
	attrib.uv = vary.uv[0] * bc[0] + vary.uv[1] * bc[1] + vary.uv[2] * bc[2];
	attrib.fragPos_lightSpace = vary.pos_lightSpace[0] * bc[0] + vary.pos_lightSpace[1] * bc[1] + vary.pos_lightSpace[2] * bc[2];

	// calculate TBN matrix
	vec3 e[2];
	e[0] = vary.pos[1] - vary.pos[0];
	e[1] = vary.pos[2] - vary.pos[0];
	vec2 t[3];
	t[0] = vary.uv[1] - vary.uv[0];
	t[1] = vary.uv[2] - vary.uv[0];

	double f = 1.0 / (t[0].x * t[1].y - t[1].x * t[0].y);

	vec3 tangent, bitangent;
	tangent = f * (e[0] * t[1].y - e[1] * t[0].y);
	tangent = normalize(tangent);
	bitangent = f * (e[1] * t[0].x - e[0] * t[1].x);
	bitangent = normalize(bitangent);
	mat3 TBN;
	TBN[0] = tangent;
	TBN[1] = bitangent;
	TBN[2] = attrib.normal;

	// texture diffuse color
	vec3 color;
	if(uniform.diffuseMap)
		color = uniform.model->Diffuse(attrib.uv);

	vec3 ambient = 0.1 * color;

	vec3 lightDir = normalize(uniform.lightPos - attrib.fragPos);
	vec3 normal = attrib.normal;
	if (uniform.normalMap)
	{
		normal = uniform.model->Normal(attrib.uv);
	}
	normal = normalize(TBN.transpose() * normal);
	double diff = std::max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	
	double pow_spec = 32.0;
	if (uniform.specularMap)
	{
		pow_spec = uniform.model->Specular(attrib.uv);
	}
	vec3 viewDir = normalize(uniform.viewPos - attrib.fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	double spec = pow(std::max(dot(normal, halfwayDir), 0.0), pow_spec);
	vec3 specular = vec3(0.1, 0.1, 0.1) * spec;

	float bias = std::max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	double shadow = Shadow(attrib.fragPos_lightSpace, bias);

	fragColor = to_vec4(ambient + shadow * (diffuse + specular), 1.0);
	return false;
}

double PhongShader::Shadow(vec4 fragPosLightSpace, double bias)
{
	vec3 p = vec3(fragPosLightSpace.x / fragPosLightSpace.w, fragPosLightSpace.y / fragPosLightSpace.w, fragPosLightSpace.z / fragPosLightSpace.w);
	p.x = (p.x + 1.0) / 2.0;
	p.y = (p.y + 1.0) / 2.0;
	double depth = (-p.z + 1.0) / 2.0;
	int x = clamp(p.x * uniform.shadowMap->width - 1.0, 0, uniform.shadowMap->width - 1);
	int y = clamp(p.y * uniform.shadowMap->height - 1.0, 0, uniform.shadowMap->height - 1);
	double closestDepth = uniform.shadowMap->get(x, y);
	double shadow = depth - bias < closestDepth ? 1.0 : 0.0;
	return shadow;
}

vec4 DepthShader::VS(const int iface, const int nthvert)
{
	vec3 vert_model = uniform.model->Vert(iface, nthvert);
	vec4 vert_clip = uniform.lightSpaceMat * uniform.modelMat * to_vec4(vert_model, 1.0);
	return vert_clip;
}

bool DepthShader::FS(vec3 bc, vec4& fragColor)
{
	return false;
}