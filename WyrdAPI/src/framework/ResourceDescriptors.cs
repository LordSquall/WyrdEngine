using System;
using System.Runtime.InteropServices;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ManagedTextureDesc {
        public IntPtr NativePtr;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] GUID;
        public Int32 Width;
        public Int32 Height;
    }
}
