using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Matrix4 : NotifableType
    {
        public float X { get; set; } = 0.0f;
        public float Y { get; set; } = 0.0f;
        public float Z { get; set; } = 0.0f;

        public Matrix4() { }

        public Matrix4(float x, float y, float z)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
        }

        public override string ToString()
        {
            return "[" + X + ", " + Y + ", " + Z + "]";
        }
    }
}
