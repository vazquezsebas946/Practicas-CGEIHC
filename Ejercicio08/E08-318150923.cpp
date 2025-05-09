/*
Pr�ctica 7: Iluminaci�n 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

/*solo hay 7 fuentes de luz restantes, una es la direccional.
Hay varuas firnas de crear color: por VAO, por texturizado, por uniform o por material, esta es la componente difusa de una luz (KD)
La componente ambiental (KA) es una tonalidad de color que afecta a todos los objetos por igual
	En aplicaciones avanzadas, la KA se divide en KA e IA, IA siendo la intensidad y KA la deficiencia del material
	un material opaco refleja menos la luz que otro 
La componente especular (KS) es el valor que define cuanto brilla y que tanto refleja la luz un objeto
El sombreado phong se agrega a algo mas para ver los brillos de una luz mediante la camara y el angulo
La normal de un vertice es el promedio de las normales de las superficies a las que esta conectado

*/
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture10;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model lampara;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
PointLight pointlights2[MAX_POINT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		-1.0f, -1.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void crearDado10caras() {
	unsigned int caras10_indices[] = {
		// ARRIBA, Cara 1
		0, 1, 2,
		2, 3, 0,
		//ARRIBA, cara 2
		4, 5, 6,
		6, 7, 4,
		//ARRIBA, cara 3
		8, 9, 10,
		10, 11, 8,
		//ARRIBA, cara 4
		12, 13, 14,
		14, 15, 12,
		//ARRIBA, cara 5
		16, 17, 18,
		18, 19, 16,
		//ABAJO, cara 6 
		20, 21, 22,
		22, 23, 20,
		//ABAJO, cara 7
		24, 25, 26,
		26, 27, 24,
		//ABAJO, cara 8
		28, 29, 30,
		30, 31, 28,
		//ABAJO, cara9
		32, 33, 34,
		34, 35, 32,
		//ABAJO, cara 10
		36, 37, 38,
		38, 39, 36
	};

	GLfloat caras10_vertices[] = {
		// CARA 1
		//x		y		z		S		    T			NX		NY		NZ
		-0.5f, +0.0f,  0.5f,	0.0566f,    0.2969f,	1.0f,	-1.0f,	-1.0f,	//0
		0.0f, -0.25f,   0.7f,	0.0215f,	0.1856f,	1.0f,	-1.0f,	-1.0f,	//1
		0.5f, +0.0f,   0.5f,	0.1504f,	0.0996f,	-1.0f,	-1.0f,	-1.0f,	//2
		0.0f,  0.75f,  -0.25f,	0.4277f,	0.2539f,	1.0f,	-1.0f,	-1.0f,	//3
		// CARA 2
		//x		y		z		S		    T
		0.5f, +0.0f,   0.5f,    0.3145f,    0.8906f,	-1.0f,	-1.0f,	-1.0f,
		1.f,  -0.25f,  0.1f,	0.1875f,    0.8047f,	-1.0f,	-1.0f,	-1.0f,
		0.9f, +0.0f,   -0.5f,	0.2266f,	0.6895f,	-1.0f,	-1.0f,	1.0f,
		0.0f,  0.75f,  -0.25f,	0.5957f,	0.7324f,	-1.0f,	-1.0f,	-1.0f,

		//CARA 3
		-0.5f, +0.0f,  0.5f,	0.4629f,	0.0234f,	1.0f,	-1.0f,	-1.0f,
		-1.f,  -0.25f, 0.1f,    0.2793f,	0.0117f,	1.0f,	-1.0f,	-1.0f,
		-0.9f, +0.0f,  -0.5f,	0.1504f,	0.0996f,	1.0f,	-1.0f,	-1.0f,
		0.0f,  0.75f,  -0.25f,	0.4277f,	0.2539f,	1.0f,	-1.0f,	-1.0f,

		// CARA 4
		//x		y		z		S			T
		0.9f, +0.0f,   -0.5f,	0.9141f,	0.8535f,	-1.0f,	-1.0f,	1.0f,
		0.6f, -0.35f,  -1.15f,	0.8184f,	0.9551f,	-1.0f,	-1.0f,	1.0f,
		0.0f,  0.0f,  -1.25f,	0.6328f,	0.9648f,	1.0f,	-1.0f,	1.0f,
		0.0f,  0.75f,  -0.25f,	0.5957f,	0.7324f,	-1.0f,	-1.0f,	1.0f,

		// CARA 5
		//x		y		z		S			T
		-0.9f, +0.0f,   -0.5f,	0.2598f,	0.4648f,	1.0f,	-1.0f,	-1.0f,
		-0.6f, -0.35f,  -1.15f,	0.4297f,	0.5195f,	1.0f,	-1.0f,	1.0f,
		0.0f,  0.0f,  -1.25f,	0.5957f,	0.4668f,	1.0f,	-1.0f,	1.0f,
		0.0f,  0.75f,  -0.25f,	0.4277f,	0.2539f,	1.0f,	-1.0f,	1.0f,

		//CARA 6
		//x		y		z		S			T
		0.0f, -0.25f,   0.7f,   0.3164f,	0.8906f,	-1.0f,	1.0f,	-1.0f,
		0.5f, +0.0f,    0.5f,	0.4473f,	0.9785f,	-1.0f,	1.0f,	-1.0f,
		1.f,  -0.25f,   0.1f,	0.6309f,	0.9629f,	-1.0f,	1.0f,	-1.0f,
		0.0f,  -1.25f,  -0.25f,	0.5957f,	0.7324f,	-1.0f,	1.0f,	-1.0f,

		//CARA 7
		0.0f, -0.25f,   0.7f,   0.2598f,	0.4648f,	-1.0f,	1.0f,	-1.0f,
		-0.5f, +0.0f,   0.5f,	0.0918f,    0.4141f,	1.0f,	1.0f,	-1.0f,
		-1.f,  -0.25f,   0.1f,	0.0625f,	0.3027f,	1.0f,	1.0f,	-1.0f,
		0.0f,  -1.25f,  -0.25f,	0.4277f,	0.2539f,	1.0f,	1.0f,	-1.0f,

		//CARA 8
		1.f,  -0.25f,  0.1f,    0.2266f,	0.6895f,	-1.0f,	1.0f,	-1.0f,
		0.9f, +0.0f,   -0.5f,	0.2617f,	0.5762f,	-1.0f,	1.0f,	1.0f,
		0.6f, -0.35f,  -1.15f,	0.4297f,	0.5195f,	-1.0f,	1.0f,	1.0f,
		0.0f,  -1.25f,  -0.25f,	0.5957f,	0.7324f,	-1.0f,	1.0f,	1.0f,

		//CARA 9
		-1.f,  -0.25f,  0.1f,   0.4668f,	0.0273f,	1.0f,	1.0f,	0.0f,
		-0.9f, +0.0f,   -0.5f,	0.6523f,	0.0371f,	1.0f,	1.0f,	0.0f,
		-0.6f, -0.35f,  -1.15f,	0.75f,		0.1328f,	1.0f,	1.0f,	0.0f,
		0.0f,  -1.25f,  -0.25f,	0.4277f,	0.2539f,	1.0f,	1.0f,	0.0f,

		//CARA 10
		0.6f, -0.35f,  -1.15f,  0.7598f,	0.5234f,	-1.0f,	1.0f,	1.0f,
		0.0f,  0.0f,  -1.25f,	0.5957f,	0.4668f,	-1.0f,	1.0f,	1.0f,
		-0.6f, -0.35f,  -1.15f,	0.4297f,	0.5195f,	1.0f,	1.0f,	1.0f,
		0.0f,  -1.25f,  -0.25f,	0.5957f,	0.7324f,	-1.0f,	1.0f,	1.0f,

	};

	Mesh* caras10 = new Mesh();
	caras10->CreateMesh(caras10_vertices, caras10_indices, 320, 60);
	meshList.push_back(caras10);
}

int main()
{
	float x;
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	crearDado10caras();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture10 = Texture("Textures/dado10caras.tga");
	dadoTexture10.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	//Cargamos el modelo de la lampara
	lampara = Model();
	lampara.LoadModel("Models/street_lamp_02.obj");	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,//AMBIENTAL (QUE TAN INTENSA ES LA EMISION DE LUZ) Y DIFUSA (QUE TAN FUERTE SE VE EL COLOR DEL SOL)
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.0f);
	pointLightCount++;

	//Luz de la lampara
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,//rojo, verde y azul
		2.0f, 2.0f,//ambiental (Para luces point, aumenta el rango de la iluminacion, pero el alcance total de la luz es constante) y difuso (intensidad del color, y mas alcance hasta llegar al valor maximo)
		-15.0f, 4.4f, 0.0f,//pocision
		0.0f, 0.7f, 0.5f);//valores de la ecuacion cuadratica (modifican el rango maximo de iluminacion)
	pointLightCount++;

	pointlights2[0] = pointLights[1];
	pointlights2[1] = pointLights[0];

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);//que tan amplia es la zona de iluminacion, sirve para acotar la luz
	spotLightCount++;
	
	//Luz del helicoptero
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f, //rojo, verde y azul
		1.0f, 2.0f, //ambiental y difuso
		0.0f, 5.5f, 6.0f,//pocision
		0.0f, -5.0f, 0.0f,//direccion
		1.0f, 0.0f, 0.0f, //valores de la ecuacion cuadratica
		15.0f);//Angulo de apertura del cono del spotlight
	spotLightCount++;
	//se crean mas luces puntuales y spotlight 



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	x = -8.5f;
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);

		if(mainWindow.getarticulacion6() > 0.0) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);//para apagar una luz, la quitas del arreglo de luces en el shader
		}
		
		if (mainWindow.getarticulacion5() > 0.0) {
				shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}
		

		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 luzheli = glm::vec3(-mainWindow.getarticulacion2(), 5.5f, 6.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//------------*INICIA DIBUJO DE FIGURAS-------------------*
		//lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		spotLights[2].SetPos(luzheli);

		//Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0, mainWindow.getarticulacion2()*3.35, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//DADO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture10.UseTexture();
		meshList[4]->RenderMesh();

		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?: Siempre se deben dibujar al final los blending
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture(); //Si la textura tiene traslucides o transparencia se usan las 2 lineas anteriores
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND); //Para desactivarlo se usa esta linea

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
//Una luz no se apaga colocando el color en negro, ni colocando sus componentes difusa y ambiental en cero, ni cambiando su pocision al infinito
//Ni colocando los valores de la ecuacion cuadratica o su direccion en cero 