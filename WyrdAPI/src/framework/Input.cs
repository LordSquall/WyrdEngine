using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{ 
    public enum MouseButton
    {
        LEFT,
        RIGHT
    }

    public class Input
    {
        public static Vector2 MousePos { get; set; } = new Vector2();

        public static bool[] MouseButtons { get; set; } = new bool[2];

        public static bool GetMouseState(MouseButton button)
        {
            return MouseButtons[(int)button];
        }
    }
}
