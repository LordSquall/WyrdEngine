using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace WyrdAPI
{

    [StructLayout(LayoutKind.Sequential)]
    abstract public class ScriptedEntity
    {
        public Entity Entity { get; set; }

        public void RegisterEntity(UInt64 entityID)
        {
            Entity = EntityManager.FindEntityByIndex(entityID);
        }

        public virtual void OnStart() { }

        public virtual void OnUpdate(float elapsedTime) { }

        public virtual void OnTriggerCollision(Entity other) { }

        public virtual bool OnKeyDown(KeyCode key) { Console.WriteLine("ScriptedEntity::OnKeyDown!"); return false; }

        public virtual bool OnKeyPressed(KeyCode key) { Console.WriteLine("ScriptedEntity::OnKeyPressed!"); return false; }

        public virtual bool OnKeyUp(KeyCode key) { Console.WriteLine("ScriptedEntity::OnKeyUp!"); return false; }
    }
}
