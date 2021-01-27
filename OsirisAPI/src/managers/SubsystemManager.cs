using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
{
    class SubsystemManager
    {
        public static IntPtr Behaviour
        {
            get { return _NativeBehaviour; }
            set { 
                _NativeBehaviour = value;
                Manager_SetBehaviour(_NativeBehaviour);
            }
        }

        #region private variables 

        protected static IntPtr _NativeBehaviour;

        #endregion


        #region P/Invoke functions

        [DllImport("OsirisCAPI")]
        public static extern void Manager_SetBehaviour(IntPtr value);

        #endregion
    }
}
