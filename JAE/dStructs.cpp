#include "dStructs.h"

dStructs::point::point()
{
}

dStructs::point::point(GLfloat aX, GLfloat aY)
{
	x = aX;
	y = aY;
}

dStructs::entity::entity()
{

}

dStructs::entity::entity(GLfloat posX, GLfloat posY, GLfloat sizeX, GLfloat sizeY)
{
	pos = point(posX,posY);
	size = point(sizeX, sizeY);
}

void dStructs::copyPoint(point pointToCopy, point& toPoint)
{
	toPoint.x = pointToCopy.x;
	toPoint.y = pointToCopy.y;
}

void dStructs::copyEntity(entity entityToCopy, entity& toEntity)
{
	//copy the pos point
	copyPoint(entityToCopy.pos, toEntity.pos);
	//same for the size point
	copyPoint(entityToCopy.size, toEntity.size);
}