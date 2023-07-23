#include "wyrdpch.h"

/* local includes  */
#include "core/UID.h"
#include "core/Resources.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Material.h"
#include "core/renderer/Mesh.h"
#include "serial/TypeSerialisers.h"

/* systems includes */
#include <fstream>
#include <string>
#include <iostream>

namespace Wyrd
{
	void SerialiseTexture(std::ofstream& os, std::shared_ptr<Wyrd::Texture> data)
	{
		/* write common header info */
		writeGuid(os, data->GetUID());
		writeStr(os, data->GetName());

		/* local values for writing */
		uint32_t w = data->GetWidth();
		uint32_t h = data->GetHeight();
		unsigned char* pixelData = data->GetData();

		/* calculate pixel size */
		size_t pixelCount = w * h;

		/* write to stream */
		os.write((char*)&w, sizeof(uint32_t));
		os.write((char*)&h, sizeof(uint32_t));
		os.write((char*)data->GetData(), sizeof(unsigned char) * (pixelCount * 4));
		writeStr(os, data->GetDescription());
	}

	std::shared_ptr<Wyrd::Texture> DeserialiseTexture(std::ifstream& is)
	{
		/* read common header info */
		Wyrd::UID guid = readGuid(is);
		std::string name = readStr(is);

		/* local values */
		uint32_t w;
		uint32_t h;
		std::vector<unsigned char> pixelData;

		/* read values from stream */
		is.read((char*)&w, sizeof(uint32_t));
		is.read((char*)&h, sizeof(uint32_t));
		size_t pixelCount = w * h;
		pixelData.resize(pixelCount * 4);
		is.read((char*)&pixelData[0], sizeof(unsigned char) * (pixelCount * 4));
		std::string description = readStr(is);

		/* create texture */
		TextureDesc textureDesc;
		textureDesc.guid = guid;
		textureDesc.name = name;
		textureDesc.width = w;
		textureDesc.height = h;
		textureDesc.data = &pixelData[0];
		textureDesc.channels = 4;
		textureDesc.description = description;

		return std::shared_ptr<Wyrd::Texture>(Texture::Create(textureDesc));
	}


	void SerialiseMaterial(std::ofstream& os, std::shared_ptr<Wyrd::Material> data) 
	{
		/* write common header info */
		writeGuid(os, data->GetUID());
		writeStr(os, data->GetName());

		writeStr(os, data->GetShader()->GetName());

		const MaterialInputMap& inputMap = data->GetInputPropertyList();
		size_t inputMapSize = inputMap.size();
		os.write((char*)&inputMapSize, sizeof(size_t));

		for (auto& input : inputMap)
		{
			writeStr(os, input.first);
			writeStr(os, input.second.type);
			writeStr(os, input.second.binding);
		}
	}

	std::shared_ptr<Wyrd::Material> DeserialiseMaterial(std::ifstream& is)
	{	
		/* read common header info */
		Wyrd::UID guid = readGuid(is);
		std::string name = readStr(is);

		/* local values */
		std::string shaderName;
		size_t inputMapSize = 0;
		MaterialInputMap inputMap;

		shaderName = readStr(is);

		is.read((char*)&inputMapSize, sizeof(size_t));

		for (size_t i = 0; i < inputMapSize; ++i)
		{
			std::string name = readStr(is);
			std::string type = readStr(is);
			std::string binding = readStr(is);

			inputMap[name] = { type, binding };
		}

		/* create material */
		MaterialDesc materialDesc;
		materialDesc.resource.guid = guid;
		materialDesc.resource.name = name;
		materialDesc.shaderName = shaderName;
		materialDesc.inputMap = inputMap;

		return std::shared_ptr<Wyrd::Material>(Material::Create(materialDesc));
	}

	void SerialiseModel(std::ofstream& os, std::shared_ptr<Wyrd::Mesh> data)
	{
		/* write common header info */
		writeGuid(os, data->GetUID());
		writeStr(os, data->GetName());

		/* local values for writing */
		uint32_t vertexCount = data->Vertices.size();

		/* write to stream */
		os.write((char*)&vertexCount, sizeof(uint32_t));
		os.write((char*)&data->Vertices[0], sizeof(Vertex3D) * vertexCount);
	}

	std::shared_ptr<Wyrd::Mesh> DeserialiseModel(std::ifstream& is)
	{
		/* read common header info */
		Wyrd::UID guid = readGuid(is);
		std::string name = readStr(is);

		/* local values */
		uint32_t vertexCount;
		std::vector<Vertex3D> vertexData;

		/* read values from stream */
		is.read((char*)&vertexCount, sizeof(uint32_t));
		vertexData.resize(vertexCount);
		is.read((char*)&vertexData[0], sizeof(Vertex3D) * vertexCount);

		/* create Mesh */
		MeshDesc meshDesc;
		meshDesc.resource.guid = guid;
		meshDesc.resource.name = name;
		meshDesc.vertices = vertexData;

		return std::shared_ptr<Wyrd::Mesh>(Mesh::Create(meshDesc));
	}

	void SerialiseShader(std::ofstream& os, std::shared_ptr<Wyrd::Shader> data)
	{
		/* write common header info */
		writeGuid(os, data->GetUID());
		writeStr(os, data->GetName());

		writeStr(os, data->GetSource(ShaderStage::Vertex));
		writeStr(os, data->GetSource(ShaderStage::Fragment));
	}

	std::shared_ptr<Wyrd::Shader> DeserialiseShader(std::ifstream& is)
	{
		/* read common header info */
		Wyrd::UID guid = readGuid(is);
		std::string name = readStr(is);

		/* local values */
		std::string vertexSrc = readStr(is);
		std::string fragmentSrc = readStr(is);

		/* create Mesh */
		ShaderDesc shaderDesc;
		shaderDesc.resource.guid = guid;
		shaderDesc.resource.name = name;
		shaderDesc.vertexSrc = vertexSrc;
		shaderDesc.fragmentSrc = fragmentSrc;

		return std::shared_ptr<Wyrd::Shader>(Shader::Create(shaderDesc));
	}
}