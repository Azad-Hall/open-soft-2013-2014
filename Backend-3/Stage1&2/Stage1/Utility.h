#include <stdlib.h>

#ifndef __UTILITY_H_
#define __UTILITY_H_

class Pixel
{
public:
	int x,y;

	Pixel(int xx = -1, int yy = -1)
	{
		x = xx;
		y = yy;
	}

	bool operator==(Pixel* rhs)
	{
		return (x == rhs->x && y == rhs->y);
	}
};

class Stack
{
	Pixel* stack;
	int top;
	
public:
	Stack()
	{
		stack = (Pixel*)calloc(1229312, sizeof(Pixel));
		top = 0;
	}

	Stack(int height)
	{
		stack = (Pixel*)calloc(height, sizeof(Pixel));
		top = 0;
	}

	
	~Stack()
	{
		free(stack);
	}

	void Push(Pixel p)
	{
		stack[top].x = p.x;
		stack[top].y = p.y;
		top++;
	}

	Pixel* Pop()
	{
		//Pixel* p = NULL;
		if(top > 0)
		{
			top--;
			return &(stack[top]);
			//p = new Pixel(stack[top].x, stack[top].y);

		}
		return NULL;
	}

	bool Empty()
	{
		if(top == 0)
			return true;
		else
			return false;
	}

};

class Link
{
public:
	Pixel* data;
	Link* next;

	Link(Pixel* data, Link* nextLink = NULL)
	{
		this->data = data;
		next = nextLink;
	}

	~Link()
	{
		delete data;
	}
};

#endif