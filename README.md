# Echo Engine – Gold Version

> A modern, modular, and lightweight C++ game engine built for learning, prototyping, and creating 3D experiences.  
> Featuring a real-time editor, rendering, and dynamic scripting with hot reload.

---

## Table of Contents
- [Overview](#overview)
- [Key Features](#key-features)
  - [Resource & Scene Management](#-resource--scene-management)
  - [Editor & Workflow](#-editor--workflow)
  - [Rendering & Lighting](#-rendering--lighting)
  - [Camera System](#-camera-system)
  - [Scripting & Hot Reload](#-scripting--hot-reload)

---

## Overview
**Echo Engine** is a custom-built **game engine** designed to offer an accessible development environment.  
It provides real-time editing tools, a component-based architecture, and a rendering pipeline inspired by modern industry standards.

---

## Key Features

### Resource & Scene Management
- Import and manage multiple asset types: **models**, **textures**, **materials**, and **scripts**  
- Supported file types: `.obj`, `.png`, `.jpg`, `.h`, `.cpp`, `.cs`  
- **Automatic scene saving**: all GameObjects and layouts persist between sessions  
- Instantly reuse imported assets across the engine  

---

### Editor & Workflow
- **Hierarchy Window** – Organize your GameObjects with a clear parent-child view  
- **GameObject Creation** – Quickly create:
  - Cube  
  - Sphere  
  - Empty Object  
- **Inspector Panel** – View and modify GameObject components in real time:
  - Add or remove components (e.g., Mesh Renderer, Lights, Scripts)  
  - Edit materials, textures, and model properties  
- **Content Browser** – Manage your assets visually:
  - Import and preview models, textures, and scripts  
  - **Drag & drop** models directly into the scene to instantiate GameObjects  

---

### Rendering & Lighting
- Physically-Based Rendering (**PBR**) for realistic material and lighting simulation  
- Multiple light types:
  - **Directional Light**
  - **Point Light**
  - **Spot Light**
- **Outline Rendering** – Stylized contour effect for cel-shading or highlighting  
- **Anti-Aliasing** – Smooths edges for a cleaner, more polished final image  

---

### Camera System
- **Mouse Look** – Intuitive first-person navigation  
- **Arrow Keys** – Move through the scene  
- **Spacebar** – Adjust camera height  
- **Scroll Wheel** – Zoom in and out  

---

### Scripting & Hot Reload
- Lightweight **MonoBehaviour-style** scripting system for C#  
- Attach scripts to GameObjects for interactive and dynamic behaviors  
- **Hot Reload Support** – Edit and recompile scripts on the fly without restarting the engine  
