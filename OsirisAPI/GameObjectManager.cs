using System;
using System.Collections.Generic;
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
    }
}
