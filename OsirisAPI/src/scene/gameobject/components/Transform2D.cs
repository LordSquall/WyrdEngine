using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Transform2D : GameObjectComponent
    {
        public void SetPosition(float x, float y)
        {
            Transform2D_SetPosition(_NativePointer, x, y);
        }

        public void SetRotation(float angle)
        {
            Transform2D_SetRotation(_NativePointer, angle);
        }

        public void SetScale(float sx, float sy)
        {
            Transform2D_SetScale(_NativePointer, sx, sy);
        }

        public override IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr)
        {
            return IntPtr.Zero;
        }

        [DllImport("OsirisCAPI")]
        public static extern void Transform2D_SetPosition(IntPtr value, float x, float y);

        [DllImport("OsirisCAPI")]
        public static extern void Transform2D_SetRotation(IntPtr value, float angle);

        [DllImport("OsirisCAPI")]
        public static extern void Transform2D_SetScale(IntPtr value, float sx, float sy);
    }
}
