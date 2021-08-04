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


        public string Content
        {
            get { return _content; }
            set
            {
                _content = value;
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
    }
}
