using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class SpriteComponent : GameObjectComponent
    {
        private Texture _Texture = new Texture();
        private Vector2 _Size = new Vector2();
        private Vector2 _Position = new Vector2();
        private Color _Color = new Color();

        public Texture Texture
        {
            get
            {
                return _Texture;
            }
            set
            {
                _Texture = value;
                SpriteRenderer_SetTexture(_NativePointer, _Texture.NativePtr);
            }
        }

        public Vector2 Size
        {
            get
            {
                SpriteRenderer_GetSize(_NativePointer, _Size);
                return _Size;
            }
            set
            {
                _Size = value;
                SpriteRenderer_SetSize(_NativePointer, _Size.X, _Size.Y);
            }
        }

        public Vector2 Position
        {
            get
            {
                SpriteRenderer_GetPosition(_NativePointer, _Position);
                return _Position;
            }
            set
            {
                _Position = value;
                SpriteRenderer_SetPosition(_NativePointer, _Position.X, _Position.Y);
            }
        }

        public Color Color
        {
            get
            {
                SpriteRenderer_GetColor(_NativePointer, ref _Color);
                return _Color;
            }
            set
            {
                _Color = value;
                SpriteRenderer_SetColor(_NativePointer, _Color.R, _Color.G, _Color.B, _Color.A);
            }
        }


        public void SetColor(Color color)
        {
            SpriteRenderer_SetColor(_NativePointer, color.R, color.G, color.B, color.A);
        }

        public Color GetColor()
        {
            Color c = new Color();
            SpriteRenderer_GetColor(_NativePointer, ref c);
            return c;
        }

        public void SetSize(float x, float y)
        {
            SpriteRenderer_SetSize(_NativePointer, x, y);
        }

        public void SetPosition(float x, float y)
        {
            SpriteRenderer_SetPosition(_NativePointer, x, y);
        }

        public override IntPtr CreateUnmanagedPtr(IntPtr gameObjectPtr)
        {
            _NativePointer = SpriteRenderer_Create(gameObjectPtr);
            return _NativePointer;
        }

        public override string ToString() => "Sprite Renderer Default";

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_SetColor(IntPtr value, float red, float green, float blue, float alpha);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_GetColor(IntPtr value, ref Color color);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_SetSize(IntPtr value, float x, float y);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_GetSize(IntPtr value, Vector2 size);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_SetPosition(IntPtr value, float x, float y);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_GetPosition(IntPtr value, Vector2 position);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_SetPosition(IntPtr value, Color color);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_GetPosition(IntPtr value, Color color);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_SetTexture(IntPtr value, IntPtr texture);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_Create(IntPtr value);
    }
}
