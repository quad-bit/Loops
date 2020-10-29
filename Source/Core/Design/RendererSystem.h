#pragma once
#include "System.h"
class RendererSystem : public System
{
public:
	RendererSystem();

	virtual void Init() override;
	virtual void DeInit() override;
	virtual void Render() override;

	virtual ~RendererSystem();
};

