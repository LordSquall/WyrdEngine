using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

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

        public void CreateCachedTextureObject(Byte[] uid, IntPtr ptr)
        {
            //var str = BitConverter.ToString(uid); //Convert.ToBase64String(uid);// System.Text.Encoding..GetString(uid);
            Console.WriteLine($"Caching Texture Object {ConvertByteToUID(uid)}");
            _CachedTextures.Add(uid, new Texture() { NativePtr = ptr });
        }

        public Texture RetrieveCachedTextureObject(byte[] data)
        {
            Console.WriteLine("---------- CACHED TEXTURE TABLE -----------");
            Console.WriteLine($"Entries: {_CachedTextures.Count}");
            foreach (var t in _CachedTextures)
            {
                Console.WriteLine(ConvertByteToUID(t.Key) + " --> " + t.Value.NativePtr);
            }
            return _CachedTextures[data];
        }

        private String ConvertByteToUID(byte[] data)
        {
            StringBuilder builder = new StringBuilder();
            {
                ArraySegment<byte> segment = new ArraySegment<byte>(data, 0, 4);
                builder.Append(BitConverter.ToString(segment.ToArray()).Replace("-", string.Empty));
                builder.Append("-");
            }
            {
                ArraySegment<byte> segment = new ArraySegment<byte>(data, 4, 2);
                builder.Append(BitConverter.ToString(segment.ToArray()).Replace("-", string.Empty));
                builder.Append("-");
            }
            {
                ArraySegment<byte> segment = new ArraySegment<byte>(data, 6, 2);
                builder.Append(BitConverter.ToString(segment.ToArray()).Replace("-", string.Empty));
                builder.Append("-");
            }
            {
                ArraySegment<byte> segment = new ArraySegment<byte>(data, 8, 2);
                builder.Append(BitConverter.ToString(segment.ToArray()).Replace("-", string.Empty));
                builder.Append("-");
            }
            {
                ArraySegment<byte> segment = new ArraySegment<byte>(data, 10, 6);
                builder.Append(BitConverter.ToString(segment.ToArray()).Replace("-", string.Empty));
            }

            return builder.ToString();
        }

        #region P/Invoke functions
        // TODO
        #endregion
    }
}
