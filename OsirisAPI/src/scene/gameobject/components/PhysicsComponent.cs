using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    public class PhysicsComponent : UnmanagedObject
    {
        public void SetIsTrigger(bool isTrigger)
        {
            PhysicsComponent_SetIsTrigger(_NativePointer, isTrigger);
        }

        [DllImport("OsirisCAPI")]
        public static extern IntPtr PhysicsComponent_SetIsTrigger(IntPtr value, bool isTrigger);
    }
}
