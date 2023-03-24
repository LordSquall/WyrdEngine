using System;
using System.Collections.Generic;
using System.Linq;

namespace WyrdAPI
{
    public class ResourceManagerProxy : NativeObject
    {
        public class ByteArrayComparer : IEqualityComparer<byte[]>
        {
            public bool Equals(byte[] left, byte[] right)
            {
                if (left == null || right == null)
                {
                    return left == right;
                }
                return left.SequenceEqual(right);
            }
            public int GetHashCode(byte[] key)
            {
                if (key == null)
                    throw new ArgumentNullException("key");
                return key.Sum(b => b);
            }
        }

        public static Dictionary<Byte[], Texture> _CachedTextures = new Dictionary<Byte[], Texture>(new ByteArrayComparer());

        public static void CacheTexture(ManagedTextureDesc textureDesc)
        {

               //if (!_CachedTextures.ContainsKey(textureDesc.GUID))
                //{
                    Console.WriteLine($"Caching Texture: Width={textureDesc.Width}, Height={textureDesc.Height}");
                //    _CachedTextures.Add(textureDesc.GUID, new Texture() { Height = textureDesc.Height, Width = textureDesc.Width });

                //}
        }

        #region P/Invoke functions
        // TODO
        #endregion
    }
}
