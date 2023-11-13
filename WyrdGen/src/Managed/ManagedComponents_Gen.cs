using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using WyrdGen.Extensions;

namespace WyrdGen
{
    public class ManagedComponents_Gen
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

        private String TemplateFile { get; } = "Components.cs";

        public ManagedComponents_Gen() { }

        public void Generate(ComponentDefintions definitions, string outputDir)
        {
            // We want a seperate file for each component
            foreach (Component component in definitions.Components)
            {
                // Read the template file
                String output = File.ReadAllText(@"templates\" + TemplateFile + ".template");

                output = output.ReplaceTag("COMPONENT_NAME", component.Name);


                // Variables
                {
                    StringBuilder content = new StringBuilder();

                    foreach (Data data in component.Data)
                    {
                        if (data.HeapOnly == false)
                        {
                            ManagedType typeMap = TypeMappings[data.Type].Managed;

                            String expandedMarshalType = ExpandMarshalType(typeMap.MarshalType, data);

                            content.AppendFormat($"      [MarshalAs(UnmanagedType.{expandedMarshalType})]"); // , SizeConst = 1024)
                            content.AppendLine();
                            if (String.IsNullOrEmpty(typeMap.WrapperType))
                            {
                                content.AppendFormat($"      private {typeMap.Type} _{data.Name.ToLower()};");
                            }
                            else
                            {
                                content.AppendFormat($"      private {typeMap.WrapperType} _{data.Name.ToLower()};");
                            }
                            content.AppendLine();
                            content.AppendLine();
                        }
                    }

                    output = output.ReplaceTag("VARIABLES", content.ToString());
                }

                // Properties
                {
                    StringBuilder content = new StringBuilder();

                    foreach (Data data in component.Data)
                    {
                        if (data.HeapOnly == false)
                        {
                            ManagedType typeMap = TypeMappings[data.Type].Managed;
                            if (String.IsNullOrEmpty(typeMap.WrapperType))
                            {
                                content.AppendLine($"      public {typeMap.Type} {data.Name.Substring(0, 1).ToUpper()}{data.Name.Substring(1).ToLower()}");
                                content.AppendLine("      {");
                                content.AppendLine("         get { return _" + data.Name.ToLower() + "; }");
                                content.AppendLine("         set ");
                                content.AppendLine("         {");
                                content.AppendLine("             _" + data.Name.ToLower() + " = value;");
                                content.AppendLine("             " + component.Name + "_Set" + data.Name.Substring(0, 1).ToUpper() + data.Name.Substring(1).ToLower() + "(Scene.NativePtr, EntityID, _" + data.Name.ToLower() + ");");
                                content.AppendLine("         }");
                                content.AppendLine("      }");
                                content.AppendLine();
                            }
                            else
                            {
                                content.AppendLine($"      public {typeMap.WrapperType} {data.Name.Substring(0, 1).ToUpper()}{data.Name.Substring(1).ToLower()}");
                                content.AppendLine("      {");
                                content.AppendLine("         get { return _" + data.Name.ToLower() + "; }");
                                content.AppendLine("         set ");
                                content.AppendLine("         {");
                                content.AppendLine("             _" + data.Name.ToLower() + " = value;");
                                content.AppendLine("             " + component.Name + "_Set" + data.Name.Substring(0, 1).ToUpper() + data.Name.Substring(1).ToLower() + "(Scene.NativePtr, EntityID, _" + data.Name.ToLower() + ".NativePtr);");
                                content.AppendLine("         }");
                                content.AppendLine("      }");
                                content.AppendLine();
                            }
                        }
                    }

                    output = output.ReplaceTag("PROPERTIES", content.ToString());
                }

                // Property Bindings
                {
                    //StringBuilder content = new StringBuilder();
                    //
                    //foreach (Data data in component.Data)
                    //{
                    //    ManagedType typeMap = TypeMappings[data.Type].Managed;
                    //    if (!typeMap.IsPrimitive)
                    //    {
                    //        String deletateFunc = "{ " + component.Name + "_Set" + data.Name.Substring(0, 1).ToUpper() + data.Name.Substring(1).ToLower() + "(Scene.NativePtr, EntityID, _" + data.Name.ToLower() + "); }";
                    //        content.AppendLine($"\t\t\t_{data.Name.ToLower()}.PropertyChanged += (obj, property) => {deletateFunc};");
                    //    }
                    //}
                    //
                    //output = output.ReplaceTag("PROPERTY_BINDINGS", content.ToString());
                    output = output.ReplaceTag("PROPERTY_BINDINGS", "");
                }

                // P/Invoke function
                {
                    StringBuilder content = new StringBuilder();

                    foreach (Data data in component.Data)
                    {
                        if (data.HeapOnly == false)
                        {
                            ManagedType typeMap = TypeMappings[data.Type].Managed;

                            if (String.IsNullOrEmpty(typeMap.WrapperType))
                            {
                                content.AppendFormat($"         [DllImport(\"WyrdCAPI\")]");
                                content.AppendLine();
                                content.AppendFormat($"         public static extern IntPtr {component.Name}_Set{data.Name.Substring(0, 1).ToUpper()}{data.Name.Substring(1).ToLower()}(IntPtr scenePtr, UInt64 entity, {typeMap.Type} {data.Name.ToLower()});");
                                content.AppendLine();
                            }
                            else
                            {
                                content.AppendFormat($"         [DllImport(\"WyrdCAPI\")]");
                                content.AppendLine();
                                content.AppendFormat($"         public static extern IntPtr {component.Name}_Set{data.Name.Substring(0, 1).ToUpper()}{data.Name.Substring(1).ToLower()}(IntPtr scenePtr, UInt64 entity, IntPtr {data.Name.ToLower()});");
                                content.AppendLine();
                            }
                        }
                    }

                    output = output.ReplaceTag("PINVOKE_FUNCTIONS", content.ToString());
                }

                
                File.WriteAllText(Path.Combine(outputDir, component.Name + ".cs"), output);
            }

        }

        private String ExpandMarshalType(string type, Data data)
        {
            while (type.Contains('{'))
            {
                int s = type.IndexOf('{');
                int e = type.Substring(s).IndexOf('}');

                string name = type.Substring(s+1, e-1);

                PropertyInfo[] properties = data.GetType().GetProperties();

                foreach (var prop in properties)
                {
                    string formattedName = name.Substring(0, 1).ToUpper() + name.Substring(1).ToLower();

                    if (prop.Name == formattedName)
                    {
                        String val = prop.GetValue(data).ToString();

                        type = type.Replace("{" + name + "}", val);
                    }
                }
            }

            return type;
        }
    }
}
