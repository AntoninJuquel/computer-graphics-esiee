#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <GL/glew.h>
#include <vector>
#include <GLM/fwd.hpp>

class EBO
{
public:
	// ID reference of Elements Buffer Object
	GLuint ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO(std::vector<glm::vec3>& indices);

	// Binds the EBO
	void Bind();
	// Unbinds the EBO
	void Unbind();
	// Deletes the EBO
	void Delete();
};

#endif