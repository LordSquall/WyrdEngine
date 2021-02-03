using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
{
    public class GameObjectManager
    {
        /// <summary>
        /// Map off all the gameobjects within the domain
        /// </summary>
        private static List<GameObject> _GameObjects = new List<GameObject>();

        /// <summary>
        /// Reset the Manager between executions
        /// </summary>
        public static void Reset()
        {
            _GameObjects.Clear();
        }
        
        public static void RegisterGameObject(GameObject gameObject)
        {
            _GameObjects.Add(gameObject);
            Console.WriteLine("GameObject Registered!!!");
        }

        /// <summary>
        /// Create and store a new gameobject agasint a given uid
        /// </summary>
        /// <param name="uid"></param>
        /// <returns></returns>
        public static GameObject CreateGameObject(ref int uid)
        {
            GameObject newGameObject = new GameObject();

            //_GameObjects.Add(uid, newGameObject);

            return newGameObject;
        }

        /// <summary>
        /// Find a Registered GameObject by uid
        /// </summary>
        /// <param name="uid"></param>
        /// <returns></returns>
        public static GameObject FindGameObject(int uid)
        {
            return _GameObjects[uid];
        }

        /// <summary>
        /// Links the managed object to a unmanaged pointer
        /// </summary>
        /// <param name="pointer"></param>
        public static void LinkToManaged(int uid, IntPtr pointer)
        {
            GameObject gameObject = FindGameObject(uid);
            if (gameObject != null)
            {
                gameObject.NativePtr = pointer;

                int componentCount = GameObject.GameObject_Get_Component_Count(gameObject.NativePtr);
                for (int i = 0; i < componentCount; ++i)
                {
                    IntPtr componentPtr;
                    int type;
                    GameObject.GameObject_Get_Component(gameObject.NativePtr, i, out type, out componentPtr);

                    switch (type)
                    {
                        case 1: gameObject.RegisterComponent<Transform2D>(componentPtr); break;          // Transform2D
                        case 2: gameObject.RegisterComponent<Transform3D>(componentPtr); break;          // Transform3D
                        case 3: gameObject.RegisterComponent<SpriteComponent>(componentPtr); break;      // SpriteRenderer
                        case 4: gameObject.RegisterComponent<ScriptComponent>(componentPtr); break;      // ScriptComponent
                        case 5: gameObject.RegisterComponent<PhysicsComponent>(componentPtr); break;     // PhysicsComponent
                    }
                }
               
            }
            else
            {
                Console.WriteLine("C#: Unable to find linkable GameObject : " + uid);
            }

        }

        /// <summary>
        /// Create a new instance of a ScripedObject
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public static T CreateScriptInstance<T>(GameObject gameObject) where T : ScriptedObject, new()
        {
            T t = new T();
            IntPtr obj = Manager_CreateScriptedObject(gameObject.NativePtr, typeof(T).Name);

            //Marshal.PtrToStructure(obj, t);

           // Console.WriteLine(t.GameObject.Name);

            return t;
        }

        #region P/Invoke functions

        [DllImport("OsirisCAPI")]
        public static extern IntPtr Manager_CreateScriptedObject(IntPtr gameObject, [MarshalAs(UnmanagedType.LPStr)] String className);

        #endregion
    }
}
