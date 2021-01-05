#pragma once
#include "shader.h"
#include "buffer.h"

struct Viewport
{
	int x = 0, y = 0; // lower left corner of the viewport rectangle
	double width = 0.0, height = 0.0; // the width and height of the viewport

	vec2 GetViewportPos(vec2 vn)
	{
		return vec2((vn.x + 1.0) * width / 2.0 + x, (vn.y + 1.0) * height / 2.0 + y);
	}
};

void FaceCulling(int flag);
void DrawTriangle(int iface, FrameBuffer* framebuffer, DepthBuffer* zbuffer, IShader *shader, Viewport &viewport);
void rasterize(vec4* vert_clip, FrameBuffer* framebuffer, DepthBuffer* zbuffer, IShader* shader, Viewport& viewport);

void SetColor(FrameBuffer* framebuffer, int x, int y, vec4 color);