using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public partial class Transform3DComponent : Component, INotifyPropertyChanged
    {

      [MarshalAs(UnmanagedType.Struct)]
      private Vector3 _position;

      [MarshalAs(UnmanagedType.Struct)]
      private Vector3 _rotation;

      [MarshalAs(UnmanagedType.Struct)]
      private Vector3 _scale;

      [MarshalAs(UnmanagedType.Struct)]
      private Matrix4 _modelmatrix;

      [MarshalAs(UnmanagedType.Struct)]
      private Matrix4 _parentmodelmatrix;


        public Vector3 Position
      {
         get { return _position; }
         set 
         {
             _position = value;
             Transform3DComponent_SetPosition(Scene.NativePtr, EntityID, _position);
         }
      }

      public Vector3 Rotation
      {
         get { return _rotation; }
         set 
         {
             _rotation = value;
             Transform3DComponent_SetRotation(Scene.NativePtr, EntityID, _rotation);
         }
      }

      public Vector3 Scale
      {
         get { return _scale; }
         set 
         {
             _scale = value;
             Transform3DComponent_SetScale(Scene.NativePtr, EntityID, _scale);
         }
      }

      public Matrix4 Modelmatrix
      {
         get { return _modelmatrix; }
         set 
         {
             _modelmatrix = value;
             Transform3DComponent_SetModelmatrix(Scene.NativePtr, EntityID, _modelmatrix);
         }
      }

      public Matrix4 Parentmodelmatrix
      {
         get { return _parentmodelmatrix; }
         set 
         {
             _parentmodelmatrix = value;
             Transform3DComponent_SetParentmodelmatrix(Scene.NativePtr, EntityID, _parentmodelmatrix);
         }
      }



        public UInt64 EntityID { get; set; }

        public void SetEntity(Entity entity)
        {
            EntityID = entity.NativeID;

            _position.PropertyChanged   += (obj, property) => { Transform3DComponent_SetPosition(Scene.NativePtr, EntityID, _position); };
            _rotation.PropertyChanged   += (obj, property) => { Transform3DComponent_SetRotation(Scene.NativePtr, EntityID, _rotation); };
            _scale.PropertyChanged      += (obj, property) => { Transform3DComponent_SetScale(Scene.NativePtr, EntityID, _scale); };
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void NotifyPropertyChanged(string property)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(property));
            }
        }

        #region P/Invoke functions

        [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform3DComponent_SetPosition(IntPtr scenePtr, UInt64 entity, Vector3 position);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform3DComponent_SetRotation(IntPtr scenePtr, UInt64 entity, Vector3 rotation);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform3DComponent_SetScale(IntPtr scenePtr, UInt64 entity, Vector3 scale);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform3DComponent_SetModelmatrix(IntPtr scenePtr, UInt64 entity, Matrix4 modelmatrix);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr Transform3DComponent_SetParentmodelmatrix(IntPtr scenePtr, UInt64 entity, Matrix4 parentmodelmatrix);


        #endregion
    }
}
