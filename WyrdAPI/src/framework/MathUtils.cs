using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    public class MathUtils
    {
        public static double ConvertToRadians(double angle)
        {
            return (Math.PI / 180) * angle;
        }

        public static float ConvertToRadians(float angle)
        {
            return ((float)Math.PI / 180.0f) * angle;
        }

        public static double ConvertToDegrees(double angle)
        {
            return angle * (180.0 / Math.PI);
        }

        public static float ConvertToDegrees(float angle)
        {
            return angle * (180.0f / (float)Math.PI);
        }

        public static float Lerp(float a, float b, float t)
        {
            return a * (1 - t) + b * t;
        }

        public static float Clamp(float a, float min, float max)
        {
            if (a < min) a = min;
            else if (a > max) a = max;
            return a;
        }

        public static float PingPong(float a, float max)
        {
            return max - Math.Abs(Clamp(a - (float)Math.Floor((double)(a / max)) * max, 0.0f, max) - max);
        }
    }
}
