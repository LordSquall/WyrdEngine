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

        public static IntPtr Physics
        {
            get { return _NativePhysics; }
            set
            {
                _NativePhysics = value;
                Manager_SetPhysics(_NativePhysics);
            }
        }

        public static IntPtr Resources
        {
            get { return _NativeResources; }
            set
            {
                _NativeResources = value;
                Manager_SetResources(_NativeResources);
            }
        }

        #region private variables 

        protected static IntPtr _NativeBehaviour;
        protected static IntPtr _NativePhysics;
        protected static IntPtr _NativeResources;

        #endregion


        #region P/Invoke functions

        [DllImport("OsirisCAPI")]
        public static extern void Manager_SetBehaviour(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern void Manager_SetPhysics(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern void Manager_SetResources(IntPtr value);

        #endregion
    }
}
