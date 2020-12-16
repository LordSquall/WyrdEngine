using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    public class GameObject : UnmanagedObject
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

        #region Public Functions

        public void Move(float x, float y)
        {
            GameObject_Move(_NativePointer, x, y);
        }

        public void Rotate(float angle)
        {
            GameObject_Rotate(_NativePointer, angle);
        }

        public void Scale(float x, float y)
        {
            GameObject_Scale(_NativePointer, x, y);
        }

        public void SetVelocity(float vX, float vY)
        {
            GameObject_SetVelocity(_NativePointer, vX, vY);
        }

        public T GetComponent<T>() where T : UnmanagedObject
        {
            if (_components.ContainsKey(typeof(T)))
            {
                T newComponentWrapper = (T)Activator.CreateInstance(typeof(T));
                newComponentWrapper.NativePtr = _components[typeof(T)];
                return newComponentWrapper;
            }
            T d = default(T);

            return d;
        }

        public void AddComponent<T>(IntPtr nativeAddress) where T : UnmanagedObject
        {
            if (_components.ContainsKey(typeof(T)))
            {
                // error 
                return;
            }

            _components.Add(typeof(T), nativeAddress);
            return;
        }

        #endregion

        #region Private Variables

        private Dictionary<Type, IntPtr> _components = new Dictionary<Type, IntPtr>();

        #endregion


        #region Object Overrides

        public override string ToString() => Marshal.PtrToStringAnsi(GameObject_Name_Get(_NativePointer));

        #endregion

        #region P/Invoke functions

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Name_Get(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Move(IntPtr value, float x, float y);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Rotate(IntPtr value, float angle);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Scale(IntPtr value, float x, float y);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_SetVelocity(IntPtr value, float vX, float vY);

        #endregion
    }
}
