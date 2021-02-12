﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OsirisAPI
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
    }
}
