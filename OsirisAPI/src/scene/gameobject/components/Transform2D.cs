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

        public float Rotation
        {
            get
            {
                Transform2D_GetRotation(_NativePointer, ref _Rotation);
                return _Rotation;
            }
            set {
                _Rotation = value;
                Transform2D_SetRotation(_NativePointer, _Rotation);
            }
        }

        public Vector2 Scale
        {
            get
            {
                Transform2D_GetScale(_NativePointer, _Scale);
                return _Scale;
            }
            set { 
                _Scale = value;
                Transform2D_SetScale(_NativePointer, _Scale.X, _Scale.Y);
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

        [DllImport("OsirisCAPI")]
        public static extern void Transform2D_SetRotation(IntPtr value, float angle);

        [DllImport("OsirisCAPI")]
        public static extern void Transform2D_GetRotation(IntPtr value, ref float angle);

        [DllImport("OsirisCAPI")]
        public static extern void Transform2D_SetScale(IntPtr value, float sx, float sy);

        [DllImport("OsirisCAPI")]
        public static extern void Transform2D_GetScale(IntPtr value, Vector2 vec);
    }
}
