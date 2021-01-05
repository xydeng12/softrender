#include "camera.h"
#include "win32.h"

void Camera::UpdateCamera()
{
	zAxis = normalize(pos - target);
	xAxis = normalize(cross(up, zAxis));
	yAxis = normalize(cross(zAxis, xAxis));

	vec2 pos_cur = GetMousePos();
	win->mouse.orbitDelta = win->mouse.orbitPos - pos_cur;
	win->mouse.orbitPos = pos_cur;

	if (win->button[0])
	{
		Orbit();
	}
	if (win->button[1])
	{
		MoveParallel();
	}
	if (win->mouse.wheelDelta != 0.0)
	{
		Zoom();
	}
}

void Camera::Orbit()
{
	double x_delta = win->mouse.orbitDelta.x / win->width;
	double y_delta = win->mouse.orbitDelta.y / win->height;
	double speed = 1.5 * M_PI;

	vec3 view = pos - target;
	double radius = view.norm();
	double phi = atan2(view.x, view.z);
	double theta = acos(view.y / radius);

	phi += x_delta * speed;
	theta += y_delta * speed;
	theta = clamp(theta, 0, M_PI);

	pos.x = target.x + radius * sin(phi) * sin(theta);
	pos.y = target.y + radius * cos(theta);
	pos.z = target.z + radius * sin(theta) * cos(phi);
}

void Camera::MoveParallel()
{
	double x_delta = win->mouse.orbitDelta.x / win->width;
	double y_delta = win->mouse.orbitDelta.y / win->height;
	double speed = M_PI;

	pos += (x_delta * speed * xAxis - y_delta * speed * yAxis);
	target += (x_delta * speed * xAxis - y_delta * speed * yAxis);
}

void Camera::Zoom()
{
	double speed = 0.3;
	pos += -win->mouse.wheelDelta * speed * zAxis;
}
