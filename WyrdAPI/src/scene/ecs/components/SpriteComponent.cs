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
      private Texture _sprite;

      [MarshalAs(UnmanagedType.Struct)]
      private Vector2 _position;

      [MarshalAs(UnmanagedType.Struct)]
      private Vector2 _size;

      [MarshalAs(UnmanagedType.Struct)]
      private Vector2 _tiling;

      [MarshalAs(UnmanagedType.Struct)]
      private Color _color;



      public bool Enabled
      {
         get { return _enabled; }
         set 
         {
             _enabled = value;
             SpriteComponent_SetEnabled(Scene.NativePtr, EntityID, _enabled);
         }
      }

      public Texture Sprite
      {
         get { return _sprite; }
         set 
         {
             _sprite = value;
             SpriteComponent_SetSprite(Scene.NativePtr, EntityID, _sprite);
         }
      }

      public Vector2 Position
      {
         get { return _position; }
         set 
         {
             _position = value;
             SpriteComponent_SetPosition(Scene.NativePtr, EntityID, _position);
         }
      }

      public Vector2 Size
      {
         get { return _size; }
         set 
         {
             _size = value;
             SpriteComponent_SetSize(Scene.NativePtr, EntityID, _size);
         }
      }

      public Vector2 Tiling
      {
         get { return _tiling; }
         set 
         {
             _tiling = value;
             SpriteComponent_SetTiling(Scene.NativePtr, EntityID, _tiling);
         }
      }

      public Color Color
      {
         get { return _color; }
         set 
         {
             _color = value;
             SpriteComponent_SetColor(Scene.NativePtr, EntityID, _color);
         }
      }



        public UInt64 EntityID { get; set; }

        public void SetEntity(Entity entity)
        {
            EntityID = entity.NativeID;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr SpriteComponent_SetEnabled(IntPtr scenePtr, UInt64 entity, bool enabled);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr SpriteComponent_SetSprite(IntPtr scenePtr, UInt64 entity, Texture sprite);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr SpriteComponent_SetPosition(IntPtr scenePtr, UInt64 entity, Vector2 position);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr SpriteComponent_SetSize(IntPtr scenePtr, UInt64 entity, Vector2 size);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr SpriteComponent_SetTiling(IntPtr scenePtr, UInt64 entity, Vector2 tiling);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr SpriteComponent_SetColor(IntPtr scenePtr, UInt64 entity, Color color);


        #endregion
    }
}
