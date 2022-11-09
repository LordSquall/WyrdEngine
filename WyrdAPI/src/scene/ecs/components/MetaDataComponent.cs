using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class MetaDataComponent : Component
    {

      [MarshalAs(UnmanagedType.ByValTStr, SizeConst=255)]
      private String _name;

      [MarshalAs(UnmanagedType.Struct)]
      private UID _uid;



      public String Name
      {
         get { return _name; }
         set 
         {
             _name = value;
             MetaDataComponent_SetName(Scene.NativePtr, Entity, _name);
         }
      }

      public UID Uid
      {
         get { return _uid; }
         set 
         {
             _uid = value;
             MetaDataComponent_SetUid(Scene.NativePtr, Entity, _uid);
         }
      }



        public ulong Entity { get; set; }

        public void SetEntity(ulong entity)
        {
            Entity = entity;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr MetaDataComponent_SetName(IntPtr scenePtr, UInt64 entity, String name);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr MetaDataComponent_SetUid(IntPtr scenePtr, UInt64 entity, UID uid);


        #endregion
    }
}
