using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdGen
{
    public class UnmanagedComponents_Gen
    {
        private TypeMappingDefinitions _TypeMappings;

        public TypeMappingDefinitions TypeMappings {
            get {
                return _TypeMappings;
            }
            set {
                _TypeMappings = value;
            }
        }

        private String OutputFile { get; } = "Components.h";

        public UnmanagedComponents_Gen()
        {
        }

        public void Generate(ComponentDefintions definitions, string outputDir)
        {
            // Read the template file
            String output = File.ReadAllText(@"templates\" + OutputFile + ".template");

            {
                Dictionary<String, int> maxLengthDefineMap = new Dictionary<string, int>();

                // Build the content string 
                StringBuilder content = new StringBuilder();
                
                foreach (Component component in definitions.Components)
                {
                    foreach (Data data in component.Data)
                    {
                        if (data.HeapOnly == false)
                        {
                            UnmanagedType typeMap = TypeMappings[data.Type].Unmanaged;

                            if (typeMap.GenerateMaxLengthDefines)
                            {
                                String name = String.Format("{0}_{1}_LENGTH", component.Name.ToUpper(), data.Name.ToUpper());
                                maxLengthDefineMap.Add(name, data.Length);
                            }
                        }
                    }
                }

                foreach (var typeEntry in maxLengthDefineMap)
                {
                    content.Append("#define " + typeEntry.Key + " " + typeEntry.Value);   
                }

                // Replace the content tag
                output = output.Replace("<!CONTENT_DEFINES>", content.ToString());
            }

            {
                // Build the content string 
                StringBuilder content = new StringBuilder();

                foreach (Component component in definitions.Components)
                {
                    content.AppendLine();
                    content.Append(FormatOptionalCommentBlock(component.Comment));
                    content.AppendFormat("    struct WYRD_LIBRARY_API {0}", component.Name);
                    content.AppendLine();
                    content.AppendLine("    {");


                    // Component Declarations
                    foreach (Data data in component.Data)
                    {
                        String resolvedTypeDeclaration = GetResolvedTypeMappingDeclaration(data);

                        content.AppendFormat("        {0};", resolvedTypeDeclaration);
                        content.AppendLine();
                        
                    }
                    content.AppendLine();

                    // Component Constructor
                    content.AppendFormat("        {0}() : ", component.Name);
                    for (int i = 0; i < component.Data.Length; i++)
                    {
                        Data data = component.Data[i];

                        String resolvedTypeConstructor = GetResolvedTypeMappingConstructor(data);

                        content.AppendFormat("{0}{1} ", resolvedTypeConstructor, i == component.Data.Length - 1 ? " {}" : ",");
                    }
                    content.AppendLine();

                    content.AppendLine();
                    content.AppendLine("        static void ResetFunc(ComponentPool* pool, Entity entity) {");
                    content.AppendFormat("            {0}* c = ({0}*)pool->get(entity);", component.Name);
                    content.AppendLine();
                    content.AppendFormat("            (new (c) {0}());", component.Name);
                    content.AppendLine();
                    content.AppendLine("        }");

                    content.AppendLine("    };");
                }

                // Replace the content tag
                output = output.Replace("<!CONTENT>", content.ToString());
            }

            File.WriteAllText(outputDir + "\\" + OutputFile, output);
        }

        private String GetResolvedTypeMappingDeclaration(Data type)
        {
            // retrieve type mapping
            UnmanagedType typeMap = TypeMappings[type.Type].Unmanaged;

            if (!typeMap.IsArray)
            {
                return String.Format("{0} {1}", typeMap.Type, type.Name);
            }
            else
            {
                return String.Format("{0} {1}[{2}]", typeMap.Type, type.Name, type.Length);
            }
        }

        private String GetResolvedTypeMappingConstructor(Data type)
        {
            // retrieve type mapping
            TypeMap typeMap = TypeMappings[type.Type];
            UnmanagedType unmanagedType = typeMap.Unmanaged;
            return String.Format("{0}({1})", type.Name, !String.IsNullOrWhiteSpace(type.Default) ? type.Default : unmanagedType.Default);
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
