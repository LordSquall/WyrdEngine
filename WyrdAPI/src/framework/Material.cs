using System;
using System.Runtime.InteropServices;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Material : NativeObject
    {

        /// <summary>
        /// Default Constructor
        /// </summary>
        public Material() { }

        public void SetProperty(String name, Texture texture)
        {
            Material_SetPropTexture(NativePtr, name, texture.NativePtr);
        }

        #region P/Invoke functions

        [DllImport("WyrdCAPI")]
        public static extern IntPtr Material_SetPropTexture(IntPtr nativePtr, String name, IntPtr texturePtr);


        #endregion

        public override string ToString()
        {
            return "Managed Material Object!";
        }
    }
}
