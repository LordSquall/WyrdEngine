using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace OsirisAPI
{

    [StructLayout(LayoutKind.Sequential)]
    abstract public class ScriptedObject
    {
        private GameObject _GameObject;

        public GameObject GameObject { 
            get { return _GameObject; }
            set
            {
                _GameObject = value;
            }
        }

        public virtual void OnStart() { }

        public virtual void OnUpdate(float elapsedTime) { }

        public virtual void OnTriggerCollision(GameObject other) { }

        public virtual bool OnKeyDown(KeyCode key) { return false; }

        public virtual bool OnKeyPressed(KeyCode key) { return false; }

        public virtual bool OnKeyUp(KeyCode key) { return false; }
    }
}
