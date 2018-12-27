#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cstdint>
#include<stdio.h>

#include "Shader.h"
#include "Utils.h"
#include "Window.h"
#include "Buffer.h"
#include "Camera.h"
using namespace std; //TERMINAR

const char* pathProyecto = "../tests/EJ6_4/";
Utils utils;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(-1.2f, 1.0f, 2.0f);
float lastFrame = 0.0f;
bool firstMouse = true;

const uint32_t screen_width = 800, screen_height = 600;
float lastX = (float)screen_width / 2.0f;
float lastY = (float)screen_height / 2.0f;
Window window;
//UtilsHandlers handlers;
const float M_PI = 3.14f;

bool _firstMouse = false;
double _lastX, _lastY, _xoffset, _yoffset;
//uint32_t numeroElementosVerticesCubo = 144;

struct Sphere {
	float* vertices;
	float* normals;
	float* textCoords;
	uint32_t* elementos;
	uint32_t numeroVertices;
	uint32_t sizeElementos;
	uint32_t numeroNormales;
	uint32_t numeroTexturas;
};


#pragma region Cabezeras
void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height);
#pragma endregion

#pragma region Metodos


void OnChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
	//redimension de pantalla 
	//Cambio de clip scene a view scene
	glViewport(0, 0, width, height);
}

void OnMouse(GLFWwindow* window, double xpos, double ypos) {
	if (_firstMouse) {
		_firstMouse = false;
		_lastX = xpos;
		_lastY = ypos;
	}

	_xoffset = xpos - _lastX;
	_yoffset = ypos - _lastY;
	_lastX = xpos;
	_lastY = ypos;
	camera.handleMouseMovement(_xoffset, _yoffset);
}


void OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
	camera.handleMouseScroll(yoffset);
}

void HandlerInput(GLFWwindow* window, const double deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Forward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Backward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Left, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.HandleKeyboard(Camera::Movement::Right, deltaTime);
	}
}

int Inicializacion() {
	if (!glfwInit()) {
		cout << "Error initializing GLFW" << endl;
		glfwTerminate();
		return -1;
	}
	window = Window(screen_width, screen_height);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Error initializing GLAD" << endl;
		return -1;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//cuando la ventana cambie de tamaño
	glfwSetCursorPosCallback(window.GetWindow(), OnMouse);
	glfwSetFramebufferSizeCallback(window.GetWindow(), OnChangeFrameBufferSize);
	glfwSetScrollCallback(window.GetWindow(), OnScroll);
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 1;
};

void Render(uint32_t VAO, const Shader& shaderCube, const Shader& shaderlight, const uint32_t numberOfElements, Camera camera) {
	//Renderizamos la pantalla con un color basandonos en el esquema RGBA(transparencia)
	//Si lo quitamos, no borra nunca la pantalla

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//glClearColor(0.5f, 0.f, 0.5f, 0);
	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

	shaderlight.Use();
	shaderlight.Set("projection", projection);
	shaderlight.Set("view", view);

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	shaderlight.Set("model", model);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);

	shaderCube.Use();
	shaderCube.Set("projection", projection);
	shaderCube.Set("view", view);
	shaderCube.Set("model", glm::mat4(1.0f));
	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	shaderCube.Set("normalMat", normalMat);

	vec3 objectColor{ 1.0f, 0.5f, 0.3f };
	vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	shaderCube.Set("objectColor", objectColor);
	shaderCube.Set("lightColor", lightColor);
	shaderCube.Set("ambientStrenght", 0.000001f);
	shaderCube.Set("lightPos", lightPos);
	shaderCube.Set("viewPos", camera.GetPosition());
	shaderCube.Set("shininess", 320);
	shaderCube.Set("specularStrenght", 0.6f);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0);
}

uint32_t createVertexData(vector<float> vertices, Sphere sphere) {
	unsigned int VAO, VBO, EBO;
	//const float* vertices = sphere.vertices;
	const uint32_t* indices = sphere.elementos;
	const uint32_t n_indices = sphere.sizeElementos;

	glGenVertexArrays(1, &VAO);
	//Generamos 2 buffer, elementos y objetos
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bindeamos el VAO
	glBindVertexArray(VAO);

	//Bindeamos buffer vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Subida de vertices al buffer

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, n_verts * sizeof(float), &vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, n_verts, vertices, GL_STATIC_DRAW);


	//Bindeo buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(float), indices, GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices, indices, GL_STATIC_DRAW);

	//Vertices de posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * (sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);

	////Vertices de normales
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * (sizeof(float)), (void*)(3 * (sizeof(float))));
	glEnableVertexAttribArray(1);

	//Vertices de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * (sizeof(float)), (void*)(6 * (sizeof(float))));
	glEnableVertexAttribArray(2);


	//desbindeamos buffer objetos
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Desbindeo
	glBindVertexArray(0);

	//desbindeamos buffer elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}


void generateVerts(float * verts, float * norms, float * tex, unsigned int * el, const uint32_t slices, const uint32_t stacks, const uint32_t radius) {
	float theta, phi;       // Generate positions and normals
	float thetaFac = (float)((2.0 * M_PI) / slices);
	float phiFac = (float)(M_PI / stacks);
	float nx, ny, nz, s, t;
	uint32_t idx = 0, tIdx = 0;
	for (uint8_t i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (float)i / slices;
		for (uint8_t j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (float)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = radius * nx;
			verts[idx + 1] = radius * ny;
			verts[idx + 2] = radius * nz;
			norms[idx] = nx;
			norms[idx + 1] = ny;
			norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	idx = 0;                      // Generate the element list
	for (uint8_t i = 0; i < slices; i++) {
		uint32_t stackStart = i * (stacks + 1);
		uint32_t nextStackStart = (i + 1) * (stacks + 1);
		for (uint8_t j = 0; j < stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}

Sphere createSphere(const float radius) {
	constexpr int slices = 10;
	constexpr int stacks = 10;

	Sphere sphere;

	constexpr uint32_t nVerts = (slices + 1) * (stacks + 1); //121
	constexpr uint32_t elements = (slices * 2 * (stacks - 1)) * 3; //540

	float v[3 * nVerts];        // Verts      
	float n[3 * nVerts];        // Normals    
	float tex[2 * nVerts];      // Tex coords    
	uint32_t el[elements];      // Elements

	generateVerts(v, n, tex, el, slices, stacks, radius);
	sphere.vertices = v;
	sphere.textCoords = tex;
	sphere.elementos = el;
	sphere.normals = n;
	sphere.numeroVertices = nVerts * 3;
	sphere.numeroNormales = nVerts * 3;
	sphere.numeroTexturas = 2 * nVerts;
	sphere.sizeElementos = elements;

	return sphere;
}

int main(int argc, char* argv[]) {
	if (!Inicializacion()) {
		return -1;
	}


	string vertexpathStr = utils.GetFinalPath(pathProyecto, "Shaders/vertex.vs");
	const char* vertexpath = vertexpathStr.c_str();

	string fragmentPathString = utils.GetFinalPath(pathProyecto, "Shaders/fragment.fs");
	const char* fragmentPath1 = fragmentPathString.c_str();

	string vertexpathLightString = utils.GetFinalPath(pathProyecto, "Shaders/vertexLight.vs");
	const char* vertexpathLight = vertexpathLightString.c_str();

	string fragmentPathLightString = utils.GetFinalPath(pathProyecto, "Shaders/fragmentLight.fs");
	const char* fragmentPathLight = fragmentPathLightString.c_str();


	Shader shader = Shader(vertexpath, fragmentPath1);
	Shader shaderlight = Shader(vertexpathLight, fragmentPathLight);

	Sphere sphere = createSphere(1);
	const int size = sphere.numeroVertices + sphere.numeroNormales + sphere.numeroTexturas;

	//for (uint32_t i = 0; i < sphere.numeroVertices; ++i) {
	//	newVertice[i] = sphere.vertices[i];
	//}

	//for (uint32_t i = 0; i < sphere.numeroNormales; i++) {
	//	newVertice[sphere.numeroVertices + i] = sphere.normals[i];
	//}

	//for (uint32_t i = 0; i < sphere.numeroTexturas; i++) {
	//	newVertice[sphere.numeroVertices + sphere.numeroNormales + i] = sphere.textCoords[i];
	//}

	vector<float> newVertice2(size);
	int i = 0;
	int iVertices = 0;
	int iNormales = 0;
	int iTexturas = 0;
	int numeroVector = 1;
	int numeroVuelta = 0;


	while (i < sphere.numeroVertices) {
		while (i < 3 * numeroVector) {
			float newPosVertice = sphere.vertices[iVertices];
			int pos = i + (5 * numeroVuelta);
			newVertice2[pos] = newPosVertice;
			i++;
			iVertices++;
		}
		numeroVuelta++;
		numeroVector++;
	};

	i = 0;
	numeroVector = 1;
	numeroVuelta = 0;
	while (i < sphere.numeroNormales) {
		while (i < 3 * numeroVector) {
			float newPosVertice = sphere.normals[iNormales];
			int pos = 3 + i + (5 * numeroVuelta);
			newVertice2[pos] = newPosVertice;
			i++;
			iNormales++;
		}
		numeroVuelta++;
		numeroVector++;
	};

	i = 0;
	numeroVector = 1;
	numeroVuelta = 0;
	while (i < sphere.numeroTexturas) {
		while (i < 2 * numeroVector) {
			float newPosVertice = sphere.textCoords[iTexturas];
			int pos = 6 + numeroVuelta + i + (5 * numeroVuelta);
			newVertice2[pos] = newPosVertice;
			i++;
			iTexturas++;
		}
		numeroVuelta++;
		numeroVector++;
	};

	uint32_t VAO = createVertexData(newVertice2, sphere);


	//Bucle inicial donde se realiza toda la accion del motor
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		HandlerInput(window.GetWindow(), deltaTime);
		window.HandlerInput();

		Render(VAO, shader, shaderlight, sphere.sizeElementos, camera);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
#pragma endregion

