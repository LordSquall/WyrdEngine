using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class SpriteComponent : Component
    {

      [MarshalAs(UnmanagedType.Bool)]
      private bool _enabled;

      [MarshalAs(UnmanagedType.Struct)]
      private UID _sprite;

      [MarshalAs(UnmanagedType.Struct)]
      private Vector2 _size;

      [MarshalAs(UnmanagedType.Struct)]
      private Color _tint;



      public bool Enabled
      {
         get { return _enabled; }
         set 
         {
             _enabled = value;
             SpriteComponent_SetEnabled(Scene.NativePtr, Entity, _enabled);
         }
      }

      public UID Sprite
      {
         get { return _sprite; }
         set 
         {
             _sprite = value;
             SpriteComponent_SetSprite(Scene.NativePtr, Entity, _sprite);
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

      public Color Tint
      {
         get { return _tint; }
         set 
         {
             _tint = value;
             SpriteComponent_SetTint(Scene.NativePtr, Entity, _tint);
         }
      }



        public ulong Entity { get; set; }

        public void SetEntity(ulong entity)
        {
            Entity = entity;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr SpriteComponent_SetEnabled(IntPtr scenePtr, UInt64 entity, bool enabled);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr SpriteComponent_SetSprite(IntPtr scenePtr, UInt64 entity, UID sprite);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr SpriteComponent_SetSize(IntPtr scenePtr, UInt64 entity, Vector2 size);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr SpriteComponent_SetTint(IntPtr scenePtr, UInt64 entity, Color tint);


        #endregion
    }
}
