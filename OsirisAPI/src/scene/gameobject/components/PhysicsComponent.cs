using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class PhysicsComponent : GameObjectComponent
    {
        public void SetIsTrigger(bool isTrigger)
        {
            PhysicsComponent_SetIsTrigger(_NativePointer, isTrigger);
        }

        public override IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr)
        {
            return IntPtr.Zero;
        }

        [DllImport("OsirisCAPI")]
        public static extern IntPtr PhysicsComponent_SetIsTrigger(IntPtr value, bool isTrigger);
    }
}
