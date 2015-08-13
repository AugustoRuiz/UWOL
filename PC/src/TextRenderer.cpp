#include "TextRenderer.h"

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::DrawString(int x, int y, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot)
{
	float tx1, tx2, ty1, ty2;
    int xPos;
    int charPos;
	char currentChar;

	if (this->texture == NULL) {
		TextureMgr *texMgr = TextureMgr::GetInstance();
		this->texture = texMgr->LoadTexture("data/font.png");
	}

	glEnable(GL_TEXTURE_2D);
	//Load the texture
	glBindTexture(GL_TEXTURE_2D, this->texture->texture);
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    if(aliasing)
    {
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    }

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	charPos = 0;

	xPos = x;

	while(text[charPos] != 0)
	{
		currentChar = text[charPos++];

		updateTexCoords(currentChar, &tx1, &tx2, &ty1, &ty2);

		//Top-left vertex (corner)
		glColor4f(rTop, gTop, bTop, 1.0f);	
		glTexCoord2f(tx1, ty1);
		glVertex2i(xPos, y);

		//Top-right vertex (corner)
		glColor4f(rTop, gTop, bTop, 1.0f);	
		glTexCoord2f(tx2, ty1);
		glVertex2i(xPos + 16, y);

		//Bottom-right vertex (corner)
		glColor4f(rBot, gBot, bBot, 1.0f);	
		glTexCoord2f(tx2, ty2);
		glVertex2i(xPos + 16, y + 16);

		//Bottom-left vertex (corner)
		glColor4f(rBot, gBot, bBot, 1.0f);	
		glTexCoord2f(tx1, ty2);
		glVertex2i(xPos, y + 16);

		xPos += 16;
	}

	glEnd();
}

void TextRenderer::updateTexCoords(char currentChar, float *tx1, float *tx2, float *ty1, float *ty2)
{
	const float factor = 0.0625f;
	char diff;

	if(currentChar >= 'a' && currentChar <= 'p')
	{
        diff = currentChar - 'a';
		*tx1 = diff * factor;
		*tx2 = (diff + 1) * factor;
		*ty1 = 0;
		*ty2 = factor;
	}
	else
	{
		if(currentChar >= 'q' && currentChar <= 'z')
		{
			diff = currentChar - 'q';
			*tx1 = diff * factor;
			*tx2 = (diff + 1) * factor;
			*ty1 = factor;
			*ty2 = 2 * factor;
		}
		else
		{
			if(currentChar >= 'A' && currentChar <= 'P')
			{
				diff = currentChar - 'A';
				*tx1 = diff * factor;
				*tx2 = (diff + 1) * factor;
				*ty1 = 2 * factor;
				*ty2 = 3 * factor;
			}
			else
			{
				if(currentChar >= 'Q' && currentChar <= 'Z')
				{
					diff = currentChar - 'Q';
					*tx1 = diff * factor;
					*tx2 = (diff + 1) * factor;
					*ty1 = 3 * factor;
					*ty2 = 4 * factor;
				}
				else
				{
					if(currentChar >= '0' && currentChar <= '9')
					{
						diff = currentChar - '0';
						*tx1 = diff * factor;
						*tx2 = (diff + 1) * factor;
						*ty1 = 4 * factor;
						*ty2 = 5 * factor;
					}
					else
					{
						*ty1 = 5 * factor;
						*ty2 = 6 * factor;
						switch(currentChar)
						{
							case '.':
								*tx1 = 0;
								*tx2 = factor;
								break;
							case ':':
								*tx1 = factor;
								*tx2 = 2 * factor;
								break;
							case ',':
								*tx1 = 2 * factor;
								*tx2 = 3 * factor;
								break;
							case ';':
								*tx1 = 3 * factor;
								*tx2 = 4 * factor;
								break;
							case '(':
								*tx1 = 4 * factor;
								*tx2 = 5 * factor;
								break;
							case '*':
								*tx1 = 6 * factor;
								*tx2 = 7 * factor;
								break;
							case '!':
								*tx1 = 7 * factor;
								*tx2 = 8 * factor;
								break;
							case '?':
								*tx1 = 8 * factor;
								*tx2 = 9 * factor;
								break;
							case '\'':
								*tx1 = 9 * factor;
								*tx2 = 10 * factor;
								break;
							case ')':
								*tx1 = 10 * factor;
								*tx2 = 11 * factor;
								break;
							default:
								*tx1 = 11 * factor;
								*tx2 = 12 * factor;
								break;
						}
					}
				}
			}
		}
	}
}