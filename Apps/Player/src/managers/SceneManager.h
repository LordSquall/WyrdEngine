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
    Wyrd::Scene* GetScene() const { return _Scene.get(); }

    void SetInitialScene(const Wyrd::UID& uid) { _InitialScene = uid; }
    const Wyrd::UID& GetInitialScene() const { return _InitialScene; }

    bool LoadInitialScene(const std::string& root);
    bool LoadScene(const std::string& root, const Wyrd::UID& uid);

private:
    Wyrd::UID _InitialScene;
    std::unique_ptr<Wyrd::Scene> _Scene;
};