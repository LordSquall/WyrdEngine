using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public partial class MaterialComponent : Component
    {

      [MarshalAs(UnmanagedType.Bool)]
      private bool _enabled;

      [MarshalAs(UnmanagedType.Struct)]
      private Color _color;

      [MarshalAs(UnmanagedType.Struct)]
      private Material _material;



      public bool Enabled
      {
         get { return _enabled; }
         set 
         {
             _enabled = value;
             MaterialComponent_SetEnabled(Scene.NativePtr, EntityID, _enabled);
         }
      }

      public Color Color
      {
         get { return _color; }
         set 
         {
             _color = value;
             MaterialComponent_SetColor(Scene.NativePtr, EntityID, _color);
         }
      }

      public Material Material
      {
         get { return _material; }
         set 
         {
             _material = value;
             MaterialComponent_SetMaterial(Scene.NativePtr, EntityID, _material);
         }
      }



        public UInt64 EntityID { get; set; }

        public void SetEntity(Entity entity)
        {
            EntityID = entity.NativeID;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr MaterialComponent_SetEnabled(IntPtr scenePtr, UInt64 entity, bool enabled);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr MaterialComponent_SetColor(IntPtr scenePtr, UInt64 entity, Color color);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr MaterialComponent_SetMaterial(IntPtr scenePtr, UInt64 entity, Material material);


        #endregion
    }
}
