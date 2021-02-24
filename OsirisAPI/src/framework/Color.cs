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
        public float R { get; set; } = 1.0f;
        public float G { get; set; } = 1.0f;
        public float B { get; set; } = 1.0f;
        public float A { get; set; } = 1.0f;
        public override string ToString()
        {
            return String.Format("Color: {0}, {1}, {2}, {3}", R, G, B, A);
        }
    }
}
