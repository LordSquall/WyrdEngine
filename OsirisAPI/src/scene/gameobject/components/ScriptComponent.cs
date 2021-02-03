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
            _NativePointer = ScriptComponent_Create(gameObjectPtr);
            return _NativePointer;
        }

        public T CreateInstance<T>(GameObject gameObject) where T : ScriptedObject
        {
            Console.WriteLine("FOOBAR: {0}", gameObject.Name);
            ScriptComponent_CreateInstance(gameObject.NativePtr, NativePtr, typeof(T).Name);

            return (T)Instance;
        }

        #region P/Invoke functions

        [DllImport("OsirisCAPI")]
        public static extern IntPtr ScriptComponent_Create(IntPtr value);


        [DllImport("OsirisCAPI")]
        public static extern IntPtr ScriptComponent_CreateInstance(IntPtr gameObject, IntPtr component, [MarshalAs(UnmanagedType.LPStr)] String className);


        #endregion
    }
}
