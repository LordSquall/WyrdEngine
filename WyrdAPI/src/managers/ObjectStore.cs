using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    public class ObjectStore
    {
        private static List<Object> _objectList = new List<Object>();

        static Object LastCreatedObject { get; set; }

        public static void Store(Object obj)
        {
            _objectList.Add(obj);

            LastCreatedObject = obj;
        }

        public static void Remove(Object obj)
        {
            _objectList.Remove(obj);
        }

        public static void Reset()
        {
            _objectList.Clear();
        }
    }
}
