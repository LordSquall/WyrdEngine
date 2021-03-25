using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Transform2D : GameObjectComponent
    {
        private Vector2 _Position = new Vector2();
        private float _Rotation = 0.0f;
        private Vector2 _Scale = new Vector2();


        public Vector2 Position
        {
            get { 
                Transform2D_GetPosition(_NativePointer, _Position);
                return _Position;
            }
            set { 
                _Position = value;
                Transform2D_SetPosition(_NativePointer, _Position.X, _Position.Y);
            }
        }


        public override IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr)
        {
            return IntPtr.Zero;
        }

        [DllImport("OsirisCAPI")]
        public static extern void Transform2D_SetPosition(IntPtr value, float x, float y);

        [DllImport("OsirisCAPI")]
        public static extern void Transform2D_GetPosition(IntPtr value, Vector2 vec);
    }
}
