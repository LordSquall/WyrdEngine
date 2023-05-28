using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class CameraComponent : Component
    {

      [MarshalAs(UnmanagedType.Struct)]
      private Rect _viewport;

      [MarshalAs(UnmanagedType.R4)]
      private float _aspectratio;

      [MarshalAs(UnmanagedType.Struct)]
      private Vector2 _size;



      public Rect Viewport
      {
         get { return _viewport; }
         set 
         {
             _viewport = value;
             CameraComponent_SetViewport(Scene.NativePtr, EntityID, _viewport);
         }
      }

      public float Aspectratio
      {
         get { return _aspectratio; }
         set 
         {
             _aspectratio = value;
             CameraComponent_SetAspectratio(Scene.NativePtr, EntityID, _aspectratio);
         }
      }

      public Vector2 Size
      {
         get { return _size; }
         set 
         {
             _size = value;
             CameraComponent_SetSize(Scene.NativePtr, EntityID, _size);
         }
      }



        public UInt64 EntityID { get; set; }

        public void SetEntity(Entity entity)
        {
            EntityID = entity.NativeID;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr CameraComponent_SetViewport(IntPtr scenePtr, UInt64 entity, Rect viewport);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr CameraComponent_SetAspectratio(IntPtr scenePtr, UInt64 entity, float aspectratio);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr CameraComponent_SetSize(IntPtr scenePtr, UInt64 entity, Vector2 size);


        #endregion
    }
}
