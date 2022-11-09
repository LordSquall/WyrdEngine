using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdGen
{
    public class GeneratorUtils
    {
        public static UnmanagedType GetUnmanagedType(TypeMappingDefinitions mappings, String type)
        {
            TypeMap map = mappings[type];
            if (map.HeapOnly)
            {
                return null;
            }

            return map.Unmanaged;
        }

        public static ManagedType GetManagedType(TypeMappingDefinitions mappings, String type)
        {
            TypeMap map = mappings[type];
            if (map.HeapOnly)
            {
                return null;
            }

            return map.Managed;
        }

        public static JsonType GetJsonType(TypeMappingDefinitions mappings, String type)
        {
            TypeMap map = mappings[type];
            if (map.HeapOnly)
            {
                return null;
            }

            return map.Json;
        }
    }
}
