using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class CameraComponent : GameObjectComponent
    {
        private Vector2 _Position = new Vector2();


        public Vector2 Position
        {
            get { 
                return _Position;
            }
            set { 
                _Position = value;
            }
        }


        public override IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr)
        {
            return IntPtr.Zero;
        }
    }
}
