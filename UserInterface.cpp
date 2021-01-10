//*****************************************************************************************//
//**                                                                                     **//
//**                             UserInterface                                           **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "UserInterface.h"
#include "../Common/DirectInputWrapper/DxInput.h"

Meter::Meter() {
	me = std::make_unique<PolygonData2D[]>(2);
}

void Meter::create(int sizeW, int sizeH, char* headerString) {
	wid = sizeW;
	hei = sizeH;
	int len = (int)strlen(headerString);
	if (len > 100)len = 100;
	mbstowcs(HeaderString, headerString, sizeof(TCHAR) * len);
	HeaderString[len] = '\0';
	me[0].GetVBarray2D(1);
	me[1].GetVBarray2D(1);
	me[0].CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, true, true);
	me[1].CreateBox(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, true, true);
}

bool Meter::checkRange() {
	DxInput* di = DxInput::GetInstance();
	int x = (int)di->PosX();
	int y = (int)di->PosY();
	if (px <= x && x <= px + wid &&
		py <= y && y < py + hei * 0.5f)return true;

	return false;
}

bool Meter::checkMeterRange() {
	DxInput* di = DxInput::GetInstance();
	int x = (int)di->PosX();
	int y = (int)di->PosY();
	if (px + 2.0f <= x && x <= px + wid - 2.0f &&
		py + hei * 0.5f <= y && y <= py + hei - 2.0f)return true;

	return false;
}

float Meter::updatePos(int posX, int posY, float depth, int meterMov) {
	px = posX;
	py = posY;
	int mwidMax = wid - 4;
	meterWid += meterMov;
	if (meterWid < 0)meterWid = 0;
	if (meterWid > mwidMax)meterWid = mwidMax;
	Ratio = (float)meterWid / (float)mwidMax;
	DxText::GetInstance()->
		UpDateText(HeaderString, (float)px + 1.0f, (float)py + 1.0f, (float)hei * 0.3f, { 1.0f, 1.0f, 1.0f, 1.0f });
	DxText::GetInstance()->
		UpDateValue((int)(Ratio * 100.0f), (float)px + 1.0f, (float)py + 1.0f + hei * 0.5f, (float)hei * 0.3f,
			3, { 1.0f, 1.0f, 1.0f, 1.0f });

	me[0].Update((float)px, (float)py, depth + 0.02f,
		0.0f, 0.3f, 1.0f, 1.0f, (float)wid, (float)hei);
	me[1].Update((float)px + 2.0f, (float)py + hei * 0.5f, depth + 0.01f,
		1.0f, 0.0f, 0.0f, 1.0f, (float)meterWid, hei * 0.5f - 2.0f);

	return Ratio;
}

float Meter::updatePosMouse(float depth) {
	DxInput* di = DxInput::GetInstance();
	Mov = 0;
	if (di->isLAction() && checkRange()) {
		ActionSt = true;
		CorrectionX = px - di->PosX();
		CorrectionY = py - di->PosY();
	}
	if (ActionSt && di->isLDown()) {
		px = di->PosX() + CorrectionX;
		py = di->PosY() + CorrectionY;
	}
	else {
		ActionSt = false;
		CorrectionX = 0;
		CorrectionY = 0;
	}

	if (di->isLAction() && checkMeterRange()) {
		ActionStMeter = true;
		CorrectionMeterX = di->PosX();
	}
	if (ActionStMeter && di->isLDown()) {
		Mov = di->PosX() - CorrectionMeterX;
		CorrectionMeterX = di->PosX();
	}
	else {
		ActionStMeter = false;
		CorrectionMeterX = 0;
	}

	return updatePos(px, py, depth, Mov);
}

void Meter::Draw(int com) {
	me[0].SetCommandList(com);
	me[1].SetCommandList(com);
	me[0].Draw();
	me[1].Draw();
}

UserInterface::~UserInterface() {
	ARR_DELETE(me);
}

void UserInterface::setWindowSize(int Width, int Height) {
	windowWidth = Width;
	windowHeight = Height;
}

void UserInterface::setNumMeter(int num) {
	numMeter = num;
	me = new Meter[numMeter];
}

void UserInterface::createMeter(int index, int sizeW, int sizeH, char* headerString) {
	me[index].create(sizeW, sizeH, headerString);
}

float UserInterface::updatePos(int index, int posX, int posY, float depth, int meterMov) {
	return me[index].updatePos(posX, posY, depth, meterMov);
}

float UserInterface::updatePosMouse(int index, float depth) {
	return me[index].updatePosMouse(depth);
}

void UserInterface::Draw(int index, int com) {
	me[index].Draw(com);
}