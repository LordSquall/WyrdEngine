using System;
using System.Runtime.InteropServices;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class SpriteComponent : Component
    {
        [MarshalAs(UnmanagedType.Struct)]
        public Vector2 _position;

        [MarshalAs(UnmanagedType.Struct)]
        public Vector2 _size;
        
        [MarshalAs(UnmanagedType.Struct)]
        public Color _color;

        public ulong Entity { get; set; }

        public void SetEntity(ulong entity)
        {
            Entity = entity;
        }

        public Vector2 Position
        {
            get { return _position; }
            set {
                _position = value;
                SpriteComponent_SetPosition(Scene.NativePtr, Entity, _position);
            }
        }

        public Vector2 Size
        {
            get { return _size; }
            set
            {
                _size = value;
                SpriteComponent_SetSize(Scene.NativePtr, Entity, _size);
            }
        }

        public Color Color
        {
            get { return _color; }
            set
            {
                _color = value;
                SpriteComponent_SetColor(Scene.NativePtr, Entity, _color);
            }
        }

        public override string ToString()
        {
            return String.Format("[SpriteComponent] Position: {0},{1} Size: {2},{3} Color: {4},{5},{6},{7}", _position.X, _position.Y, _size.X, _size.Y, _color.R, _color.G, _color.B, _color.A);
        }

        #region P/Invoke functions

        [DllImport("WyrdCAPI")]
        public static extern IntPtr SpriteComponent_SetPosition(IntPtr scenePtr, UInt64 entity, Vector2 position);

        [DllImport("WyrdCAPI")]
        public static extern IntPtr SpriteComponent_SetSize(IntPtr scenePtr, UInt64 entity, Vector2 size);

        [DllImport("WyrdCAPI")]
        public static extern IntPtr SpriteComponent_SetColor(IntPtr scenePtr, UInt64 entity, Color color);

        #endregion
    }
}
