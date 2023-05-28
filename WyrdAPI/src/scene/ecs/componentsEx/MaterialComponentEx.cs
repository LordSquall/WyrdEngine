using System;
using System.Runtime.InteropServices;

namespace WyrdAPI
{
    public partial class MaterialComponent : Component
    {
        public void SetProperty(string name, Texture texture)
        {
            MaterialComponent_SetProperty_Texture(Scene.NativePtr, EntityID, name, texture);
        }

        #region P/Invoke functions

        [DllImport("WyrdCAPI")]
        public static extern IntPtr MaterialComponent_SetProperty_Texture(IntPtr scenePtr, UInt64 entity, String name, Texture texture);

        #endregion
    }
}
