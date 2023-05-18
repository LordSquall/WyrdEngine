using System;
using System.Runtime.InteropServices;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Model : NativeObject
    {
        public Int32 Width;

        public Int32 Height;

        /// <summary>
        /// Default Constructor
        /// </summary>
        public Model() { }

        #region Native Properties

        #endregion

        public override string ToString()
        {
            return "Managed Model Object!";
        }
    }
}
