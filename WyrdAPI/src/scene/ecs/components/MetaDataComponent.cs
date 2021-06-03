using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential, Size=32, Pack=1)]
    public class MetaDataComponent : Component
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        private string _name;

        public string Name
        {
            get { return _name; }
            set
            {
                _name = value;
            }
        }

        public ulong Entity { get; set; }

        public void SetEntity(ulong entity)
        {
            Entity = entity;
        }

        public override string ToString()
        {
            return String.Format("[MetaDataComponent] Name: {0}", Name);
        }
    }
}
