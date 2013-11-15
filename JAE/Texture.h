#pragma once


#include <iostream>
#include <vector>
#include <GL/glut.h>
#include "lodepng.h"
#include "dStructs.h"

using namespace std;
typedef dStructs::point point;


class Texture
{


public:
	Texture(int width, int height, string filename);//initialises our texture and loads its pixeldata into a buffer
	Texture(void);
	~Texture(void);
	
	void draw(point centerPoint, point dimensions);

	unsigned int w;//width of our imagefile
	unsigned int h;
	
	//GLuint texID;//the ID we will give OGL for this particular texture.
	
private:
	vector<unsigned char> image;
	void texGLInit();
	GLuint textures;
};

