using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    public class EntityManager
    {
        public static Dictionary<UInt64, Entity> _Entities = new Dictionary<ulong, Entity>();

        public static void Reset()
        {
            _Entities.Clear();
        }

        public static Entity FindEntityByIndex(UInt64 entity)
        {
            if (_Entities.ContainsKey(entity))
                return _Entities[entity];

            return null;
        }

        public static Entity CreateEntity(UInt64 entity)
        {
            // Create new entity
            Entity e = new Entity() { NativeID = entity };

            _Entities.Add(entity, e);

            //Console.WriteLine("Create Entity: {0}", entity);

            return e;
        }
    }
}
