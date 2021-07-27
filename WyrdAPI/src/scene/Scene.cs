using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    public class Scene
    {
        public static IntPtr NativePtr { get; set; }

        private static Dictionary<Type, int> _ScriptClassMap = new Dictionary<Type, int>();


        public static void Reset()
        {
            Console.WriteLine("Scene Resetting!");
            _ScriptClassMap.Clear();
            EntityManager.Reset();
        }

        public static void BuildComponentList()
        {
            Scene_BuildComponentList(NativePtr);
        }

        public static Entity FindEntityByName(String name)
        {
            var pair = EntityManager._Entities.FirstOrDefault(t => t.Value.Name == name);
            return pair.Value;
        }

        public static void AddPoolRegistration(string scriptName, int idx)
        {
            Console.WriteLine("AddPoolRegistration: {0} {1}", idx, scriptName);

            // find the class with matching script name
            Type scriptType = Type.GetType("WyrdAPI." + scriptName);
            if (scriptType == null)
            {
                Console.WriteLine("Unable to find Class: {0}", scriptName);
                return;
            }

            // store the mapping between the script class type and the native component pool index
            _ScriptClassMap[scriptType] = idx;
        }


        public static T GetComponent<T>(Entity entity) where T : Component
        {
            // check to ensure we have the script type mapped
            if (!_ScriptClassMap.ContainsKey(typeof(T)))
            {
                Console.WriteLine("Attempting to GetComponent on an unmapped native script type: {0}", typeof(T).Name);
                return default(T);
            }

            IntPtr nativePtr = Scene_GetComponent(NativePtr, _ScriptClassMap[typeof(T)], entity.NativeID);
            T component = Marshal.PtrToStructure<T>(nativePtr);
            return component;
        }

        #region P/Invoke functions

        [DllImport("WyrdCAPI")]
        public static extern IntPtr Scene_GetComponent(IntPtr scenePtr, Int32 componentPoolIndex, UInt64 entity);

        [DllImport("WyrdCAPI")]
        public static extern IntPtr Scene_BuildComponentList(IntPtr scenePtr);

        #endregion

    }
}
