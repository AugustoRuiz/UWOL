#include "Animation/Frame.h"

Frame::Frame()
{
	this->_program = NULL;
}

Frame::Frame(const string &path) : Frame(path, 0, 1, 0, 1) {
};

Frame::Frame(const string &path, float tx1, float tx2, float ty1, float ty2) : Frame() {
	this->Texture = TextureMgr::GetInstance()->LoadTexture(path);
	this->Coords.tx1 = tx1;
	this->Coords.tx2 = tx2;
	this->Coords.ty1 = ty1;
	this->Coords.ty2 = ty2;
	this->_program = NULL;
};

Frame::~Frame()
{
}

Program* Frame::GetProgram() const { return this->_program; }
void Frame::SetProgram(Program* p) { this->_program = p; }

void Frame::Init(const Json::Value& frame)
{
	if (frame != Json::Value::null) {
		string path = frame.get("img", "").asString();
		if (!path.empty()) {
			this->Texture = TextureMgr::GetInstance()->LoadTexture(path);
			Json::Value rectCoords = frame["rect"];
			if (rectCoords != Json::Value::null) {
				this->Coords.tx1 = (rectCoords[0].asFloat() / this->Texture->width);
				this->Coords.ty1 = (rectCoords[1].asFloat() / this->Texture->height);
				this->Coords.tx2 = (rectCoords[2].asFloat() / this->Texture->width);
				this->Coords.ty2 = (rectCoords[3].asFloat() / this->Texture->height);
			}
			else {
				this->Coords.tx1 = 0.0f;
				this->Coords.ty1 = 0.0f;
				this->Coords.tx2 = 1.0f;
				this->Coords.ty2 = 1.0f;
			}
		}
	}
}
