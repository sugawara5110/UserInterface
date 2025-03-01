//*****************************************************************************************//
//**                                                                                     **//
//**                             UserInterface                                           **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "UserInterface.h"
#include "../Common/DirectInputWrapper/DxInput.h"

namespace {

	VERTEX2 v2[] =
	{
		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} ,{0.0f,1.0f}},
		{ {0.5f, -0.5f, 0.0f},  {0.0f, 0.0f, 0.0f, 0.0f} ,{1.0f,1.0f}},
		{ {0.5f, 0.5f, 0.0f},   {0.0f, 0.0f, 0.0f, 0.0f} ,{1.0f,0.0f}},
		{ {-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f, 0.0f, 0.0f} ,{0.0f,0.0f}}
	};

	UINT index[] =
	{
		0,2,1,
		0,3,2
	};

	void sizeChange(float& posX, float& posY, float& sizeX, float& sizeY) {

		auto sw = Dx_SwapChain::GetInstance();
		auto w = sw->getClientWidth();
		auto h = sw->getClientHeight();
		float x = posX;
		float y = posY;
		float sx = sizeX;
		float sy = sizeY;
		//ç∂ -1, â∫ -1
		float magX = 2.0f / w;
		float magY = 2.0f / h;

		posX = x * magX - 1.0f;
		posY = (y * magY - 1.0f) * -1.0f;
		sizeX = sx * magX;
		sizeY = sy * magY;
	}

	class Meter {

	public:
		std::unique_ptr<PolygonData2D[]> me = nullptr;
		int wid = 0;
		int hei = 0;
		int px = 0;
		int py = 0;

		bool ActionSt = false;
		int CorrectionX = 0;
		int CorrectionY = 0;
		TCHAR HeaderString[STR_MAX_LENGTH];
		int meterWid = 0;
		float Ratio = 0.0f;
		bool ActionStMeter = false;
		int CorrectionMeterX = 0;
		int Mov = 0;

		Meter() {
			me = std::make_unique<PolygonData2D[]>(2);
		}
		void create(int sizeW, int sizeH, char* headerString) {
			wid = sizeW;
			hei = sizeH;
			int len = (int)strlen(headerString);
			if (len > STR_MAX_LENGTH - 1)len = STR_MAX_LENGTH - 1;
			mbstowcs(HeaderString, headerString, sizeof(TCHAR) * len);
			HeaderString[len] = '\0';
			me[0].GetVBarray2D(256);
			me[1].GetVBarray2D(256);
			me[0].Create(0, true, true, -1, v2, 4, index, 6);
			me[1].Create(0, true, true, -1, v2, 4, index, 6);
		}
		bool checkRange() {
			DxInput* di = DxInput::GetInstance();
			int x = (int)di->PosX();
			int y = (int)di->PosY();
			if (px <= x && x <= px + wid &&
				py <= y && y < py + hei * 0.5f)return true;

			return false;
		}
		bool checkMeterRange() {
			DxInput* di = DxInput::GetInstance();
			int x = (int)di->PosX();
			int y = (int)di->PosY();
			if (px + 2.0f <= x && x <= px + wid - 2.0f &&
				py + hei * 0.5f <= y && y <= py + hei - 2.0f)return true;

			return false;
		}
		float updatePos(int posX, int posY, float depth, int meterMov) {
			px = posX;
			py = posY;
			float meshX = (float)px + wid * 0.5f;
			float meshY = (float)py + hei * 0.5f;
			float strX = (float)px + wid * 0.5f;
			float strY = (float)py + hei * 0.25f;
			int mwidMax = wid - 4;
			meterWid += meterMov;
			if (meterWid < 0)meterWid = 0;
			if (meterWid > mwidMax)meterWid = mwidMax;
			Ratio = (float)meterWid / (float)mwidMax;
			DxText::GetInstance()->
				UpDateText(HeaderString, strX, strY, (float)hei * 0.3f, { 1.0f, 1.0f, 1.0f, 1.0f });
			DxText::GetInstance()->
				UpDateValue((int)(Ratio * 100.0f), strX, strY + hei * 0.5f, (float)hei * 0.3f,
					3, { 1.0f, 1.0f, 1.0f, 1.0f });

			float ppx = meshX;
			float ppy = meshY;
			float wwid = (float)wid;
			float hhei = (float)hei;
			sizeChange(ppx, ppy, wwid, hhei);

			float ppx1 = meshX + (float)meterWid * 0.5f - wid * 0.5f + 2.0f;
			float ppy1 = meshY + hei * 0.25f;
			float wwid1 = (float)meterWid;
			float hhei1 = hei * 0.5f - 2.0f;
			sizeChange(ppx1, ppy1, wwid1, hhei1);

			me[0].Update({ ppx, ppy, depth + 0.02f }, 0,
				{ wwid, hhei }, { 0.0f, 0.3f, 1.0f, 1.0f });
			me[1].Update({ ppx1, ppy1, depth + 0.01f }, 0,
				{ wwid1, hhei1 }, { 1.0f, 0.0f, 0.0f, 1.0f });

			return Ratio;
		}
		float updatePosMouse(float depth) {
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
		void Draw(int comIndex) {
			me[0].Draw(comIndex);
			me[1].Draw(comIndex);
		}
	};

	Meter* me = nullptr;

	class WindowObj {

	public:
		std::unique_ptr<PolygonData2D[]> win = nullptr;
		int wid = 0;
		int hei = 0;
		int fontSize = 0;
		int ChoicePos = 0;
		int numChoice = 0;
		int returnPos = 0;
		TCHAR MenuString[STRTEX_MAX_PCS][STR_MAX_LENGTH] = {};

		WindowObj() {
			returnPos = -1;
			win = std::make_unique<PolygonData2D[]>(1);
		}
		void create(int fontsize) {
			fontSize = fontsize;
			win[0].GetVBarray2D(256);
			win[0].Create(0, true, true, -1, v2, 4, index, 6);
		}
		void setMenuName(int numMenu, char** MenuArr) {
			int maxCntX = 0;
			int maxCntY = numMenu;
			numChoice = numMenu;

			for (int i = 0; i < numMenu; i++) {
				int len = (int)strlen(MenuArr[i]);
				if (len > STR_MAX_LENGTH - 1)len = STR_MAX_LENGTH - 1;
				if (maxCntX < len)maxCntX = len;
				mbstowcs(MenuString[i], MenuArr[i], sizeof(TCHAR) * len);
				MenuString[i][len] = '\0';
			}
			wid = fontSize * maxCntX;
			hei = fontSize * maxCntY;
		}
		int updatePos(int x, int y) {
			returnPos = -1;
			DxInput* di = DxInput::GetInstance();
			switch (di->checkKeyActionNo()) {
			case DIK_UP:
				if (--ChoicePos < 0)ChoicePos = 0;
				break;
			case DIK_DOWN:
				if (++ChoicePos > numChoice - 1)ChoicePos = numChoice - 1;
				break;
			case DIK_RCONTROL:
				returnPos = ChoicePos;
				break;
			case DIK_DELETE:
				returnPos = numChoice;
				break;
			}

			float ppx = (float)x + wid * 0.5f;
			float ppy = (float)y + hei * 0.5f;
			float wwid = (float)wid;
			float hhei = (float)hei;
			sizeChange(ppx, ppy, wwid, hhei);

			float strX = (float)x + wid * 0.5f;
			float strY = (float)y + fontSize * 0.5f;

			win[0].Update({ ppx, ppy, 0.02f }, 0,
				{ wwid, hhei }, { 0.5f, 0.5f, 0.5f, 0.6f });
			for (int i = 0; i < numChoice; i++)
				if (i == ChoicePos)
					DxText::GetInstance()->
					UpDateText(MenuString[i], strX, strY + i * fontSize, (float)fontSize, { 1.0f, 1.0f, 1.0f, 1.0f });
				else
					DxText::GetInstance()->
					UpDateText(MenuString[i], strX, strY + i * fontSize, (float)fontSize, { 0.0f, 0.0f, 1.0f, 1.0f });
			return returnPos;
		}
		void Draw(int comIndex) {
			win[0].Draw(comIndex);
		}
	};

	WindowObj* win = nullptr;
}

UserInterfaceMeter::~UserInterfaceMeter() {
	ARR_DELETE(me);
}

void UserInterfaceMeter::setNumMeter(int num) {
	numMeter = num;
	me = new Meter[numMeter];
}

void UserInterfaceMeter::create(int index, int sizeW, int sizeH, char* headerString) {
	me[index].create(sizeW, sizeH, headerString);
}

float UserInterfaceMeter::updatePos(int index, int posX, int posY, float depth, int meterMov) {
	return me[index].updatePos(posX, posY, depth, meterMov);
}

float UserInterfaceMeter::updatePosMouse(int index, float depth) {
	return me[index].updatePosMouse(depth);
}

void UserInterfaceMeter::Draw(int index, int com) {
	me[index].Draw(com);
}

UserInterfaceWindow::~UserInterfaceWindow() {
	ARR_DELETE(win);
}

void UserInterfaceWindow::setNumWindow(int num) {
	numWin = num;
	win = new WindowObj[numWin];
}

void UserInterfaceWindow::create(int index, int fontSize) {
	win[index].create(fontSize);
}

void UserInterfaceWindow::setMenuName(int index, int numMenu, char** MenuArr) {
	win[index].setMenuName(numMenu, MenuArr);
}

int UserInterfaceWindow::updatePos(int index, int x, int y) {
	return win[index].updatePos(x, y);
}

void UserInterfaceWindow::Draw(int index, int com) {
	win[index].Draw(com);
}