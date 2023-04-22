using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class MeshRendererComponent : Component
    {

      [MarshalAs(UnmanagedType.Bool)]
      private bool _enabled;

      [MarshalAs(UnmanagedType.Struct)]
      private Color _color;

      [MarshalAs(UnmanagedType.Struct)]
      private Material _material;

      [MarshalAs(UnmanagedType.Struct)]
      private Model _model;



      public bool Enabled
      {
         get { return _enabled; }
         set 
         {
             _enabled = value;
             MeshRendererComponent_SetEnabled(Scene.NativePtr, Entity, _enabled);
         }
      }

      public Color Color
      {
         get { return _color; }
         set 
         {
             _color = value;
             MeshRendererComponent_SetColor(Scene.NativePtr, Entity, _color);
         }
      }

      public Material Material
      {
         get { return _material; }
         set 
         {
             _material = value;
             MeshRendererComponent_SetMaterial(Scene.NativePtr, Entity, _material);
         }
      }

      public Model Model
      {
         get { return _model; }
         set 
         {
             _model = value;
             MeshRendererComponent_SetModel(Scene.NativePtr, Entity, _model);
         }
      }



        public ulong Entity { get; set; }

        public void SetEntity(ulong entity)
        {
            Entity = entity;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr MeshRendererComponent_SetEnabled(IntPtr scenePtr, UInt64 entity, bool enabled);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr MeshRendererComponent_SetColor(IntPtr scenePtr, UInt64 entity, Color color);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr MeshRendererComponent_SetMaterial(IntPtr scenePtr, UInt64 entity, Material material);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr MeshRendererComponent_SetModel(IntPtr scenePtr, UInt64 entity, Model model);


        #endregion
    }
}
