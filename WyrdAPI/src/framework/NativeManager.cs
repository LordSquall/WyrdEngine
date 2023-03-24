using System;

namespace WyrdAPI
{
    public class NativeManager
    {
        static private IntPtr _NativePtr;

        public static IntPtr NativePtr
        {
            get
            {
                return _NativePtr;
            }
            set
            {
                _NativePtr = value;
            }
        }
    }
}
