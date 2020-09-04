using System;

namespace OsirisAPI
{
    abstract public class ScriptedObject
    {
        public GameObject GameObject { get; set; }

        void OnStart() { }
        void OnUpdate() { }

        void OnTriggerCollision(GameObject other) { }

        bool OnKeyDown(KeyCode key) { return false; }
        bool OnKeyPressed(KeyCode key) { return false; }
        bool OnKeyUp(KeyCode key) { return false; }
    }
}
