//*****************************************************************************************//
//**                                                                                     **//
//**                             UserInterface                                           **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_UserInterface_Header
#define Class_UserInterface_Header

#include "../Common/Direct3DWrapperOption/DxText.h"

class UserInterfaceMeter {

private:
	int numMeter = 0;

public:
	~UserInterfaceMeter();
	void setNumMeter(int num);
	void create(int index, int sizeW, int sizeH, char* headerString);
	float updatePos(int index, int posX, int posY, float depth, int meterMov);
	float updatePosMouse(int index, float depth);
	void Draw(int index, int com);
};

class UserInterfaceWindow {

private:
	int numWin = 0;

public:
	~UserInterfaceWindow();
	void setNumWindow(int num);
	void create(int index, int fontSize);
	void setMenuName(int index, int numMenu, char** MenuArr);
	int updatePos(int index, int x, int y);
	void Draw(int index, int com);
};

#endif