using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    public class ScriptComponent : GameObjectComponent
    {
        public override IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr)
        {
            return IntPtr.Zero;
        }

    }
}
