using System;
using System.Runtime.InteropServices;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Texture : NativeObject
    {
        public Int32 Width;

        public Int32 Height;

        /// <summary>
        /// Default Constructor
        /// </summary>
        public Texture() { }

        #region Native Properties

        #endregion

        public override string ToString()
        {
            return "Managed Texture Object!";
        }

        [DllImport("WyrdCAPI")]
        public static extern Int32 Texture_GetWidth(IntPtr nativePtr);

        [DllImport("WyrdCAPI")]
        public static extern Int32 Texture_GetHeight(IntPtr nativePtr);
    }
}
