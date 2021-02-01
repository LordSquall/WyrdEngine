using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
{
    class ResourceManager
    {
        private static Dictionary<int, Texture> _Textures = new Dictionary<int, Texture>();

        public static void Reset()
        {
            _Textures.Clear();
        }

        /// <summary>
        /// Links the managed object to a unmanaged pointer
        /// </summary>
        /// <param name="pointer"></param>
        public static void AddResource(Texture texture)
        {
            _Textures.Add(0, texture);
            Console.WriteLine("Texture Added ");
        }

        #region P/Invoke functions


        #endregion
    }
}
