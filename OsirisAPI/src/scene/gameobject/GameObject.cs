﻿using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
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

        public GameObject Parent { get; set; }

        public Transform2D Transform
        {
            get
            {
                return GetComponent<Transform2D>();
            }
        }
       
        #endregion

        public GameObject()
        {
            GameObjectManager.RegisterGameObject(this);
        }

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

        public void SetAngle(float angle)
        {
            GameObject_SetAngle(_NativePointer, angle);
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

        public void AddComponent(GameObjectComponent component)
        {
            if (!_components.ContainsKey(component.GetType()))
            {
                component.Owner = this;
                _components[component.GetType()] = component;
            }
        }
        
        public T CreateComponent<T>(string typeName = null) where T : GameObjectComponent, new()
        {
            if (_components.ContainsKey(typeof(T)))
            {
                // error 
                return default(T);
            }

            // create a new component object 
            T component = new T();

            // set the component owner
            component.Owner = this;

            // add to the object store to ensure it's not targeted by the GC
            ObjectStore.Store(component);

            component.CreateUnmanagedPtr(_NativePointer);
            _components[typeof(T)] = component;

            if (typeName != null)
            {

            }

            return component;
        }

        public void AddChild(GameObject gameObject)
        {
            GameObject_AddChild(_NativePointer, gameObject.NativePtr);

            gameObject.Parent = this;
            _children.Add(gameObject);
        }

        public GameObject Create(string name, GameObject parent = null)
        {
            GameObject newGameObject = new GameObject();
            ObjectStore.Store(newGameObject);

            GameObject_Create(parent == null ? _NativePointer : parent.NativePtr, name, true);

            newGameObject.Parent = parent == null ? this : parent;

            return newGameObject;
        }

        public T RegisterComponent<T>(IntPtr nativeAddress) where T : GameObjectComponent, new()
        {
            if (_components.ContainsKey(typeof(T)))
            {
                // error 
                return default(T);
            }

            T component = new T() { NativePtr = nativeAddress, Owner = this };
            _components[typeof(T)] = component;

            return component;
        }


        public GameObject FindChild(string name)
        {
            return _children.Find(x => x.Name.Equals(name));
        }

        #endregion

        #region Private Variables

        private Dictionary<Type, GameObjectComponent> _components = new Dictionary<Type, GameObjectComponent>();

        private List<GameObject> _children = new List<GameObject>();


        #endregion

        #region private Methods

        private void _AddGameObject(GameObject gameObject)
        {
            // set the parent of the gameobject
            gameObject.Parent = this;
            _children.Add(gameObject);
        }

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
        public static extern IntPtr GameObject_SetAngle(IntPtr value, float angle);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_SetVelocity(IntPtr value, float vX, float vY);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_AddChild(IntPtr value, IntPtr child);

        [DllImport("OsirisCAPI")]
        public static extern int GameObject_Get_Component_Count(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern void GameObject_Get_Component(IntPtr value, int idx, out int type, out IntPtr componentPtr);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr GameObject_Create(IntPtr parentGameObject, [MarshalAs(UnmanagedType.LPStr)] String name, bool retrieveManagedObject);

        #endregion
    }
}
