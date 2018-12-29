#include "Model.h"

#include<iostream>
#include<glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

void Model::loadModel(string const path) {
	//read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(),	aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	//check errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "ERROR ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	//retrieve the directory path of the filepath
	directory_ = path.substr(0, path.find_last_of('/'));

	//process ASSIMP's root node recursevily
	processNode(scene->mRootNode, scene);
}

Model::Model(string const &path, bool gamma) : gammaCorrection_(gamma) {
	loadModel(path);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
	//process each mesh located at the current node
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		//the node object only contains indices to index the actual objects in the scene
		//the scene contains all the data, node is justo to keep stuff organized (like realtions between nodes)
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes_.push_back(processMesh(mesh, scene));
	}

	//after we have processed all of the meshes (if any) we then recursively process each of the children nodes
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

static unsigned int TextureFromFile(const char *path, const string &directory, bool gamme) {
	string fileName = string(path);
	fileName = directory + '/' + fileName;
	unsigned int textureID;

	glGenTextures(1, &textureID);

	int widht, height, nrComponents;
	unsigned char * data = stbi_load(fileName.c_str(), &widht, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1) {
			format = GL_RED;
		}
		else if (nrComponents == 3) {
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, widht, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		cout << "Failed to load the texture at path: " << path << endl;
	}
	stbi_image_free(data);
	return textureID;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
	vector<Texture> textures;
	for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		//check if texture was loaded before and if so, continue to next iteration : skip loading a new texture
		bool skip = false;
		for (uint32_t j = 0; j < textures_loaded_.size(); j++)
		{
			if (strcmp(textures_loaded_[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded_[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory_, false);
			texture.type = typeName;
			texture.path = str.C_Str();

			textures.push_back(texture);
			textures_loaded_.push_back(texture);
		}
	}
	return textures;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
	//data to fill
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	vector<Texture> textures;

	//Walk through each of the mesh's vertices
	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; //We declare a placeholder vector since assimp uses its own vector 
		//positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		//normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		//texture coordinates
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TextCoords = vec;

		}
		else {
			vertex.TextCoords = glm::vec2(0.0f, 0.0f);
		}
		//tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		//Bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		vertices.push_back(vertex);

	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		//retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	//proccess materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//we assume a convention for sampler names in the shader. Each diffuse texture should be name as 'texture diffuseN' N = sequential number from 1 to MAX_SAMPLER NUMBER.
	//Same for the rest of types of textures

	//1. diffuse maps
	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//specularMaps
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	//normalMaps
	vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	//heightMaps
	vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

void Model::Draw(const Shader& shader) const {
	for (uint32_t i = 0; i < meshes_.size(); i++)
	{
		meshes_[i].Draw(shader);
	}
}