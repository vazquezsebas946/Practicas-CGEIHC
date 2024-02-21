//Práctica 2 semestre 2024-2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;//Listas de tipo FIFO, para tener mas un de shader, mesh y meshcolor. Esta linea nos ayuda a crear nuevos objetos sin copiar tanto codigo
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
//siempre va en este orden la operacion projection*model*vec4(pos,1.0f);
//el degradado es interpolacion de colores, cosa que hace la funcion clamp de shader.vert
static const char* fShader = "shaders/shader.frag";
//Solo manda como salida el dato que mando shader.vert
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* fShaderAzul = "shaders/shaderazul.frag";
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* fShaderVerde = "shaders/shaderverde.frag";
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* fShaderRojo = "shaders/shaderrojo.frag";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* fShaderCafe = "shaders/shadercafe.frag";
static const char* vShaderVerdeOsc = "shaders/shaderverdeoscuro.vert";
static const char* fShaderVerdeOsc = "shaders/shaderverdeoscuro.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, 0.0f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Nueva funcion, similar a la anterior, solo para poder encimarlas y crear una piramide cuadrangular
void CreaPiramideinversa()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, 0.0f,	//2
		0.0f,-0.5f, +0.5f,	//3

	};
	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36); // 6 caras x 6 vertices para cada uno
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_S[] = {
		//X			Y			Z			R		G		B
		-0.8f,      -0.5f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.8f,      -0.3f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      -0.5f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.8f,      -0.3f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      -0.5f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      -0.3f,     0.0f,		1.0f,	0.0f,	0.0f,

		-0.5f,      -0.5f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      +0.1f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.65f,     -0.5f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      +0.1f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.65f,     -0.5f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.65f,     +0.1f,     0.0f,		1.0f,	0.0f,	0.0f,

		-0.8f,      -0.1f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.8f,      +0.1f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      -0.1f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.8f,      +0.1f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      -0.1f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      +0.1f,     0.0f,		1.0f,	0.0f,	0.0f,

		-0.8f,      -0.1f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.8f,      +0.3f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.65f,     -0.1f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.8f,      +0.3f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.65f,     -0.1f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.65f,     +0.3f,     0.0f,		1.0f,	0.0f,	0.0f,

		-0.8f,      +0.5f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.8f,      +0.3f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      +0.5f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.8f,      +0.3f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      +0.5f,     0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,      +0.3f,     0.0f,		1.0f,	0.0f,	0.0f,

	};
	MeshColor* letraS = new MeshColor();
	letraS->CreateMeshColor(vertices_S, 180);
	meshColorList.push_back(letraS);

	GLfloat vertices_V[] = {
		//X			Y			Z			R		G		B
		-0.2f,     -0.5f,      0.0f,		0.0f,	1.0f,	0.0f,
		-0.2f,     -0.3f,      0.0f,		0.0f,	1.0f,	0.0f,
		+0.1f,     -0.5f,      0.0f,		0.0f,	1.0f,	0.0f,
		-0.2f,     -0.3f,      0.0f,		0.0f,	1.0f,	0.0f,
		+0.1f,     -0.5f,      0.0f,		0.0f,	1.0f,	0.0f,
		+0.1f,     -0.3f,      0.0f,		0.0f,	1.0f,	0.0f,

		-0.35f,    +0.5f,     0.0f,		    0.0f,	1.0f,	0.0f,
		-0.2f,     -0.5f,     0.0f,		    0.0f,	1.0f,	0.0f,
		-0.05f,    -0.3f,     0.0f,		    0.0f,	1.0f,	0.0f,
		-0.35f,    +0.5f,     0.0f,		    0.0f,	1.0f,	0.0f,
		-0.05f,    -0.3f,     0.0f,		    0.0f,	1.0f,	0.0f,
		-0.25f,    +0.5f,     0.0f,		    0.0f,	1.0f,	0.0f,

		+0.25f,    +0.5f,     0.0f,		    0.0f,	1.0f,	0.0f,
		+0.1f,     -0.5f,     0.0f,		    0.0f,	1.0f,	0.0f,
		-0.05f,    -0.3f,     0.0f,		    0.0f,	1.0f,	0.0f,
		+0.25f,    +0.5f,     0.0f,		    0.0f,	1.0f,	0.0f,
		-0.05f,    -0.3f,     0.0f,		    0.0f,	1.0f,	0.0f,
		+0.15f,    +0.5f,     0.0f,		    0.0f,	1.0f,	0.0f,


	};
	MeshColor* letraV = new MeshColor();
	letraV->CreateMeshColor(vertices_V, 108);
	meshColorList.push_back(letraV);

	GLfloat vertices_R[] = {
		//X			Y			Z			R		G		B
		+0.8f,     +0.3f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.8f,     +0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.5f,     +0.3f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.8f,     +0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.5f,     +0.3f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.5f,     +0.5f,      0.0f,		0.0f,	0.0f,	1.0f,

		+0.4f,     +0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.4f,     -0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.55f,    -0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.4f,     +0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.55f,    -0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.55f,    +0.5f,      0.0f,		0.0f,	0.0f,	1.0f,

		+0.8f,     +0.1f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.8f,     -0.1f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.5f,     +0.1f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.8f,     -0.1f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.5f,     +0.1f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.5f,     -0.1f,      0.0f,		0.0f,	0.0f,	1.0f,

		+0.8f,     +0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.8f,     -0.1f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.65f,    -0.1f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.8f,     +0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.65f,    -0.1f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.65f,    +0.5f,      0.0f,		0.0f,	0.0f,	1.0f,

		+0.8f,     -0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.65f,    -0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.5f,     -0.1f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.8f,     -0.5f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.65f,    -0.1f,      0.0f,		0.0f,	0.0f,	1.0f,
		+0.5f,     -0.1f,      0.0f,		0.0f,	0.0f,	1.0f,


	};
	MeshColor* letraR = new MeshColor();
	letraR->CreateMeshColor(vertices_R, 180);
	meshColorList.push_back(letraR);

}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide. Shader arcoiris
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader(); //shader verde
	shader2->CreateFromFiles(vShaderVerde, fShaderVerde);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader(); //shader azul
	shader3->CreateFromFiles(vShaderAzul, fShaderAzul);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader(); //shader rojo
	shader4->CreateFromFiles(vShaderRojo, fShaderRojo);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader(); //shader verde oscuro
	shader5->CreateFromFiles(vShaderCafe, fShaderCafe);
	shaderList.push_back(*shader5);

	Shader* shader6 = new Shader(); //shader cafe
	shader6->CreateFromFiles(vShaderVerdeOsc, fShaderVerdeOsc);
	shaderList.push_back(*shader6);

	Shader* shader7 = new Shader();//shader para usar color como parte del VAO: letras 
	shader7->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader7);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CreaPiramideinversa();//índice 1 en MeshList
	CrearCubo();//índice 2 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		//| la rayita es para que pueda recibir mas de 1 parametro

		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//S
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.3f, +2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//V
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(+0.0f, +2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[1]->RenderMeshColor();

		//R
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(+0.3f, +2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();

	    angulo += 0.1f;
		//CASITA
		// CUBO ROJO
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(+0.0f, -0.6f, -4.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		// PIRAMIDE AZUL
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(+0.0f, +0.9f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 1.0f, 5.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(+0.0f, +0.9f, -4.0f));
		model = glm::scale(model, glm::vec3(3.5f, 1.0f, 5.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//CUBOS VERDES
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(+0.0f, -1.2f, -3.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(+0.55f, -0.1f, -3.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.55f, -0.1f, -3.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//PIRAMIDES VERDES OSCURO
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, -0.5f, -3.8f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, -0.5f, -3.8f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); //Las piramides con indice 1 estan pegadas a las de indice 0, formando una piramide cuadrangular, y que el arbol no se vea partido a la mitad

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(+2.0f, -0.5f, -3.8f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(+2.0f, -0.5f, -3.8f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//CUBOS CAFES
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.1f, -1.2f, -4.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(+2.1f, -1.2f, -4.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/