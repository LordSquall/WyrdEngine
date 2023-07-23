using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdGen
{
    public class ComponentSerialiserFactory_Gen
    {
        private TypeMappingDefinitions _TypeMappings;

        public TypeMappingDefinitions TypeMappings
        {
            get
            {
                return _TypeMappings;
            }
            set
            {
                _TypeMappings = value;
            }
        }

        private String OutputFile_Header { get; } = "ComponentSerialiserFactory.h";
        private String OutputFile_Source { get; } = "ComponentSerialiserFactory.cpp";

        public class TypeDef
        {
            public String Type { get; }
            public String DefaultValue { get; }
            public bool IsArray { get; }

            public TypeDef(String type, String defaultValue, bool isArray = false)
            {
                Type = type;
                DefaultValue = defaultValue;
                IsArray = isArray;
            }
        }


        public ComponentSerialiserFactory_Gen()
        {
        }

        public void Generate(ComponentDefintions definitions, string outputDir)
        {
            GenerateHeader(definitions, outputDir);
            GenerateSource(definitions, outputDir);
        }

        private void GenerateHeader(ComponentDefintions definitions, string outputDir)
        {
            // Read the template file
            String output = File.ReadAllText(@"templates\" + OutputFile_Header + ".template");


            // Json Serialise Content
            {
                // Build the content string 
                StringBuilder content = new StringBuilder();

                foreach (Component component in definitions.Components)
                {
                    content.AppendLine("            else if (strcmp(name.c_str(), \"" + component.ShortName + "\") == 0)");
                    content.AppendLine("            {");
                    content.AppendLine("                Serialise(obj, (" + component.Name + "*)data);");
                    content.AppendLine("            }");
                }

                // Replace the content tag
                output = output.Replace("<!CONTENT_JSON_SERIALISE>", content.ToString());
            }

            // Stream Serialise Content
            {
                // Build the content string 
                StringBuilder content = new StringBuilder();

                foreach (Component component in definitions.Components)
                {
                    content.AppendLine("            else if (strcmp(name.c_str(), \"" + component.ShortName + "\") == 0)");
                    content.AppendLine("            {");
                    content.AppendLine("                Serialise(stream, (" + component.Name + "*)data);");
                    content.AppendLine("            }");
                }

                // Replace the content tag
                output = output.Replace("<!CONTENT_STREAM_SERIALISE>", content.ToString());
            }

            // Json Deserialise Content
            {
                // Build the content string 
                StringBuilder content = new StringBuilder();

                foreach (Component component in definitions.Components)
                {
                    content.AppendLine("            else if (strcmp(name.c_str(), \"" + component.ShortName + "\") == 0)");
                    content.AppendLine("            {");
                    content.AppendLine("                " + component.Name + "* comp = new (buffer) " + component.Name + "();");
                    content.AppendLine("                Deserialise(obj, comp);");
                    content.AppendLine("            }");
                }

                // Replace the content tag
                output = output.Replace("<!CONTENT_JSON_DESERIALISE>", content.ToString());
            }

            // Stream Deserialise Content
            {
                // Build the content string 
                StringBuilder content = new StringBuilder();

                foreach (Component component in definitions.Components)
                {
                    content.AppendLine("            else if (strcmp(name.c_str(), \"" + component.ShortName + "\") == 0)");
                    content.AppendLine("            {");
                    content.AppendLine("                " + component.Name + "* comp = new (buffer) " + component.Name + "();");
                    content.AppendLine("                Deserialise(stream, comp);");
                    content.AppendLine("            }");
                }

                // Replace the content tag
                output = output.Replace("<!CONTENT_STREAM_DESERIALISE>", content.ToString());
            }


            // Declarations Content
            {
                // Build the content string 
                StringBuilder content = new StringBuilder();

                foreach (Component component in definitions.Components)
                {
                    content.AppendLine();
                    content.AppendLine("        static void Serialise(jsonxx::Object& obj, Wyrd::" + component.Name + "* data);");
                    content.AppendLine("        static void Serialise(std::ofstream& stream, Wyrd::" + component.Name + "* data);");
                    content.AppendLine("        static void Deserialise(jsonxx::Object& obj, Wyrd::" + component.Name + "* data);");
                    content.AppendLine("        static void Deserialise(std::ifstream& stream, Wyrd::" + component.Name + "* data);");
                }

                // Replace the content tag
                output = output.Replace("<!CONTENT_DECLARATIONS>", content.ToString());
            }

            File.WriteAllText(outputDir + OutputFile_Header, output);
        }

        private void GenerateSource(ComponentDefintions definitions, string outputDir)
        {
            // Read the template file
            String output = File.ReadAllText(@"templates\" + OutputFile_Source + ".template");

            // Serialise Content
            {
                // Build the content string 
                StringBuilder content = new StringBuilder();

                foreach (Component component in definitions.Components)
                {
                    content.AppendLine("    void ComponentSerialiserFactory::Serialise(jsonxx::Object & obj, Wyrd::" + component.Name + "* data)");
                    content.AppendLine("    {");
                    foreach (Data data in component.Data)
                    {
                        if (data.HeapOnly && data.CustomSerialisation)
                        {
                            content.AppendLine($"        CustomSerialisation_{component.ShortName}_{data.Name}(obj, data);");
                        }
                        else if (data.HeapOnly == false)
                        {
                            if (data.CustomSerialisation)
                            {
                                content.AppendLine($"        CustomSerialisation_{component.ShortName}_{data.Name}(obj, data);");
                            }
                            else if (data.RuntimeOnly == false)
                            {
                                content.AppendLine("        obj << \"" + data.Name + "\" << data->" + data.Name + ";");
                            }
                        }
                    }

                    if (component.CustomSerialisation)
                    {
                        content.AppendLine($"        CustomSerialisation_{component.Name}(obj, data);");
                    }
                    content.AppendLine("    }");
                    content.AppendLine();

                    content.AppendLine("    void ComponentSerialiserFactory::Serialise(std::ofstream& stream, Wyrd::" + component.Name + "* data)");
                    content.AppendLine("    {");
                    foreach (Data data in component.Data)
                    {
                        UnmanagedType typeMap = _TypeMappings[data.Type].Unmanaged;
                        if (data.HeapOnly == false)
                        {
                            if (typeMap.IsArray)
                            {
                                content.AppendLine($"        stream.write((char*)&data->{data.Name}, sizeof({_TypeMappings[data.Type].Unmanaged.Type}) * {data.Length});");
                            }
                            else
                            {
                                content.AppendLine($"        stream.write((char*)&data->{data.Name}, sizeof({_TypeMappings[data.Type].Unmanaged.Type}));");
                            }
                        }
                    }
                    if (component.CustomSerialisation)
                    {
                        content.AppendLine($"        CustomSerialisation_{component.Name}(stream, data);");
                    }
                    content.AppendLine("    }");
                    content.AppendLine();
                }

                // Replace the content tag
                output = output.Replace("<!CONTENT_SERIALISE>", content.ToString());
            }

            {
                // Build the content string 
                StringBuilder content = new StringBuilder();

                foreach (Component component in definitions.Components)
                {
                    content.AppendLine("    void ComponentSerialiserFactory::Deserialise(jsonxx::Object & obj, Wyrd::" + component.Name + "* data)");
                    content.AppendLine("    {");
                    foreach (Data data in component.Data)
                    {
                        if (data.HeapOnly && data.CustomSerialisation)
                        {
                            content.AppendLine($"        CustomDeserialisation_{component.ShortName}_{data.Name}(data, obj);");
                        }
                        else if (data.HeapOnly == false)
                        {
                            // retrieve json type mapping
                            JsonType jsonType = _TypeMappings[data.Type].Json;

                            // determing the assignment operation
                            string assignementOperator = jsonType.IsStreamable ? "<<" : "=";

                            // build the default value, if needed
                            string defaultValue = data.Default;
                            if (!jsonType.IsPrimitive)
                            {
                                defaultValue = "Decode" + data.Type + "(\"" + data.Default + "\")";
                            }

                            if (data.CustomSerialisation)
                            {
                                content.AppendLine($"        CustomDeserialisation_{component.ShortName}_{data.Name}(data, obj);");
                            }
                            else if (data.RuntimeOnly == false)
                            {
                                // content.AppendLine("        data->" + data.Name + " " + assignementOperator + " TryGetOrDefault<jsonxx::" + jsonType.Type + ">(obj, \"" + data.Name + "\"" + defaultValue + ");"); ;
                                content.AppendLine("        data->" + data.Name + " " + assignementOperator + " obj.get<jsonxx::" + jsonType.Type + ">(\"" + data.Name + "\", " + defaultValue + ");"); ;
                            }
                        }
                    }

                    if (component.CustomSerialisation)
                    {
                        content.AppendLine($"        CustomDeserialisation_{component.Name}(data, obj);");
                    }

                    content.AppendLine("    }");

                    content.AppendLine("");
                    content.AppendLine("    void ComponentSerialiserFactory::Deserialise(std::ifstream& stream, Wyrd::" + component.Name + "* data)");
                    content.AppendLine("    {");
                    foreach (Data data in component.Data)
                    {

                        UnmanagedType typeMap = _TypeMappings[data.Type].Unmanaged;
                        if (data.HeapOnly == false)
                        {
                            if (typeMap.IsArray)
                            {
                                content.AppendLine($"        stream.read((char*)&data->{data.Name}, sizeof({_TypeMappings[data.Type].Unmanaged.Type}) * {data.Length});");
                            }
                            else
                            {
                                content.AppendLine($"        stream.read((char*)&data->{data.Name}, sizeof({_TypeMappings[data.Type].Unmanaged.Type}));");
                            }
                        }
                    }
                    if (component.CustomSerialisation)
                    {
                        content.AppendLine($"        CustomDeserialisation_{component.Name}(stream, data);");
                    }

                    // Need custom deserialisation?

                    content.AppendLine("    }");
                    content.AppendLine();
                }

                // Replace the content tag
                output = output.Replace("<!CONTENT_DESERIALISE>", content.ToString());
            }

            File.WriteAllText(outputDir + OutputFile_Source, output);
        }

        private String GetResolvedTypeMappingDeclaration(Data type)
        {
            // retrieve type mapping
            UnmanagedType typeMap = _TypeMappings[type.Type].Unmanaged;

            if (!typeMap.IsArray)
            {
                return String.Format("{0} {1}", typeMap.Type, type.Name);
            }
            else
            {
                return String.Format("{0} {1}[{2}]", typeMap.Type, type.Name, type.Length);
            }

            return "ERROR";
        }

        private String GetResolvedTypeMappingConstructor(Data type)
        {
            // retrieve type mapping
            UnmanagedType typeMap = _TypeMappings[type.Type].Unmanaged;

            return String.Format("{0}({1})", type.Name, typeMap.Default);
        }

        private String FormatOptionalCommentBlock(String comment)
        {
            if (!String.IsNullOrWhiteSpace(comment))
            {
                StringBuilder o = new StringBuilder();
                o.AppendLine("    /**");
                o.AppendLine("     * " + comment);
                o.AppendLine("    **/ ");
                return o.ToString();
            }
            return "\n";
        }
    }
}
