using System;
using System.Runtime.InteropServices;

namespace WyrdAPI
{
    public class SceneManager
    {
        public static IntPtr NativePtr { get; set; }

        public static bool ChangeScene(String sceneName)
        {
            SceneManager_ChangeScene(NativePtr, sceneName);
            return true;
        }


        #region P/Invoke functions

        [DllImport("WyrdCAPI")]

        public static extern IntPtr SceneManager_ChangeScene(IntPtr scenePtr, String sceneName);

        #endregion
    }
}
