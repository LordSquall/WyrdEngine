using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public partial class ScriptComponent : Component
    {

      [MarshalAs(UnmanagedType.Bool)]
      private bool _enabled;

      [MarshalAs(UnmanagedType.Struct)]
      private UID _scriptid;

      [MarshalAs(UnmanagedType.I4)]
      private Int32 _instanceid;



      public bool Enabled
      {
         get { return _enabled; }
         set 
         {
             _enabled = value;
             ScriptComponent_SetEnabled(Scene.NativePtr, EntityID, _enabled);
         }
      }

      public UID Scriptid
      {
         get { return _scriptid; }
         set 
         {
             _scriptid = value;
             ScriptComponent_SetScriptid(Scene.NativePtr, EntityID, _scriptid);
         }
      }

      public Int32 Instanceid
      {
         get { return _instanceid; }
         set 
         {
             _instanceid = value;
             ScriptComponent_SetInstanceid(Scene.NativePtr, EntityID, _instanceid);
         }
      }



        public UInt64 EntityID { get; set; }

        public void SetEntity(Entity entity)
        {
            EntityID = entity.NativeID;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr ScriptComponent_SetEnabled(IntPtr scenePtr, UInt64 entity, bool enabled);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr ScriptComponent_SetScriptid(IntPtr scenePtr, UInt64 entity, UID scriptid);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr ScriptComponent_SetInstanceid(IntPtr scenePtr, UInt64 entity, Int32 instanceid);


        #endregion
    }
}
