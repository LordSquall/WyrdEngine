using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Threading.Tasks;
using System.IO;
using System.Reflection;

namespace WyrdGen
{
    public class Program
    {

        public void Run()
        {
            ComponentDefintions definitions = Utils.DeserializeToObject<ComponentDefintions>("definitions\\ComponentDefinitions.xml");

            TypeMappingDefinitions typeMappings = Utils.DeserializeToObject<TypeMappingDefinitions>("definitions\\TypeMappingDefinitions.xml");

            // generate unmanaged files
            UnmanagedComponents_Gen unmanagedComponentsGen = new UnmanagedComponents_Gen();
            unmanagedComponentsGen.TypeMappings = typeMappings;
            unmanagedComponentsGen.Generate(definitions, "../../../Wyrd/src/core/ecs/");

            ComponentSerialiserFactory_Gen componentSerialiseFactory = new ComponentSerialiserFactory_Gen();
            componentSerialiseFactory.TypeMappings = typeMappings;
            componentSerialiseFactory.Generate(definitions, "../../../Wyrd/src/serial/");

            PInvokeAPI_Gen pinvokeAPIGen = new PInvokeAPI_Gen();
            pinvokeAPIGen.TypeMappings = typeMappings;
            pinvokeAPIGen.Generate(definitions, "../../../WyrdCAPI/src/ecs/components/");

            // generate managed files
            ManagedComponents_Gen managedComponentsGen = new ManagedComponents_Gen();
            managedComponentsGen.TypeMappings = typeMappings;
            managedComponentsGen.Generate(definitions, "../../../WyrdAPI/src/scene/ecs/components/");
        }

        /// <summary>
        /// Entry Point
        /// </summary>
        public static void Main()
        {
            (new Program()).Run();
        }
    }
}
