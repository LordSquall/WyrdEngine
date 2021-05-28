using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Transform2DComponent : Component
    {
        [MarshalAs(UnmanagedType.Struct)]
        private Vector2 _position;

        [MarshalAs(UnmanagedType.R4)]
        private float _rotation;


        public Vector2 Position
        {
            get { return _position; }
            set
            {
                _position = value;
                Transform2DComponent_SetPosition(Scene.NativePtr, Entity, _position);
            }
        }

        public float Rotation
        {
            get { return _rotation; }
            set
            {
                _rotation = value;
                Transform2DComponent_SetRotation(Scene.NativePtr, Entity, _rotation);
            }
        }


        public ulong Entity { get; set; }

        public void SetEntity(ulong entity)
        {
            Entity = entity;
        }

        public override string ToString()
        {
            return String.Format("[Transform2DComponent] Position: {0},{1} Rotation: {2}", Position.X, Position.Y, Rotation);
        }

        #region P/Invoke functions

        [DllImport("WyrdCAPI")]
        public static extern IntPtr Transform2DComponent_SetPosition(IntPtr scenePtr, UInt64 entity, Vector2 position);

        [DllImport("WyrdCAPI")]
        public static extern IntPtr Transform2DComponent_SetRotation(IntPtr scenePtr, UInt64 entity, float rotation);

        #endregion
    }
}
