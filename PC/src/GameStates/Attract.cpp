#include "Attract.h"

Attract::Attract() {
	this->Name = "Attract";
	this->_frameNoise = Frame("data/noise.png");
	this->_frameSombra = Frame("data/TileSombra.png");
	this->_program = Program({ "data/shaders/Default.150.vertex" }, { "data/shaders/CRT-Jasper.fragment" });
	this->_program.Textures.push_back(Graphics::GetInstance()->GetFramebufferTexture());
	this->_program.Textures.push_back(this->_frameNoise.Texture);
	this->_eventsByRoom = vector<vector<Uint32>*>();
}

void Attract::SetRooms(std::vector<Room*> &rooms) {
	this->_rooms = rooms;
}

void Attract::SetPlayer(TPlayer* player) {
	this->_player = player;
}

Program* Attract::GetProgram() {
	return &(this->_program);
}

void Attract::SetAttractData(const vector<Uint32> &eventBuffer)
{
	for (vector<Uint32>* v : this->_eventsByRoom) {
		delete v;
	}
	this->_eventsByRoom.clear();
	vector<Uint32>* currentBuffer = nullptr;
	Uint32 deltaTime;

	for (Uint32 i = 0, li = eventBuffer.size() - 1; i < li; i += 2) {
		Uint32 time = eventBuffer[i];
		Uint32 evt = eventBuffer[i + 1];

		if ((evt & ROOM_START_EVENT) == ROOM_START_EVENT) {
			if (currentBuffer != nullptr) {
				Uint32 size = currentBuffer->size();
				if (size < 2 || currentBuffer->at(size - 2) < 2000) {
					this->_eventsByRoom.pop_back();
					delete currentBuffer;
				}
			}
			currentBuffer = new vector<	Uint32>();
			currentBuffer->push_back(evt & (~ROOM_START_EVENT));
			this->_eventsByRoom.push_back(currentBuffer);
			deltaTime = time;
		}
		else {
			if (currentBuffer != nullptr) {
				currentBuffer->push_back(time - deltaTime);
				currentBuffer->push_back(evt);
			}
		}
	}

	Uint32 size = currentBuffer->size();
	if (size < 2 || currentBuffer->at(size - 2) < 2000) {
		this->_eventsByRoom.pop_back();
		delete currentBuffer;
	}

}

void Attract::OnEnter()
{
	if (this->_eventsByRoom.size() > 0) {
		int attractDataIdx = rand() % this->_eventsByRoom.size();
		this->_currentRoomEvts = *(this->_eventsByRoom[attractDataIdx]);
		Uint32 roomIndex = this->_currentRoomEvts[0];
		this->_evtBufferIterator = this->_currentRoomEvts.begin() + 1;
		this->_totalTicks = 0;
		this->_currentRoom = this->_rooms[roomIndex];
		this->_currentRoom->setPlayer(this->_player);
		this->_currentRoom->OnEnter();
		this->_currentAlpha = 0.0f;
		this->_incrFactor = 1;
	}
}

void Attract::OnExit()
{
}

string Attract::Update(Uint32 milliSec, Event & inputEvent)
{
	string result = this->Name;

	if (inputEvent.Name == "") {
		if (this->_incrFactor != 0) {
			this->_currentAlpha += ((float)this->_incrFactor) *  milliSec * 0.001f;
			if (this->_currentAlpha >= 1.0f) {
				this->_currentAlpha = 1.0f;
				this->_incrFactor = 0;
			}
		}
		else {
			Event roomEvent = inputEvent;
			this->_totalTicks += milliSec;
			if (this->_evtBufferIterator != this->_currentRoomEvts.end()) {
				if (this->_totalTicks >= *this->_evtBufferIterator) {
					Uint32 keyData = *(this->_evtBufferIterator + 1);
					this->_evtBufferIterator += 2;

					roomEvent.Data["key"] = (ActionKeys)(keyData & 0x3FFF);
					roomEvent.Data["fake"] = true;
					roomEvent.Name = ((keyData & 0x4000) == 0x4000) ? "KEY_UP" : "KEY_DOWN";
					Log::Out << "Fake event: " << roomEvent.Name << "(" << roomEvent.Data["key"] << ")" << endl;

					InputManager* input = InputManager::GetInstance();
					input->AddFakeEvent(roomEvent);
					input->Update(0);
				}
				string roomResult = this->_currentRoom->Update(milliSec, roomEvent);
				if (roomResult == "") {
					this->_incrFactor = -1;
				}
			}
			else {
				this->_incrFactor = -1;
			}
		}
	}
	else {
		this->_incrFactor = -1;
	}

	if (this->_currentAlpha < 0.0f && _incrFactor == -1) {
		result = "Presentacion";
	}

	return result;
}

void Attract::Draw(void)
{
	Graphics* g = Graphics::GetInstance();

	this->_currentRoom->Draw();

	// Cortamos por las bravas...
	g->BlitFrameAbs(this->_frameSombra, 0, 0, g->ScreenWidth, (int)g->OffsetY - 1, false, false);
	g->BlitFrameAbs(this->_frameSombra, 0, g->ScreenHeight - (int)g->OffsetY, g->ScreenWidth, (int)g->OffsetY, false, false);
	g->BlitFrameAbs(this->_frameSombra, 0, 0, (int)g->OffsetX, g->ScreenHeight, false, false);
	g->BlitFrameAbs(this->_frameSombra, g->ScreenWidth - (int)g->OffsetX, 0, (int)g->OffsetX, g->ScreenHeight, false, false);

	g->BlitFrameAlpha(this->_frameSombra, 0, 0, g->WorldWidth, g->WorldHeight, 1.0f - this->_currentAlpha, false, false);
}

void Attract::Dispose(void)
{
}
