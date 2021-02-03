using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Transform3D : GameObjectComponent
    {
        public override IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr)
        {
            return IntPtr.Zero;
        }
    }
}
