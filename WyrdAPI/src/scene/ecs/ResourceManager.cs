using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    public class ResourceManager
    {
        public static List<Texture> _Textures = new List<Texture>();

        public static void Reset()
        {
            _Textures.Clear();
        }

        public static void AddTexture(IntPtr nativePtr)
        {
            _Textures.Add(new Texture() { NativePtr = nativePtr });
        }
    }
}
