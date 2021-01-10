//*****************************************************************************************//
//**                                                                                     **//
//**                             UserInterface                                           **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_UserInterface_Header
#define Class_UserInterface_Header

#include "../Common/Direct3DWrapper/DxText.h"

class UserInterface;

class Meter {

private:
	friend UserInterface;
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

	Meter();
	void create(int sizeW, int sizeH, char* headerString);
	bool checkRange();
	bool checkMeterRange();
	float updatePos(int posX, int posY, float depth, int meterMov);
	float updatePosMouse(float depth);
	void Draw(int com);
};

class UserInterface {

private:
	int windowWidth;
	int windowHeight;
	Meter* me = nullptr;
	int numMeter = 0;

public:
	~UserInterface();
	void setWindowSize(int Width, int Height);
	void setNumMeter(int num);
	void createMeter(int index, int sizeW, int sizeH, char* headerString);
	float updatePos(int index, int posX, int posY, float depth, int meterMov);
	float updatePosMouse(int index, float depth);
	void Draw(int index, int com);
};

#endif