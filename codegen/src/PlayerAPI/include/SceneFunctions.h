/// Auto Generated
/// Date: May 2021 Wed 21:51:31

#pragma once

#include <wyrdpch.h>

#include "SceneFormat.h"

void Read(std::ifstream& s, SceneFormat_Config& d);
void Write(std::ofstream& s, SceneFormat_Config& d);

void Read(std::ifstream& s, SceneFormat_Transform2DComponent& d);
void Write(std::ofstream& s, SceneFormat_Transform2DComponent& d);

void Read(std::ifstream& s, SceneFormat_Transform3DComponent& d);
void Write(std::ofstream& s, SceneFormat_Transform3DComponent& d);

void Read(std::ifstream& s, SceneFormat_SpriteComponent& d);
void Write(std::ofstream& s, SceneFormat_SpriteComponent& d);

void Read(std::ifstream& s, SceneFormat_ScriptComponent& d);
void Write(std::ofstream& s, SceneFormat_ScriptComponent& d);

void Read(std::ifstream& s, SceneFormat_CameraComponent& d);
void Write(std::ofstream& s, SceneFormat_CameraComponent& d);

void Read(std::ifstream& s, SceneFormat_Component& d);
void Write(std::ofstream& s, SceneFormat_Component& d);

void Read(std::ifstream& s, SceneFormat_GameObject& d);
void Write(std::ofstream& s, SceneFormat_GameObject& d);

void Read(std::ifstream& s, SceneFormat_Layer& d);
void Write(std::ofstream& s, SceneFormat_Layer& d);

void Read(std::ifstream& s, SceneFormat_LayerConfig& d);
void Write(std::ofstream& s, SceneFormat_LayerConfig& d);

