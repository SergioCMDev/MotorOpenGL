#include "Mesh.h"
#include<glad/glad.h>
#include<shader.h>
#include<iostream>
#include<cstdint>
#include<stdio.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture>	textures) 
	: vertices_(vertices), indices_(indices), textures_(textures) {
	setupMesh();
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO_);
	//Generamos 2 buffer, elementos y objetos
	glGenBuffers(1, &VBO_);
	glGenBuffers(1, &EBO_);

	//Bindeamos el VAO
	glBindVertexArray(VAO_);

	//Bindeamos buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	//Subida de vertices al buffer
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t), &indices_[0], GL_STATIC_DRAW);

	//vertices del triangulo 6 por que hay 6 elementos hasta el proximo inicio de linea
	uint32_t atributteNumber = 0;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	//Vertex texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextCoords));

	//Vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	//Vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

void Mesh::Draw(const Shader& shader) const {
	//bind textures
	uint32_t diffuseNr = 1;
	uint32_t specularNr = 1;
	uint32_t normalNr = 1;
	uint32_t heightNr = 1;
	for (uint8_t i = 0; i < textures_.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		string number;
		string name = textures_[i].type;
		if (name == "texture_diffuse")
		{
			number = to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = to_string(specularNr++);

		}
		else if (name == "texture_normal")
		{
			number = to_string(normalNr++);

		}
		else if (name == "texture_height")
		{
			number = to_string(heightNr++);
		}
		shader.Set((name + number).c_str(), i);
		//Bind the texture
		glBindTexture(GL_TEXTURE_2D, textures_[i].id);
	}

	//draw mesh
	glBindVertexArray(VAO_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Set to defaults configurations
	glActiveTexture(GL_TEXTURE0);
}