using System;
using System.Runtime.InteropServices;

namespace WyrdAPI
{
    public partial class MaterialComponent : Component
    {
        public void SetProperty(string name, int value)
        {
            MaterialComponent_SetProperty_Int(Scene.NativePtr, EntityID, name, value);
        }

        public void SetProperty(string name, UInt32 value)
        {
            MaterialComponent_SetProperty_UInt32(Scene.NativePtr, EntityID, name, value);
        }

        public void SetProperty(string name, Int64 value)
        {
            MaterialComponent_SetProperty_Int64(Scene.NativePtr, EntityID, name, value);
        }

        public void SetProperty(string name, UInt64 value)
        {
            MaterialComponent_SetProperty_UInt64(Scene.NativePtr, EntityID, name, value);
        }

        public void SetProperty(string name, float value)
        {
            MaterialComponent_SetProperty_Float(Scene.NativePtr, EntityID, name, value);
        }

        public void SetProperty(string name, double value)
        {
            MaterialComponent_SetProperty_Double(Scene.NativePtr, EntityID, name, value);
        }

        public void SetProperty(string name, Color value)
        {
            MaterialComponent_SetProperty_Color(Scene.NativePtr, EntityID, name, value);
        }

        public void SetProperty(string name, Texture value)
        {
            MaterialComponent_SetProperty_Texture(Scene.NativePtr, EntityID, name, value.NativePtr);
        }

        #region P/Invoke functions

        [DllImport("WyrdCAPI")]
        public static extern IntPtr MaterialComponent_SetProperty_Int(IntPtr scenePtr, UInt64 entity, String name, int value);
        [DllImport("WyrdCAPI")]
        public static extern IntPtr MaterialComponent_SetProperty_UInt32(IntPtr scenePtr, UInt64 entity, String name, UInt32 value);
        [DllImport("WyrdCAPI")]
        public static extern IntPtr MaterialComponent_SetProperty_Int64(IntPtr scenePtr, UInt64 entity, String name, Int64 value);
        [DllImport("WyrdCAPI")]
        public static extern IntPtr MaterialComponent_SetProperty_UInt64(IntPtr scenePtr, UInt64 entity, String name, UInt64 value);
        [DllImport("WyrdCAPI")]
        public static extern IntPtr MaterialComponent_SetProperty_Float(IntPtr scenePtr, UInt64 entity, String name, float value);
        [DllImport("WyrdCAPI")]
        public static extern IntPtr MaterialComponent_SetProperty_Double(IntPtr scenePtr, UInt64 entity, String name, double value);
        [DllImport("WyrdCAPI")]
        public static extern IntPtr MaterialComponent_SetProperty_Color(IntPtr scenePtr, UInt64 entity, String name, Color color);
        [DllImport("WyrdCAPI")]
        public static extern IntPtr MaterialComponent_SetProperty_Texture(IntPtr scenePtr, UInt64 entity, String name, IntPtr texturePtr);
        #endregion
    }
}
