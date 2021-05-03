using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Color
    {
        public float R { get; set; } = 1.0f;
        public float G { get; set; } = 1.0f;
        public float B { get; set; } = 1.0f;
        public float A { get; set; } = 1.0f;

        public static Color Lerp(Color a, Color b, float t)
        {
            return new Color()
            {
                R = a.R + (b.R - a.R) * t,
                G = a.G + (b.G - a.G) * t,
                B = a.B + (b.B - a.B) * t,
                A = a.A + (b.A - a.A) * t
            };
        }

        public override string ToString()
        {
            return String.Format("Color: {0}, {1}, {2}, {3}", R, G, B, A);
        }
    }
}
