using System;
using System.Runtime.InteropServices;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Material : NativeObject
    {
        public Int32 Width;

        public Int32 Height;

        /// <summary>
        /// Default Constructor
        /// </summary>
        public Material() { }

        #region Native Properties

        #endregion

        public override string ToString()
        {
            return "Managed Material Object!";
        }
    }
}
