#pragma once

struct FrameBuffer
{
	int width, height;
	unsigned char* data;

	FrameBuffer(int w, int h) : width(w), height(h)
	{
		data = new unsigned char[w * h * 4];
	}

	~FrameBuffer()
	{
		delete[] data;
	}

	unsigned char get(int x, int y, int e)
	{
		return data[(y * width + x) * 4 + e];
	}

	void set(unsigned char val, int x, int y, int e)
	{
		data[(y * width + x) * 4 + e] = val;
	}

	int getWidth() const
	{
		return width;
	}

	int getHeight() const
	{
		return height;
	}

	void clear()
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				int idx = (i * width + j) * 4;
				data[idx] = 0x00;      //R
				data[idx + 1] = 0x66;  //G
				data[idx + 2] = 0x66;  //B
				data[idx + 3] = 0xFF;  //A
			}
		}
	}
};

struct DepthBuffer
{
	int width, height;
	double* data;

	DepthBuffer(int w, int h) : width(w), height(h)
	{
		data = new double[w * h];
	}

	~DepthBuffer()
	{
		delete[] data;
	}

	double get(int x, int y)
	{
		return data[y * width + x];
	}

	void set(double val, int x, int y)
	{
		data[y * width + x] = val;
	}

	int getWidth() const
	{
		return width;
	}

	int getHeight() const
	{
		return height;
	}

	void clear()
	{
		for (int i = 0; i < width * height; ++i)
			data[i] = 1.0;
	}
};