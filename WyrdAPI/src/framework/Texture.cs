using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Texture
    {
        public IntPtr NativePtr { get; set; }

        public Texture() { }

        public override string ToString()
        {
            return "Texture::ToString()";
        }
    }
}
