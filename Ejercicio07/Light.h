#pragma once

#include <glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, //Componente difusa
			GLfloat aIntensity, GLfloat dIntensity); //Componente Ambiental: intensidad ambiental e intensidad difusa

	~Light();

protected: //Para convertirse en una superclase, y haya herencia
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

