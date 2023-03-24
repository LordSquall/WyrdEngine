using System;
using System.Runtime;
using System.Xml.Serialization;

namespace WyrdGen
{
    public class UnmanagedType
    {
        [XmlAttribute("type")]
        public String Type;

        [XmlAttribute("default")]
        public String Default;

        [XmlAttribute("isArray")]
        public bool IsArray;

        [XmlAttribute("generateMaxLengthDefines")]
        public bool GenerateMaxLengthDefines;

        [XmlAttribute("isPrimitive")]
        public bool IsPrimitive = true;
    }

    public class ManagedType
    {
        [XmlAttribute("type")]
        public String Type;

        [XmlAttribute("marshalType")]
        public String MarshalType;

        [XmlAttribute("default")]
        public String Default;

        [XmlAttribute("isArray")]
        public bool IsArray;

        [XmlAttribute("generateMaxLengthDefines")]
        public bool GenerateMaxLengthDefines;

        [XmlAttribute("wrapperType")]
        public String WrapperType;
    }

    public class JsonType
    {
        [XmlAttribute("type")]
        public String Type;

        [XmlAttribute("isStreamable")]
        public bool IsStreamable;

        [XmlAttribute("isPrimitive")]
        public bool IsPrimitive;
    }


    public class TypeMap
    {
        [XmlAttribute("name")]
        public String Name;

        [XmlAttribute("heapOnly")]
        public bool HeapOnly;

        [XmlElement("UnmanagedType")]
        public UnmanagedType Unmanaged;

        [XmlElement("ManagedType")]
        public ManagedType Managed;

        [XmlElement("JsonType")]
        public JsonType Json;
    }

    [XmlRoot(ElementName = "TypeMappings")]
    public class TypeMappingDefinitions
    {
        [XmlElement("TypeMap")]
        public TypeMap[] TypeMaps;

        public TypeMap this[string key]
        {
            get { return Array.Find(TypeMaps, p => p.Name == key); }
        }
    }
}
