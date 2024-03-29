﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    public class Entity
    {
        public static Entity INVALID = new Entity() { _NativeID = 0 };

        private UInt64 _NativeID = 0;

        public UInt64 NativeID
        {
            get { return _NativeID; }
            set
            {
                _NativeID = value;
            }
        }

        public String Name
        {
            get
            {
                return GetComponent<MetaDataComponent>().Name;
            }
            set
            {
                MetaDataComponent c = GetComponent<MetaDataComponent>();
                c.Name = value;
            }
        }

        public T GetComponent<T>() where T : Component, new()
        {
            // check to see if we already have a component object
            // otherwise we need to retrieve the from the engine
            if (_components.ContainsKey(typeof(T)))
            {
                return (T)_components[typeof(T)];
            }
            else 
            {
                _components[typeof(T)] = Scene.GetComponent<T>(this);
                _components[typeof(T)].SetEntity(this);

                return (T)_components[typeof(T)];
            }
        }

        public T AddComponent<T>() where T : Component, new()
        {
            if (_components.ContainsKey(typeof(T)))
            {
                return (T)_components[typeof(T)];
            }
            else
            {
                _components[typeof(T)] = Scene.AddComponent<T>(this);
                _components[typeof(T)].SetEntity(this);

                return (T)_components[typeof(T)];
            }
        }

        private Dictionary<Type, Component> _components = new Dictionary<Type, Component>();

        public override string ToString()
        {
            return  _NativeID.ToString();
        }
    }
}
