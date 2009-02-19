#include "input.h"
#include "systemstub.h"

byte MouseIsIn(float x,float y,float x2,float y2) {
	short msx,msy;

	msx=systemStub->_pi.mouseX;
	msy=systemStub->_pi.mouseY;

	return (msx>=x && msy>=y && msx<=x2 && msy<=y2);
}

float MouseX(void)
{
	return (float)systemStub->_pi.mouseX;
}

float MouseY(void)
{
	return (float)systemStub->_pi.mouseY;
}