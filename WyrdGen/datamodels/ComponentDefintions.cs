using System;

using System.Xml.Serialization;

namespace WyrdGen
{
    public class Data
    {
        [XmlAttribute("name")]
        public String Name { get; set; }

        [XmlAttribute("type")]
        public String Type { get; set; }

        [XmlAttribute("length")]
        public int Length { get; set; }

        [XmlAttribute("heapOnly")]
        public bool HeapOnly { get; set; }

        [XmlAttribute("default")]
        public String Default { get; set; }
    }

    public class Component
    {
        [XmlAttribute("name")]
        public String Name { get; set; }

        [XmlAttribute("shortName")]
        public String ShortName { get; set; }

        [XmlAttribute("customSerialisation")]
        public bool CustomSerialisation { get; set; } = false;

        [XmlAttribute("comment")]
        public String Comment { get; set; }

        [XmlElement("Data")]
        public Data[] Data { get; set; }
    }

    [XmlRoot(ElementName = "ComponentDefinitions")]
    public class ComponentDefintions
    {
        [XmlElement("Component")]
        public Component[] Components { get; set; }
    }
}
