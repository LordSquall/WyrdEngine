using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
{
    public abstract class GameObjectComponent : UnmanagedObject
    {
        public abstract IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr);
    }
}
