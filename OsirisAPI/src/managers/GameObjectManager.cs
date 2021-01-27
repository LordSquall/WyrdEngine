using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
{
    class GameObjectManager
    {
        private static Dictionary<int, GameObject> _GameObjects = new Dictionary<int, GameObject>();

        public static void Reset()
        {
            _GameObjects.Clear();
        }

        public static GameObject RegisterGameObject(ref int uid)
        {
            GameObject newGameObject = new GameObject(uid);

            _GameObjects.Add(uid, newGameObject);

            return newGameObject;
        }

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

        #region P/Invoke functions


        #endregion
    }
}
