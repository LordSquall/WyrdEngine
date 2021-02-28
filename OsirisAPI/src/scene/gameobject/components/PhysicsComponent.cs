using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class PhysicsComponent : GameObjectComponent
    {
        private bool _IsTrigger;
        private bool _IsStatic;

        public bool IsTrigger
        {
            get
            {
                return PhysicsComponent_GetIsTrigger(_NativePointer);
            }
            set
            {
                _IsTrigger = value;
                PhysicsComponent_SetIsTrigger(_NativePointer, _IsTrigger);
            }
        }

        public bool IsStatic
        {
            get
            {
                return PhysicsComponent_GetIsStatic(_NativePointer);
            }
            set
            {
                _IsStatic = value;
                PhysicsComponent_SetIsStatic(_NativePointer, _IsStatic);
            }
        }

        public override IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr)
        {
            _NativePointer = PhysicsComponent_Create(gameObjectPtr);
            return _NativePointer;
        }


        [DllImport("OsirisCAPI")]
        public static extern IntPtr PhysicsComponent_Create(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr PhysicsComponent_SetIsTrigger(IntPtr value, bool isTrigger);

        [DllImport("OsirisCAPI")]
        public static extern bool PhysicsComponent_GetIsStatic(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr PhysicsComponent_SetIsStatic(IntPtr value, bool isStatic);

        [DllImport("OsirisCAPI")]
        public static extern bool PhysicsComponent_GetIsTrigger(IntPtr value);


    }
}
