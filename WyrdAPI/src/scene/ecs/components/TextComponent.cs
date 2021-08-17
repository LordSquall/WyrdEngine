using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class TextComponent : Component
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 1024)]
        private string _content;

        [MarshalAs(UnmanagedType.R8)]
        public float _size;

        [MarshalAs(UnmanagedType.Struct)]
        public Color _color;

        public string Content
        {
            get { return _content; }
            set
            {
                _content = value;
                TextComponent_SetContent(Scene.NativePtr, Entity, _content);
            }
        }

        public float Size
        {
            get { return _size; }
            set
            {
                _size = value;
                TextComponent_SetSize(Scene.NativePtr, Entity, _size);
            }
        }

        public Color Color
        {
            get { return _color; }
            set
            {
                _color = value;
                TextComponent_SetColor(Scene.NativePtr, Entity, _color);
            }
        }

        public ulong Entity { get; set; }

        public void SetEntity(ulong entity)
        {
            Entity = entity;
        }

        public override string ToString()
        {
            return String.Format("[TextComponent] Content: {0}", Content);
        }

        #region P/Invoke functions

        [DllImport("WyrdCAPI")]
        public static extern IntPtr TextComponent_SetContent(IntPtr scenePtr, UInt64 entity, String content);

        [DllImport("WyrdCAPI")]
        public static extern IntPtr TextComponent_SetSize(IntPtr scenePtr, UInt64 entity, float size);

        [DllImport("WyrdCAPI")]
        public static extern IntPtr TextComponent_SetColor(IntPtr scenePtr, UInt64 entity, Color color);

        #endregion
    }
}
