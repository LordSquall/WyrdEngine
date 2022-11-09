using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class CameraComponent : Component
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
             CameraComponent_SetEnabled(Scene.NativePtr, Entity, _enabled);
         }
      }

      public UID Scriptid
      {
         get { return _scriptid; }
         set 
         {
             _scriptid = value;
             CameraComponent_SetScriptid(Scene.NativePtr, Entity, _scriptid);
         }
      }

      public Int32 Instanceid
      {
         get { return _instanceid; }
         set 
         {
             _instanceid = value;
             CameraComponent_SetInstanceid(Scene.NativePtr, Entity, _instanceid);
         }
      }



        public ulong Entity { get; set; }

        public void SetEntity(ulong entity)
        {
            Entity = entity;
        }

        #region P/Invoke functions

         [DllImport("WyrdCAPI")]
         public static extern IntPtr CameraComponent_SetEnabled(IntPtr scenePtr, UInt64 entity, bool enabled);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr CameraComponent_SetScriptid(IntPtr scenePtr, UInt64 entity, UID scriptid);
         [DllImport("WyrdCAPI")]
         public static extern IntPtr CameraComponent_SetInstanceid(IntPtr scenePtr, UInt64 entity, Int32 instanceid);


        #endregion
    }
}
