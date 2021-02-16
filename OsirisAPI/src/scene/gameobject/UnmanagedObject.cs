using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class UnmanagedObject
    {
        public IntPtr NativePtr
        {
            get { return _NativePointer; }
            set { 
                _NativePointer = value;
               // Console.WriteLine("[{0}] : Settings Native Pointer: {1:X}", GetType().Name, value.ToInt64());
            }
        }

        #region private variables 

        protected IntPtr _NativePointer;

        #endregion
    }
}
