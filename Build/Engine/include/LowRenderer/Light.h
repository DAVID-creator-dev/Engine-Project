#pragma once
#include "ILight.h"

class UiGUI;

class LightGL : public ILight{
public:
	LightGL() = default; 
	~LightGL() override = default;

	LightGL* GetLightGL() override { return this; }
	void DisplayComponentInInspector(InspectorUI* inspector) override;
};

class DirectionalLight : public LightGL
{
public:
	Vector3D direction;
	Vector3D color; 
};

class PointLight : public LightGL
{
public:
	Vector3D position;
	Vector3D color;
};

class SpotLight : public LightGL
{
public:
	Vector3D position;
	Vector3D direction;
	float spotAngle;
	float spotSmoothAngle;
	Vector3D color;
};
