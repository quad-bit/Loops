#pragma once

#include <stdint.h>
#include <string>

class Settings
{
public:
	static uint32_t WINDOW_WIDTH, WINDOW_HEIGHT;
	std::string applicationName;
};

uint32_t Settings::WINDOW_WIDTH = 800;
uint32_t Settings::WINDOW_HEIGHT = 600;


class GraphicSettings : public Settings
{
private:

protected:

public:
	enum RenderCore
	{
		VULKAN,
		OPENGL
	};

	RenderCore renderingCoreType = RenderCore::VULKAN;
	std::string renderingOptions[2]{ "Vulkan", "OpenGL" };
};