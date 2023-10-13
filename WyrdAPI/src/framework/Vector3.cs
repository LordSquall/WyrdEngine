using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class Vector3 : INotifyPropertyChanged
    {
        private float _X = 0.0f;
        public float X 
        { 
            get { 
                return _X; 
            }
            set { 
                _X = value;
                NotifyPropertyChanged("X"); 
            } 
        }

        private float _Y = 0.0f;
        public float Y { get { return _Y; } set { _Y = value; NotifyPropertyChanged("Y"); } }

        private float _Z = 0.0f;
        public float Z { get { return _Z; } set { _Z = value; NotifyPropertyChanged("Z"); } }

        public Vector3() { }

        public Vector3(float x, float y, float z)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
        }

        public static Vector3 operator +(Vector3 v1, Vector3 v2)
        {
            return new Vector3(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);
        }

        public static Vector3 operator -(Vector3 v1, Vector3 v2)
        {
            return new Vector3(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z);
        }

        public static Vector3 operator *(Vector3 v1, float m)
        {
            return new Vector3(v1.X * m, v1.Y * m, v1.Z * m);
        }

        public static float operator *(Vector3 v1, Vector3 v2)
        {
            return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
        }

        public static Vector3 operator /(Vector3 v1, float m)
        {
            return new Vector3(v1.X / m, v1.Y / m, v1.Z / m);
        }

        public static Vector3 Normalise(Vector3 vec)
        {
            float distance = (float)Math.Sqrt(vec.X * vec.X + vec.Y * vec.Y);
            return new Vector3(vec.X / distance, vec.Y / distance, vec.Z / distance);
        }
        public static float Distance(Vector3 v1, Vector3 v2)
        {
            return (float)Math.Sqrt(Math.Pow(v1.X - v2.X, 2) + Math.Pow(v1.Y - v2.Y, 2) + Math.Pow(v1.Z - v2.Z, 2));
        }

        public float Length()
        {
            return (float)Math.Sqrt(X * X + Y * Y + Z * Z);
        }

        public static Vector3 Lerp(Vector3 a, Vector3 b, float t)
        {
            float retX = MathUtils.Lerp(a.X, b.X, t);
            float retY = MathUtils.Lerp(a.Y, b.Y, t);
            float retZ = MathUtils.Lerp(a.Z, b.Z, t);
            return new Vector3(retX, retY, retZ);
        }

        public override string ToString()
        {
            return "[" + X + ", " + Y + ", " + Z + "]";
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void NotifyPropertyChanged(string property)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(property));
            }
        }
    }
}
