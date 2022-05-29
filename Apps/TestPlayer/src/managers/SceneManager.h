#pragma once

/* core engine includes */
#include <core/UID.h>
#include <core/scene/Scene.h>
#include <core/interfaces/ISceneManager.h>

/**
* @brief Scene Manager class
*
* Handles scene management functions for the player
*/
class SceneManager : public Wyrd::ISceneManager
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
    bool ChangeScene(std::string& sceneName) override;

    Wyrd::Scene* GetScene() const { return _Scene.get(); }

    void SetInitialScene(const Wyrd::UID& uid) { _InitialScene = uid; }
    const Wyrd::UID& GetInitialScene() const { return _InitialScene; }

    bool AddScene(Wyrd::UID& uid, std::string& name);
    bool LoadInitialScene(const std::string& root);
    bool LoadScene(const std::string& root, const Wyrd::UID& uid);

private:
    Wyrd::UID _InitialScene;
    std::unique_ptr<Wyrd::Scene> _Scene;
    std::map<std::string, Wyrd::UID> _SceneMap;
};