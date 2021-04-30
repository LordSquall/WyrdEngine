﻿using System.Collections.Generic;
using System.Xml.Serialization;

namespace CodeGenModels
{
    [XmlRoot("PlayerAPI")]
    public class PlayerAPI
    {
        [XmlElement("File")]
        public List<FileModel> Files { get; set; }
    }

    public class FileModel
    {
        [XmlAttribute("name")]
        public string Name { get; set; }

        [XmlElement("Block")]
        public List<Block> Blocks { get; set; }
    }

    public class Block
    {
        [XmlAttribute("name")]
        public string Name { get; set; }

        [XmlElement("Item")]
        public List<Item> Items { get; set; }

        [XmlElement("ItemList")]
        public List<ItemList> ItemLists { get; set; }
		
        [XmlElement("ConditionalItemList")]
        public List<ConditionalItemList> ConditionalItemList { get; set; }
    }

    public class Item
    {
        [XmlAttribute("name")]
        public string Name { get; set; }

        [XmlAttribute("type")]
        public string Type { get; set; }
    }

    public class ItemList
    {
        [XmlAttribute("name")]
        public string Name { get; set; }

        [XmlAttribute("type")]
        public string Type { get; set; }
    }
	
	public class ConditionalItem
    {
		[XmlAttribute("value")]
        public string Value { get; set; }
		
        [XmlAttribute("name")]
        public string Name { get; set; }

        [XmlAttribute("type")]
        public string Type { get; set; }
    }

    public class ConditionalItemList
    {
        [XmlAttribute("control")]
        public string Control { get; set; }

        [XmlAttribute("type")]
        public string Type { get; set; }

        [XmlAttribute("name")]
        public string Name { get; set; }

        [XmlElement("ConditionalItem")]
        public List<ConditionalItem> ConditionalItems { get; set; }
    }
}