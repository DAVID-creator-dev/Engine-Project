#pragma once

class GameObject;
class Camera;

class RenderInterface;
class LightEngine;
class ResourceManager;
class ScriptManager;
class GameStateManager; 
class HierarchyUI; 

class InspectorUI
{
public:
    InspectorUI(); 
    ~InspectorUI() = default; 

    void Render(HierarchyUI* hierarchy);

    void MeshRenderer();
    void MeshFilter();
    void MaterialComponent();
    void LightComponent();
    void DirectionalLightComponent();
    void SpotLightComponent();
    void PointLightComponent();
    void RigidBodyComponent();
    void SphereColliderComponent();
    void BoxColliderComponent();
    void CapsuleColliderComponent();
    void TextComponent();
    void CameraComponent();
    void SoundDisplay();
    void ScriptDisplay();

private:
    void TransformComponent() const;
    void DisplayComponents();
    void AddComponents();
    void AddScriptComponents();

    bool addComponent = false;
    bool showInspector = true;

    GameObject* currentObject = nullptr;

    RenderInterface* renderInterface = nullptr;
    LightEngine* lightEngine = nullptr;
    ResourceManager* resourceManager = nullptr;
    ScriptManager* scriptManager = nullptr;
    GameStateManager* gameStateManager = nullptr; 
};