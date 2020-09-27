using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    public class GameObject
    {
        #region public properties

        public String Name
        {
            get
            {
                return Marshal.PtrToStringAnsi(GameObject_Name_Get(_NativePointer));
            }
        }
       
        #endregion

        /// <summary>
        /// Links the managed object to a unmanaged pointer
        /// </summary>
        /// <param name="pointer"></param>
        public void LinkToManaged(IntPtr pointer)
        {
            _NativePointer = pointer;

            Console.WriteLine("C#: Game Object linked to Native GameObject: " + Name);
        }

        #region Public Functions

        public void Move(float x, float y)
        {
            GameObject_Move(_NativePointer, x, y);
        }

        public void SetVelocity(float vX, float vY)
        {
            GameObject_SetVelocity(_NativePointer, vX, vY);
        }

        #endregion

        #region Object Overrides

        public override string ToString() => Marshal.PtrToStringAnsi(GameObject_Name_Get(_NativePointer));

        #endregion

        #region private variables 

        private IntPtr _NativePointer;

        #endregion

        #region P/Invoke functions
        
        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Name_Get(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Move(IntPtr value, float x, float y);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_SetVelocity(IntPtr value, float vX, float vY);

        #endregion
    }
}
