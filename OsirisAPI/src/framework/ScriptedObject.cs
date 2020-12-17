using System;
using System.Collections.Generic;

namespace OsirisAPI
{
    abstract public class ScriptedObject
    {
        public GameObject GameObject { get; set; }

        public void OnStart() { }

        public void OnUpdate() { }

        public void OnTriggerCollision(GameObject other) { }

        public bool OnKeyDown(KeyCode key) { return false; }
        public bool OnKeyPressed(KeyCode key) { return false; }
        public bool OnKeyUp(KeyCode key) { return false; }
    }
}
