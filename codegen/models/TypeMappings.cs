using System;
using System.Collections.Generic;

namespace CodeGenModels
{
    public class TypeMappings
    {
        static public Dictionary<String, String> Types = new Dictionary<string, string>()
        {
            { "Int8", "int8_t" },
            { "Int16", "int16_t" },
            { "Int32", "int32_t" },
            { "Int64", "int64_t" },
            { "UInt8", "uint8_t" },
            { "UInt16", "uint16_t" },
            { "UInt32", "uint32_t" },
            { "UInt64", "uint64_t" },
            { "Float", "float" },
            { "String", "std::string" },
            { "UID", "Osiris::UID" },
            { "Byte", "char" },
        };
    }
}
