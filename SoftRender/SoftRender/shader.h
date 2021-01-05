#pragma once
#include "math.h"
#include "model.h"
#include "buffer.h"

struct IShader
{
	virtual vec4 VS(const int iface, const int nthvert) = 0;
	virtual bool FS(vec3 bc, vec4& fragColor) = 0;
};


struct PhongShader : IShader
{
	typedef struct
	{
		mat4 modelMat;
		mat4 viewMat;
		mat4 projectionMat;
		mat4 lightSpaceMat;

		Model* model;
		vec3 lightPos;
		vec3 viewPos;

		bool diffuseMap;
		bool specularMap;
		bool normalMap;
		DepthBuffer* shadowMap;
	} uniform_t;

	typedef struct
	{
		vec3 pos[3];
		vec3 normal[3];
		vec2 uv[3];
		vec4 pos_lightSpace[3];
	} varying_t;

	typedef struct
	{
		vec3 fragPos;
		vec3 normal;
		vec2 uv;
		vec4 fragPos_lightSpace;
	} attribute_t;

	uniform_t uniform;
	varying_t vary;
	attribute_t attrib;

	virtual vec4 VS(const int iface, const int nthvert) override;
	virtual bool FS(vec3 bc, vec4& fragColor) override;

	double Shadow(vec4 fragPosLightSpace, double bias);
};

struct DepthShader : public IShader
{
	typedef struct
	{
		Model* model;
		mat4 modelMat;
		mat4 lightSpaceMat;
	} uniform_t;

	uniform_t uniform;

	virtual vec4 VS(const int iface, const int nthvert) override;
	virtual bool FS(vec3 bc, vec4& fragColor) override;
};

struct SkyboxShader : public IShader
{
	typedef struct
	{
		Model* model;
		mat4 viewMat;
		mat4 projectionMat;
	} uniform_t;

	typedef struct
	{
		vec3 uv[3];
		vec4 pos_clip[3];
	} varying_t;

	typedef struct
	{
		vec3 uv;
		vec4 pos_clip;
	} attribute_t;

	uniform_t uniform;
	varying_t vary;
	attribute_t attrib;

	virtual vec4 VS(const int iface, const int nthvert) override;
	virtual bool FS(vec3 bc, vec4& fragColor) override;
};