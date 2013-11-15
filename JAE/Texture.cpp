#include "Texture.h"


//static int texFactoryID = 1;//the texture ID we assign each specific texture. Factory style.

Texture::Texture(int width, int height, string filename)
{
	//texID = texFactoryID; //give this texture its ID
	////printf("\nTexture given ID of %i\n", texID);
	//texFactoryID++; //increment the factory ID counter for the next texture
	
	textures=0;

	const char* fnPtr = filename.c_str(); //our image loader accepts a ptr to a char, not a string
	//printf(fnPtr);
	w = width; //give our texture a width and height, the reason that we need to pass in the width and height values manually
	h = height;//UPDATE, these MUST be P.O.T.

	unsigned error = lodepng::decode(image,w,h,fnPtr);//lodepng's decode function will load the pixel data into image vector
	//display any errors with the texture
	if(error)
	{
		cout << "\ndecoder error " << error << ": " << lodepng_error_text(error) <<endl;
	}

	//loop through and //printf our pixel data
	/*for(GLuint i = 0; i<image.size(); i+=4)
	{
		//printf("\n%i,%i,%i,%i,", image.at(i),image.at(i+1),image.at(i+2),image.at(i+3));
	
	}*/

	////printf("\nImage size is %i", image.size());

	//image now contains our pixeldata. All ready for OpenGL to do its thing
	
	//let's get this texture up in the video memory
	texGLInit();
	
}

void Texture::texGLInit()
{
	glGenTextures(1, &textures);
	////printf("\ntextureID = %u", textures);
	glBindTexture(GL_TEXTURE_2D, textures);//evrything we're about to do is about this texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glDisable(GL_COLOR_MATERIAL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,w,h,0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	//we COULD free the image vectors memory right about now.
	image.clear();
	
	

}

void Texture::draw(point centerPoint, point dimensions)
{
	glEnable(GL_TEXTURE_2D);
	//printf("\nDrawing texture at (%f, %f)",centerPoint.x, centerPoint.y);
	glBindTexture(GL_TEXTURE_2D, textures);//bind the texture
	//create a quick vertex array for the primitive we're going to bind the texture to
	////printf("TexID = %u",textures);
	GLfloat vArray[8] = 
	{
		centerPoint.x-(dimensions.x/2), centerPoint.y-(dimensions.y/2),//bottom left i0
		centerPoint.x-(dimensions.x/2), centerPoint.y+(dimensions.y/2),//top left i1
		centerPoint.x+(dimensions.x/2), centerPoint.y+(dimensions.y/2),//top right i2
		centerPoint.x+(dimensions.x/2), centerPoint.y-(dimensions.y/2)//bottom right i3
	};
	
	//create a quick texture array (we COULD create this on the heap rather than creating/destoying every cycle)
	GLfloat tArray[8] = 
	{
		//this has been tinkered with from my normal order. I think LodePNG is bringing the PD upside down. SO A QUICK FIX HERE WAS NECESSARY.
		0.0f,1.0f,//0
		0.0f,0.0f,//1
		1.0f,0.0f,//2
		1.0f,1.0f//3
	};

	//and finally.. the index array...remember, we draw in triangles....(and we'll go CW)
	GLubyte iArray[6] =
	{
		0,1,2,
		0,2,3
	};

	//Activate arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	//Give openGL a pointer to our vArray and tArray
	glVertexPointer(2, GL_FLOAT, 0, &vArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &tArray[0]);
	
	//Draw it all
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &iArray[0]);
	
	//glDrawArrays(GL_TRIANGLES,0,6);
	
	//Disable the vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	//done!

	/*glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
		glVertex2f(centerPoint.x-(dimensions.x/2), centerPoint.y-(dimensions.y/2));
	glTexCoord2f(0.0f,1.0f);
		glVertex2f(centerPoint.x-(dimensions.x/2), centerPoint.y+(dimensions.y/2));
	glTexCoord2f(1.0f,1.0f);
		glVertex2f(centerPoint.x+(dimensions.x/2), centerPoint.y+(dimensions.y/2));
	glTexCoord2f(1.0f,0.0f);
		glVertex2f(centerPoint.x+(dimensions.x/2), centerPoint.y-(dimensions.y/2));
	glEnd();*/
}



Texture::Texture(void)
{
}
Texture::~Texture(void)
{
}
