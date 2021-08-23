#pragma once


/* core engine includes */
#include <core/UID.h>
#include <core/scene/Scene.h>
#include <core/interfaces/ISceneManager.h>

namespace Wyrd::Editor
{
    /**
     * @brief Editor Scene Manager class
     *
     * Handles all scene management function during an active simulation
    */
    class EditorSceneManager : public Wyrd::ISceneManager
    {
    public:
        /**
         * @brief Constructor
        */
        EditorSceneManager() : _InitialScenePath("") {}
    public:
        /**
         * see Wyrd::ISceneManager::ChangeScene(std::string&);
        */
        bool ChangeScene(std::string& sceneName) override;

        /**
          * @brief Reloads the currently set initial scene
          * @param scenePath
          * @return true if scene loads successfully, false otherwise
         */
        bool ResetInitialScene();
        
        /**
          * @brief Sets the initial scene path at the start of the simulation.
          * This is the scene that will be reloaded once the simulation is has stopped
          * @param scenePath
         */
        inline void SetInitialScene(const std::string& scenePath) { _InitialScenePath = scenePath; }

    private:
        std::string _InitialScenePath;
    };
}