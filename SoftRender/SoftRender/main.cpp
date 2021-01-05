#include "math.h"
#include "win32.h"
#include "model.h"
#include "pipeline.h"
#include "camera.h"
#include "buffer.h"
#include <iostream>

vec3 eye(0, 1, 10);
vec3 target(0, 0, 0);
vec3 up(0, 1, 0);
vec3 lightPos(1, 1, 1);

int main()
{
	int width = 500, height = 500;
	FrameBuffer frameBuffer(width, height);
	DepthBuffer zBuffer(width, height);
	DepthBuffer shadowMap(width, height);

	Viewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = width;
	viewport.height = height;

	Camera camera(eye, target, up);
	
	Model model("resources/backpack/backpack.obj");
	model.LoadTextureDiffuse("resources/backpack/diffuse.tga");
	model.LoadTextureNormal("resources/backpack/normal.tga");
	model.LoadTextureSpecular("resources/backpack/specular.tga");

	PhongShader shader;
	shader.uniform.model = &model;
	shader.uniform.modelMat = mat4::identity();
	shader.uniform.projectionMat = perspective(45, width / height, -0.1, -100.0);
	shader.uniform.viewPos = eye;
	shader.uniform.lightPos = lightPos;
	shader.uniform.diffuseMap = true;
	shader.uniform.normalMap = true;
	shader.uniform.specularMap = true;
	shader.uniform.shadowMap = &shadowMap;

	DepthShader depthShader;
	depthShader.uniform.model = &model;
	depthShader.uniform.modelMat = mat4::identity();

	// light space matrix
	mat4 lightView = lookat(lightPos, target, up);
	mat4 lightProjection = ortho(-2.0, 2.0, -2.0, 2.0, -0.1, -5.0);
	shader.uniform.lightSpaceMat = lightProjection * lightView;
	depthShader.uniform.lightSpaceMat = lightProjection * lightView;

	InitWindow(width, height);

	double deltaTime = 0.0;
	double lastTime = GetTime();

	double deltaAngle = 5.0;
	double angle = 0.0;

	while (!win->close)
	{
		// per-frame time logic
		double currentTime = GetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// reset buffers
		frameBuffer.clear();
		zBuffer.clear();
		shadowMap.clear();

		// update camera
		camera.UpdateCamera();
		mat4 view = lookat(camera.pos, camera.target, camera.up);
		shader.uniform.viewMat = view;
		shader.uniform.viewPos = camera.pos;

		// animate
		if (angle >= 360.0) angle = 0.0;		
		shader.uniform.modelMat = rotate(0.0, angle, 0.0);
		angle += deltaAngle;

		// shadow map
		int nface = model.nFaces();
		for (int i = 0; i < nface; ++i)
		{
			DrawTriangle(i, nullptr, &shadowMap, &depthShader, viewport);
		}

		// draw objects
		FaceCulling(1);
		for (int i = 0; i < nface; ++i)
		{
			DrawTriangle(i, &frameBuffer, &zBuffer, &shader, viewport);
		}

		// reset mouse info
		win->mouse.orbitDelta = vec2(0, 0);
		win->mouse.wheelDelta = 0.0;

		// draw on window
		DrawWindow(frameBuffer.data);

		// call window message loop
		HandleWindowMessage();
	}
	DestroyWindow();

}
