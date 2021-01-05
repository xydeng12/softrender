#pragma once
#include "math.h"

class Camera
{
public:
	Camera(vec3 p, vec3 t, vec3 u) : pos(p), target(t), up(u) {}
	void UpdateCamera();

	vec3 pos;
	vec3 target;
	vec3 up;

private:
	vec3 xAxis;
	vec3 yAxis;
	vec3 zAxis;

	void Orbit();
	void MoveParallel();
	void Zoom();
};