using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public partial class CameraComponent : Component
    {

      [MarshalAs(UnmanagedType.Struct)]
      private Rect _viewport;

      [MarshalAs(UnmanagedType.R4)]
      private float _aspectratio;

      [MarshalAs(UnmanagedType.Struct)]
      private Vector2 _size;

      [MarshalAs(UnmanagedType.R4)]
      private float _nearplane;

      [MarshalAs(UnmanagedType.R4)]
      private float _farplane;

      [MarshalAs(UnmanagedType.R4)]
      private float _aspect;

      [MarshalAs(UnmanagedType.R4)]
      private float _fov;



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

      public float Nearplane
      {
         get { return _nearplane; }
         set 
         {
             _nearplane = value;
             CameraComponent_SetNearplane(Scene.NativePtr, EntityID, _nearplane);
         }
      }

      public float Farplane
      {
         get { return _farplane; }
         set 
         {
             _farplane = value;
             CameraComponent_SetFarplane(Scene.NativePtr, EntityID, _farplane);
         }
      }

      public float Aspect
      {
         get { return _aspect; }
         set 
         {
             _aspect = value;
             CameraComponent_SetAspect(Scene.NativePtr, EntityID, _aspect);
         }
      }

      public float Fov
      {
         get { return _fov; }
         set 
         {
             _fov = value;
             CameraComponent_SetFov(Scene.NativePtr, EntityID, _fov);
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
         [DllImport("WyrdCAPI")]
         public static extern IntPtr CameraComponent_SetNearplane(IntPtr scenePtr, UInt64 entity, float nearplane);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr CameraComponent_SetFarplane(IntPtr scenePtr, UInt64 entity, float farplane);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr CameraComponent_SetAspect(IntPtr scenePtr, UInt64 entity, float aspect);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr CameraComponent_SetFov(IntPtr scenePtr, UInt64 entity, float fov);


        #endregion
    }
}
