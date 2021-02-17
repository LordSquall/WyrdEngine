using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Vector2
    {
        public float X { get; set; } = 0.0f;
        public float Y { get; set; } = 0.0f;

        public Vector2() { }

        public Vector2(float x, float y)
        {
            this.X = x;
            this.Y = y;
        }

        public static Vector2 operator +(Vector2 v1, Vector2 v2)
        {
            return new Vector2(v1.X + v2.X, v1.Y + v2.Y);
        }

        public static Vector2 operator -(Vector2 v1, Vector2 v2)
        {
            return new Vector2(v1.X - v2.X, v1.Y - v2.Y);
        }

        public static Vector2 operator *(Vector2 v1, float m)
        {
            return new Vector2(v1.X * m, v1.Y * m);
        }

        public static float operator *(Vector2 v1, Vector2 v2)
        {
            return v1.X * v2.X + v1.Y * v2.Y;
        }

        public static Vector2 operator /(Vector2 v1, float m)
        {
            return new Vector2(v1.X / m, v1.Y / m);
        }

        public static Vector2 Normalise(Vector2 vec)
        {
            float distance = (float)Math.Sqrt(vec.X * vec.X + vec.Y * vec.Y);
            return new Vector2(vec.X / distance, vec.Y / distance);
        }
        public static float Distance(Vector2 v1, Vector2 v2)
        {
            return (float)Math.Sqrt(Math.Pow(v1.X - v2.X, 2) + Math.Pow(v1.Y - v2.Y, 2));
        }

        public float Length()
        {
            return (float)Math.Sqrt(X * X + Y * Y);
        }

        public static Vector2 Lerp(Vector2 a, Vector2 b, float t)
        {
            float retX = MathUtils.Lerp(a.X, b.X, t);
            float retY = MathUtils.Lerp(a.Y, b.Y, t);
            return new Vector2(retX, retY);
        }

        public override string ToString()
        {
            return "[" + X + ", " + Y + "]";
        }
    }
}
