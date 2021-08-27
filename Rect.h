#pragma once

class Rect
{

public:
	Rect()
	{
		this->width = 0;
		this->height = 0;
		this->left = 0;
		this->top = 0;
	}

	Rect(int width, int height)
	{
		this->width = width;
		this->height = height;
		this->left = 0;
		this->top = 0;
	}

	Rect(int left, int top, int width, int height)
	{
		this->width = width;
		this->height = height;
		this->left = left;
		this->top = top;
	}

	Rect(const Rect& rect)
	{
		this->width = rect.width;
		this->height = rect.height;
		this->left = rect.left;
		this->top = rect.top;
	}

	~Rect() = default;

public:
	int width, height, left, top;

};
