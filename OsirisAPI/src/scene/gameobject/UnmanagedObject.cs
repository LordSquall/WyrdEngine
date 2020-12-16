using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
{
    public class UnmanagedObject
    {
        public IntPtr NativePtr
        {
            get { return _NativePointer; }
            set { _NativePointer = value; }
        }

        #region private variables 

        protected IntPtr _NativePointer;

        #endregion
    }
}
