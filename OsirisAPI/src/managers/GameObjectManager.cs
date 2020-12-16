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

        public static GameObject CreateGameObject(ref int uid)
        {
            GameObject newGameObject = new GameObject();

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
        public void LinkToManaged(int uid, IntPtr pointer)
        {
            GameObject gameObject = FindGameObject(uid);
            if (gameObject != null)
            {
                gameObject.NativePtr = pointer;
                Console.WriteLine("C#: Game Object linked to Native GameObject: " + gameObject.Name);

                int componentCount = GameObject_Get_Component_Count(gameObject.NativePtr);
                for (int i = 0; i < componentCount; ++i)
                {
                    IntPtr componentPtr;
                    int type;
                    GameObject_Get_Component(gameObject.NativePtr, i, out type, out componentPtr);

                    switch(type)
                    {
                        case 1: gameObject.AddComponent<Transform2D>(componentPtr); break;          // Transform2D
                        case 2: gameObject.AddComponent<Transform3D>(componentPtr); break;          // Transform3D
                        case 3: gameObject.AddComponent<SpriteRenderer>(componentPtr); break;       // SpriteRenderer
                        case 4: gameObject.AddComponent<ScriptComponent>(componentPtr); break;      // ScriptComponent
                        case 5: gameObject.AddComponent<PhysicsComponent>(componentPtr); break;     // PhysicsComponent
                    }
                }
               
            }
            else
            {
                Console.WriteLine("C#: Unable to find linkable GameObject : " + uid);
            }

        }

        #region P/Invoke functions

        [DllImport("OsirisCAPI")]
        public static extern int GameObject_Get_Component_Count(IntPtr value);

        [DllImport("OsirisCAPI")]
        public static extern void GameObject_Get_Component(IntPtr value, int idx, out int type, out IntPtr componentPtr);

        #endregion
    }
}
