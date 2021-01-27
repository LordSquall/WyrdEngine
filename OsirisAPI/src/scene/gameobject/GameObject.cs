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

        public GameObject(int uid)
        {
        }

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

        public T GetComponent<T>() where T : GameObjectComponent, new()
        {
            if (_components.ContainsKey(typeof(T)))
            {
                return (T)_components[typeof(T)];
            }

            return null;
        }

        public T AddComponent<T>() where T : GameObjectComponent, new()
        {
            if (_components.ContainsKey(typeof(T)))
            {
                // error 
                return default(T);
            }

            T component = new T();
            component.CreateUnmanagedPtr(_NativePointer);
            _components[typeof(T)] = component;

            return component;
        }

        public void AddChild(GameObject gameObject)
        {
            GameObject_AddChild(_NativePointer, gameObject.NativePtr);
        }

        public GameObject Create(GameObject parent = null)
        {
            IntPtr unmanagedGameObject = GameObject_Create(parent == null ? _NativePointer : parent.NativePtr);

            int uid = GameObject_UID_Get(unmanagedGameObject);

            GameObject managedGameObject = GameObjectManager.RegisterGameObject(ref uid);

            GameObjectManager.LinkToManaged(uid, unmanagedGameObject);

            return managedGameObject;
        }

        public T RegisterComponent<T>(IntPtr nativeAddress) where T : GameObjectComponent, new()
        {
            if (_components.ContainsKey(typeof(T)))
            {
                // error 
                return default(T);
            }

            T component = new T() { NativePtr = nativeAddress };
            _components[typeof(T)] = component;

            return component;
        }

        #endregion

        #region Private Variables

        private Dictionary<Type, GameObjectComponent> _components = new Dictionary<Type, GameObjectComponent>();

        #endregion


        #region Object Overrides

        public override string ToString() => Marshal.PtrToStringAnsi(GameObject_Name_Get(_NativePointer));

        #endregion

        #region P/Invoke functions

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Name_Get(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern int GameObject_UID_Get(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Move(IntPtr value, float x, float y);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Rotate(IntPtr value, float angle);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Scale(IntPtr value, float x, float y);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_SetVelocity(IntPtr value, float vX, float vY);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_AddChild(IntPtr value, IntPtr child);

        [DllImport("OsirisCAPI")]
        public static extern int GameObject_Get_Component_Count(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern void GameObject_Get_Component(IntPtr value, int idx, out int type, out IntPtr componentPtr);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Create(IntPtr value);

        #endregion
    }
}
