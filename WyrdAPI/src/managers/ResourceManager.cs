using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    public class ResourceManager
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
            _Textures.Add(_Textures.Count, texture);
            Console.WriteLine("Texture Added ");
        }

        public static Texture GetResource(int index)
        {
            return _Textures[index];
        }

        #region P/Invoke functions


        #endregion
    }
}
