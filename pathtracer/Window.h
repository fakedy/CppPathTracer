#pragma once
class Window
{
public:

	virtual void init() = 0;
	virtual bool windowShouldClose() = 0;
	virtual void update() = 0;
	virtual void shutdown() = 0;
};

