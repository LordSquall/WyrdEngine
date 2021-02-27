using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public abstract class GameObjectComponent : UnmanagedObject
    {
        public GameObject Owner { get; set; }

        public abstract IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr);
    }
}
