#pragma once

#include <GL/glut.h>


class dStructs 
{


public:
	struct point
	{
		GLfloat x, y;

		point();
		point(GLfloat aX,GLfloat aY); //constructor for point
	};

	struct entity
	{
		point pos, size;

		entity();
		entity(GLfloat posX, GLfloat posY, GLfloat sizeX, GLfloat sizeY);
	
	};

static void copyPoint(point pointToCopy, point& toPoint);
static void copyEntity(entity entityToCopy, entity& toEntity);

};


