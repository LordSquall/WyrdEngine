using System;

namespace OsirisAPI
{
    abstract public class ScriptedObject
    {
        public GameObject GameObject { get; set; }

        void OnStart() { }
        void OnUpdate() { }
    }
}
