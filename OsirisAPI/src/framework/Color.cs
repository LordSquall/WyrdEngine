using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Color
    {
        public float R;
        public float G;
        public float B;
        public float A;
    }
}
