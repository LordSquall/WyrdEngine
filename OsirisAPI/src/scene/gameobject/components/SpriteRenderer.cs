using System;
using System.Runtime.InteropServices;

namespace OsirisAPI
{
    public class SpriteRenderer : UnmanagedObject
    {
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

        public override string ToString() => "Sprite Renderer Default";

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_SetColor(IntPtr value, float red, float green, float blue, float alpha);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_GetColor(IntPtr value, ref Color color);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_SetSize(IntPtr value, float x, float y);

        [DllImport("OsirisCAPI")]
        public static extern IntPtr SpriteRenderer_SetPosition(IntPtr value, float x, float y);
    }
}
