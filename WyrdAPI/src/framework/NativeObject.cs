using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class NativeObject 
    {
        private IntPtr _NativePtr;

        public IntPtr NativePtr
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
