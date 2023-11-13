using System;
using System.Runtime.InteropServices;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Rect : NotifableType
    {
        private Vector2 _position = new Vector2(0.0f, 0.0f);
        private Vector2 _size = new Vector2(0.0f, 0.0f);

        public Vector2 Position 
        { 
            get { return _position; }
            set 
            { 
                _position = value;
                NotifyPropertyChanged("Position");
            }
        }

        public Vector2 Size
        {
            get { return _size; }
            set
            {
                _size = value;
                NotifyPropertyChanged("Size");
            }
        }

        public Rect() { }

        public override string ToString()
        {
            return "[" + _position + ", " + _size + "]";
        }
    }
}
