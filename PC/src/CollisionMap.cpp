#include "CollisionMap.h"

CollisionMap::CollisionMap(void)
{
	map = NULL;
	cols = 0;
	rows = 0;
	cellWidth = 0;
	cellHeight = 0;
	_g = Graphics::GetInstance();
}

CollisionMap::~CollisionMap(void)
{
}

CollisionType CollisionMap::Collides(RECTANGLEF *srcRect, VECTOR2F *vel)
{
	CollisionType colType = COLLISION_TYPE_NONE;

	int tileX;
    int tileX2;
    int tileY;
    int tileY2;
    int tileValue;

    srcRect->x += (int) vel->x;

    tileY = (int)((srcRect->y) / this->cellHeight);
    tileY2 = (int)((srcRect->y + srcRect->height) / cellHeight);

    if (vel->x > 0) 
	{
       // Comprobamos a la derecha del personaje.
       tileX = (int)((srcRect->x + srcRect->width) / cellWidth); 
	   for(int tY = tileY; tY <= tileY2; tY++)
	   {
          tileValue = this->map[tY * this->cols + tileX];
          if(tileValue == COLLISION_BLOCK)
		  {
             srcRect->x = (tileX * cellWidth) - (srcRect->width + 1);
             colType |= COLLISION_TYPE_RIGHT;
			 break;
		  }
	   }
	}
    else if(vel->x < 0)
	{
       // Comprobamos a la izquierda del personaje.
       tileX = (int)((srcRect->x) / cellWidth); 
	   for(int tY = tileY; tY <= tileY2; tY++)
	   {
          tileValue = this->map[tY * this->cols + tileX];
          if(tileValue == COLLISION_BLOCK)
		  {
             srcRect->x = (float) (tileX + 1) * cellWidth;
             colType |= COLLISION_TYPE_LEFT;
			 break;
		  }
	   }
	}

    srcRect->y += (int) vel->y;

    tileX = (int)((srcRect->x) / cellWidth) ;
    tileX2 = (int)((srcRect->x + srcRect->width) / cellWidth) ;

    if (vel->y < 0) 
	{
       // Comprobamos encima.
       tileY = (int)(srcRect->y / cellHeight);
	   for(int tX = tileX; tX <= tileX2; tX++)
	   {
          tileValue = this->map[tileY * this->cols + tX];
          if (tileValue == COLLISION_BLOCK)
		  {
             srcRect->y = (float) (tileY + 1) * cellHeight;
             colType |= COLLISION_TYPE_UP; 
			 break;
		  }
	   }
	}
	else if (vel->y > 0)
	{
       // Comprobamos debajo.
       tileY = (int)((srcRect->y + srcRect->height) / cellHeight);
	   for(int tX = tileX; tX <= tileX2; tX++)
	   {
		  tileValue = this->map[tileY * this->cols + tX];

		  if (tileValue == COLLISION_BLOCK)
		  {
             srcRect->y = (tileY * cellHeight) - (srcRect->height + 1);
             colType |= COLLISION_TYPE_DOWN;
			 break;
		  }
       
          if (tileValue == COLLISION_PLATFORM)
		  {
             if (((int)(srcRect->y + srcRect->height) % cellHeight) <= vel->y)
			 {
                srcRect->y = (tileY * cellHeight) - (srcRect->height + 1);
                colType |= COLLISION_TYPE_DOWN;
				break;
			 }
		  }
	   }
	}

	return colType;
}

void CollisionMap::Initialize(int nCols, int nRows, int cellW, int cellH)
{
	int count;

	this->cols = nCols;
	this->rows = nRows;
	this->cellHeight = cellH;
	this->cellWidth = cellW;

	if(this->map != NULL)
	{
		free(this->map);
	}

	count = this->cols * this->rows;

	this->map = (CollisionType *) malloc(count * sizeof(CollisionType));

	for(int i = 0; i < count; i++)
	{
		this->map[i] = COLLISION_NONE;
	}
	
}

void CollisionMap::DebugPaint()
{
	CollisionType colState;
	vector<VECTOR2> vertexes;
	int idx = 0;
	
	for(int iY = 0; iY < this->rows; iY++)
	{
		for(int iX = 0; iX < this->cols; iX++)
		{
			colState = this->map[idx];
			idx += 1;
			
			int x1 = iX * this->cellWidth, x2 = (iX + 1) * this->cellWidth;
			int y1 = iY * this->cellHeight, y2 = (iY + 1) * this->cellHeight;

			if((colState & COLLISION_BLOCK) == COLLISION_BLOCK)
			{
				vertexes.push_back(VECTOR2(x1, y1));
				vertexes.push_back(VECTOR2(x2, y1));
				vertexes.push_back(VECTOR2(x1, y2));
				vertexes.push_back(VECTOR2(x2, y2));
				vertexes.push_back(VECTOR2(x1, y1));
				vertexes.push_back(VECTOR2(x1, y2));
				vertexes.push_back(VECTOR2(x2, y1));
				vertexes.push_back(VECTOR2(x2, y2));
			}
			if((colState & COLLISION_PLATFORM) == COLLISION_PLATFORM)
			{
				vertexes.push_back(VECTOR2(x1, y1));
				vertexes.push_back(VECTOR2(x2, y1));
			}
			_g->DrawPolyLines(vertexes, 1.0, 1.0, 1.0, 1.0);
		}
	}
}

