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

      [MarshalAs(UnmanagedType.Struct)]
      private Vector2 _scale;



      public Vector2 Position
      {
         get { return _position; }
         set 
         {
             _position = value;
             Transform2DComponent_SetPosition(Scene.NativePtr, EntityID, _position);
         }
      }

      public float Rotation
      {
         get { return _rotation; }
         set 
         {
             _rotation = value;
             Transform2DComponent_SetRotation(Scene.NativePtr, EntityID, _rotation);
         }
      }

      public Vector2 Scale
      {
         get { return _scale; }
         set 
         {
             _scale = value;
             Transform2DComponent_SetScale(Scene.NativePtr, EntityID, _scale);
         }
      }



        public UInt64 EntityID { get; set; }

        public void SetEntity(Entity entity)
        {
            EntityID = entity.NativeID;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform2DComponent_SetPosition(IntPtr scenePtr, UInt64 entity, Vector2 position);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform2DComponent_SetRotation(IntPtr scenePtr, UInt64 entity, float rotation);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform2DComponent_SetScale(IntPtr scenePtr, UInt64 entity, Vector2 scale);


        #endregion
    }
}
