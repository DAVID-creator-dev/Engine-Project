#pragma once
#include "Math/Math.h"
#include "Core/DataStructure/Component.h"

class LightGL;

class ILight : public Component
{
public:
	virtual ~ILight() = default;
	virtual LightGL* GetLightGL() = 0;
};