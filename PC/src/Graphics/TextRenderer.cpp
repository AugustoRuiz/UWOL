#include "TextRenderer.h"

TextRenderer::TextRenderer()
{
	this->_texture = NULL;
	this->_program = NULL;
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::DrawStringAlpha(int x, int y, int textSize, const string &text, float rTop, float gTop, float bTop, float rBot, float gBot, float bBot, float alpha)
{
	GLFuncs* g = GLFuncs::GetInstance();
    int xPos;
    int charPos;
	char currentChar;

	charPos = 0;
	xPos = x;

	if (this->_texture == NULL) {
		this->_texture = TextureMgr::GetInstance()->LoadTexture("data/font.png");
	}

	if (this->_program == NULL) {
		vector<string> vertexShaders = { "data/shaders/Default.vertex" };
		vector<string> fragmentShaders = { "data/shaders/TexturedColored.fragment" };
		this->_program = new Program(vertexShaders, fragmentShaders);
	}

	Program* program = this->_program;
	if (program->Textures.size() > 0) {
		program->Textures[0] = this->_texture;
	}
	else {
		program->Textures.push_back(this->_texture);
	}

	program->Use();
	program->BindTextures();

	glUniformMatrix4fv(glGetUniformLocation(program->ProgramId, "MVP"), 1, GL_FALSE, &(g->MVP[0][0]));

	GLfloat color_buffer_data[] = {
		rTop, gTop, bTop, alpha,
		rTop, gTop, bTop, alpha,
		rBot, gBot, bBot, alpha,
		rBot, gBot, bBot, alpha
	};

	while(text[charPos] != 0)
	{
		float tx1, tx2, ty1, ty2;

		currentChar = text[charPos++];

		updateTexCoords(currentChar, &tx1, &ty1, &tx2, &ty2);

		GLfloat vertex_buffer_data[] = {
			(float)xPos, (float)y, 0.0f,
			(float)xPos + textSize, (float)y, 0.0f,
			(float)xPos + textSize, (float)y + textSize, 0.0f,
			(float)xPos, (float)y + textSize, 0.0f
		};

		GLfloat uv_buffer_data[] = {
			tx1, ty1,
			tx2, ty1,
			tx2, ty2,
			tx1, ty2
		};

		g->BlitVerts(vertex_buffer_data, sizeof(vertex_buffer_data), uv_buffer_data, sizeof(uv_buffer_data), color_buffer_data, sizeof(color_buffer_data));
		
		xPos += textSize;
	}
}

void TextRenderer::updateTexCoords(char currentChar, float* tx1, float* ty1, float* tx2, float* ty2)
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
							case '&':
								*tx1 = 11 * factor;
								*tx2 = 12 * factor;
								break;
							case '-':
								*tx1 = 12 * factor;
								*tx2 = 13 * factor;
								break;
							case '@':
								*tx1 = 13 * factor;
								*tx2 = 14 * factor;
								break;
							default:
								*tx1 = 15 * factor;
								*tx2 = 16 * factor;
								break;
						}
					}
				}
			}
		}
	}
}