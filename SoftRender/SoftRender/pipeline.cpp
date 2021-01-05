#include "pipeline.h"

bool isBackFace(vec3 p[3])
{
	double det = p[0].x * (p[1].y - p[2].y) + p[1].x * (p[2].y - p[0].y) + p[2].x * (p[0].y - p[1].y);
	return det <= 0;
}

static int faceCulling = 0;

void FaceCulling(int flag)
{
	if (flag == 0)
		faceCulling = 0;
	else if (flag == 1)
		faceCulling = 1;
	else if (flag == 2)
		faceCulling = 2;
}

void DrawTriangle(int iface, FrameBuffer* framebuffer, DepthBuffer* zbuffer, IShader* shader, Viewport& viewport)
{
	vec4 vert_clip[3]; // triangle vertices in clip space
	for (int j = 0; j < 3; ++j)
		vert_clip[j] = shader->VS(iface, j);

	rasterize(vert_clip, framebuffer, zbuffer, shader, viewport);

}

void rasterize(vec4* vert_clip, FrameBuffer* framebuffer, DepthBuffer* zbuffer, IShader* shader, Viewport& viewport)
{
	// ndc coordinates of vertices
	vec3 vert_ndc[3];
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			vert_ndc[i][j] = vert_clip[i][j] / vert_clip[i].w;

	if (faceCulling == 1)
	{
		if (isBackFace(vert_ndc))
			return;
	}
	if (faceCulling == 2)
	{
		if (!isBackFace(vert_ndc))
			return;
	}
	

	// screen position of vertices
	vec2 pos_screen[3];
	for (int i = 0; i < 3; ++i)
	{
		pos_screen[i] = viewport.GetViewportPos(vec2(vert_ndc[i].x, vert_ndc[i].y));
	}

	// bounding box of the triangle
	vec2 bboxmin(1000000, 1000000);
	vec2 bboxmax(-1000000, -1000000);
	vec2 clamp(viewport.width - 1, viewport.height - 1);
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 2; ++j)
		{
			bboxmin[j] = std::max(0.0, std::min(bboxmin[j], pos_screen[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pos_screen[i][j]));
		}
	
	// rasterize
	for (int x = (int)bboxmin.x; x <= (int)bboxmax.x; ++x)
	{
		for (int y = (int)bboxmin.y; y <= (int)bboxmax.y; ++y)
		{
			vec3 bc_screen = barycentric2D(pos_screen, vec2(x, y));
			if (bc_screen.x < 0.0 || bc_screen.y < 0.0 || bc_screen.z < 0.0) continue;

			double depth = vert_ndc[0].z * bc_screen[0] + vert_ndc[1].z * bc_screen[1] + vert_ndc[2].z * bc_screen[2];
			depth = (-depth + 1.0) / 2.0;
			if (depth > zbuffer->get(x, y)) continue;
			zbuffer->set(depth, x, y);
			
			if (!framebuffer) continue;
			vec4 color;
			if (shader->FS(bc_screen, color)) continue;
			color *= 255.0;
			
			SetColor(framebuffer, x, y, color);
		}
	}

}

void SetColor(FrameBuffer* framebuffer, int x, int y, vec4 color)
{
	int index = ((framebuffer->height - y - 1) * framebuffer->width + x) * 4;
	for (int i = 0; i < 4; ++i)
	{
		framebuffer->data[index + i] = color[i];
	}
		
}
