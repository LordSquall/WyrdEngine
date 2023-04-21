using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Transform3DComponent : Component
    {

      [MarshalAs(UnmanagedType.Struct)]
      private Vector3 _position;

      [MarshalAs(UnmanagedType.Struct)]
      private Vector3 _rotation;

      [MarshalAs(UnmanagedType.Struct)]
      private Vector3 _scale;



      public Vector3 Position
      {
         get { return _position; }
         set 
         {
             _position = value;
             Transform3DComponent_SetPosition(Scene.NativePtr, Entity, _position);
         }
      }

      public Vector3 Rotation
      {
         get { return _rotation; }
         set 
         {
             _rotation = value;
             Transform3DComponent_SetRotation(Scene.NativePtr, Entity, _rotation);
         }
      }

      public Vector3 Scale
      {
         get { return _scale; }
         set 
         {
             _scale = value;
             Transform3DComponent_SetScale(Scene.NativePtr, Entity, _scale);
         }
      }



        public ulong Entity { get; set; }

        public void SetEntity(ulong entity)
        {
            Entity = entity;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform3DComponent_SetPosition(IntPtr scenePtr, UInt64 entity, Vector3 position);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform3DComponent_SetRotation(IntPtr scenePtr, UInt64 entity, Vector3 rotation);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform3DComponent_SetScale(IntPtr scenePtr, UInt64 entity, Vector3 scale);


        #endregion
    }
}
