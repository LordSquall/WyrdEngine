#pragma once

#include <core/UID.h>
#include <core/scene/Scene.h>

class SceneManager
{
public:
    static SceneManager& getInstance()
    {
        static SceneManager    instance;
        return instance;
    }

private:
	SceneManager(): _Scene(nullptr)  {}
    SceneManager(SceneManager const&) = delete;
    void operator=(SceneManager const&) = delete;

public:
    Osiris::Scene* GetScene() const { return _Scene.get(); }

    void SetInitialScene(const Osiris::UID& uid) { _InitialScene = uid; }
    const Osiris::UID& SetInitialScene() const { return _InitialScene; }

    bool LoadScene(const std::string& root, const Osiris::UID& uid);

private:
    Osiris::UID _InitialScene;
    std::unique_ptr<Osiris::Scene> _Scene;
};