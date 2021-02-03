using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace OsirisAPI
{

    [StructLayout(LayoutKind.Sequential)]
    abstract public class ScriptedObject
    {
        public GameObject GameObject { get; set; }

        public virtual void OnStart() { }

        public virtual void OnUpdate() { }

        public virtual void OnTriggerCollision(GameObject other) { }

        public virtual bool OnKeyDown(KeyCode key) { return false; }

        public virtual bool OnKeyPressed(KeyCode key) { return false; }

        public virtual bool OnKeyUp(KeyCode key) { return false; }
    }
}
