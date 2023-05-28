using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class MeshComponent : Component
    {

      [MarshalAs(UnmanagedType.Bool)]
      private bool _enabled;

      [MarshalAs(UnmanagedType.Struct)]
      private Color _color;



      public bool Enabled
      {
         get { return _enabled; }
         set 
         {
             _enabled = value;
             MeshComponent_SetEnabled(Scene.NativePtr, EntityID, _enabled);
         }
      }

      public Color Color
      {
         get { return _color; }
         set 
         {
             _color = value;
             MeshComponent_SetColor(Scene.NativePtr, EntityID, _color);
         }
      }



        public UInt64 EntityID { get; set; }

        public void SetEntity(Entity entity)
        {
            EntityID = entity.NativeID;
        }

        #region P/Invoke functions

        [DllImport("WyrdCAPI")]
         public static extern IntPtr MeshComponent_SetEnabled(IntPtr scenePtr, UInt64 entity, bool enabled);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr MeshComponent_SetColor(IntPtr scenePtr, UInt64 entity, Color color);


        #endregion
    }
}
