using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class RelationshipComponent : Component
    {

      [MarshalAs(UnmanagedType.Struct)]
      private Entity _first;

      [MarshalAs(UnmanagedType.Struct)]
      private Entity _previous;

      [MarshalAs(UnmanagedType.Struct)]
      private Entity _next;

      [MarshalAs(UnmanagedType.Struct)]
      private Entity _parent;

      [MarshalAs(UnmanagedType.U4)]
      private UInt32 _childrencnt;

      [MarshalAs(UnmanagedType.I4)]
      private Int32 _depth;

      [MarshalAs(UnmanagedType.Bool)]
      private bool _remove;



      public Entity First
      {
         get { return _first; }
         set 
         {
             _first = value;
             RelationshipComponent_SetFirst(Scene.NativePtr, EntityID, _first);
         }
      }

      public Entity Previous
      {
         get { return _previous; }
         set 
         {
             _previous = value;
             RelationshipComponent_SetPrevious(Scene.NativePtr, EntityID, _previous);
         }
      }

      public Entity Next
      {
         get { return _next; }
         set 
         {
             _next = value;
             RelationshipComponent_SetNext(Scene.NativePtr, EntityID, _next);
         }
      }

      public Entity Parent
      {
         get { return _parent; }
         set 
         {
             _parent = value;
             RelationshipComponent_SetParent(Scene.NativePtr, EntityID, _parent);
         }
      }

      public UInt32 Childrencnt
      {
         get { return _childrencnt; }
         set 
         {
             _childrencnt = value;
             RelationshipComponent_SetChildrencnt(Scene.NativePtr, EntityID, _childrencnt);
         }
      }

      public Int32 Depth
      {
         get { return _depth; }
         set 
         {
             _depth = value;
             RelationshipComponent_SetDepth(Scene.NativePtr, EntityID, _depth);
         }
      }

      public bool Remove
      {
         get { return _remove; }
         set 
         {
             _remove = value;
             RelationshipComponent_SetRemove(Scene.NativePtr, EntityID, _remove);
         }
      }



        public UInt64 EntityID { get; set; }

        public void SetEntity(Entity entity)
        {
            EntityID = entity.NativeID;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr RelationshipComponent_SetFirst(IntPtr scenePtr, UInt64 entity, Entity first);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr RelationshipComponent_SetPrevious(IntPtr scenePtr, UInt64 entity, Entity previous);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr RelationshipComponent_SetNext(IntPtr scenePtr, UInt64 entity, Entity next);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr RelationshipComponent_SetParent(IntPtr scenePtr, UInt64 entity, Entity parent);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr RelationshipComponent_SetChildrencnt(IntPtr scenePtr, UInt64 entity, UInt32 childrencnt);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr RelationshipComponent_SetDepth(IntPtr scenePtr, UInt64 entity, Int32 depth);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr RelationshipComponent_SetRemove(IntPtr scenePtr, UInt64 entity, bool remove);


        #endregion
    }
}
