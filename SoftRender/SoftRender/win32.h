#pragma once
#include <Windows.h>
#include "math.h"

typedef struct mouse
{
	vec2 orbitPos;
	vec2 orbitDelta;
	double wheelDelta;
}mouseInfo;

typedef struct window
{
	bool close;
	HWND hWnd;
	HDC memDC;
	HBITMAP bmOld;
	HBITMAP bmDIB;
	unsigned char* wndFrameBuf;
	int width;
	int height;
	mouseInfo mouse;
	bool button[2];
	bool key[512];
	TRACKMOUSEEVENT tme;
	long long int timerFreq;
}Window;

extern Window* win;

void InitWindow(int width, int height);
void HandleWindowMessage();
void DestroyWindow();
void DrawWindow(unsigned char *buf);
double GetTime();
vec2 GetMousePos();
