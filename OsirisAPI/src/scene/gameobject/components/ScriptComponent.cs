using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class ScriptComponent : GameObjectComponent
    {
        /// <summary>
        /// User ScriptedObject instance
        /// </summary>
        public ScriptedObject Instance { get; set; }

        public override IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr)
        {
            GCHandle handle = GCHandle.Alloc(this);
            _NativePointer = ScriptComponent_Create(gameObjectPtr, GCHandle.ToIntPtr(handle));
            return _NativePointer;
        }

        public T CreateInstance<T>(GameObject gameObject) where T : ScriptedObject
        {
            ScriptComponent_CreateInstance(gameObject.NativePtr, NativePtr, typeof(T).Name);

            //Console.WriteLine("Instance: {0}", Instance.GameObject.ToString());
            return (T)Instance;
        }

        #region P/Invoke functions

        [DllImport("OsirisCAPI")]
        public static extern IntPtr ScriptComponent_Create(IntPtr value, IntPtr componentManagedObject);


        [DllImport("OsirisCAPI")]
        public static extern IntPtr ScriptComponent_CreateInstance(IntPtr gameObject, IntPtr component, [MarshalAs(UnmanagedType.LPStr)] String className);


        #endregion
    }
}
