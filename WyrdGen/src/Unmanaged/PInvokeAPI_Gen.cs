using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WyrdGen.Extensions;

namespace WyrdGen
{
    public class PInvokeAPI_Gen
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

        private String HeaderTemplateFile { get; } = "PInvokeAPI.h";
        private String SourceTemplateFile { get; } = "PInvokeAPI.cpp";

        public PInvokeAPI_Gen()
        {
        }

        public void Generate(ComponentDefintions definitions, string outputDir)
        {
            // We want a seperate file for each component
            foreach (Component component in definitions.Components)
            {
                // Header file
                {
                    // Read the header template file
                    String output = File.ReadAllText(@"templates\" + HeaderTemplateFile + ".template");

                    output = output.ReplaceTag("COMPONENT_NAME", component.Name);

                    // P/Invoke function
                    {
                        StringBuilder content = new StringBuilder();

                        foreach (Data data in component.Data)
                        {
                            if (data.HeapOnly == false)
                            {
                                UnmanagedType typeMap = TypeMappings[data.Type].Unmanaged;

                                content.AppendLine($"LIBRARY_API void {component.Name}_Set{data.Name.Substring(0, 1).ToUpper()}{data.Name.Substring(1).ToLower()}(void* obj, Wyrd::Entity entity, {typeMap.Type}* position);");
                                content.AppendLine();
                            }
                        }

                        output = output.ReplaceTag("FUNCTION_DECLARATIONS", content.ToString());
                    }

                    File.WriteAllText(Path.Combine(outputDir, "C_" + component.Name + ".h"), output);
                }

                // Source file
                {
                    // Read the source template file
                    String output = File.ReadAllText(@"templates\" + SourceTemplateFile + ".template");

                    output = output.ReplaceTag("COMPONENT_NAME", component.Name);

                    // P/Invoke function
                    {
                        StringBuilder content = new StringBuilder();

                        foreach (Data data in component.Data)
                        {
                            if (data.HeapOnly == false)
                            {
                                UnmanagedType typeMap = TypeMappings[data.Type].Unmanaged;

                                string pointerValue = typeMap.IsPrimitive ? "" : "*";

                                content.AppendLine($"void {component.Name}_Set{data.Name.Substring(0, 1).ToUpper()}{data.Name.Substring(1).ToLower()}(void* scenePtr, Wyrd::Entity entity, {typeMap.Type}{pointerValue} {data.Name})");
                                content.AppendLine("{");
                                content.AppendLine($"   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;");
                                content.AppendLine($"   Wyrd::{component.Name}* component = scene->Get<Wyrd::{component.Name}>(entity);");
                                content.AppendLine();
                                content.AppendLine($"   component->{data.Name} = {pointerValue}{data.Name};");
                                content.AppendLine("}");
                                content.AppendLine();
                            }
                        }

                        output = output.ReplaceTag("FUNCTIONS", content.ToString());
                    }

                    File.WriteAllText(Path.Combine(outputDir, "C_" + component.Name + ".cpp"), output);
                }
            }
        }
    }
}
