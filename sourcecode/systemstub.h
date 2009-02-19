#ifndef __SYSTEMSTUB_H__
#define __SYSTEMSTUB_H__

#include "basics.h"
#include <string>
using namespace std;


struct PlayerInput {
	enum {
		DIR_UP    = 1 << 0,
		DIR_DOWN  = 1 << 1,
		DIR_LEFT  = 1 << 2,
		DIR_RIGHT = 1 << 3,
	};
  enum {
    DF_FASTMODE  = 1 << 0,
  };

	uint8 dirMask;
  uint8 dbgMask;
  
  int16 mouseX;
  int16 mouseY;
  bool leftClick;
  bool rightClick;

  bool s;
	bool space;
	bool quit;
  bool escape;

  char lastChar;
};


class SystemStub {
public:
  PlayerInput _pi;

	virtual ~SystemStub() {}

	virtual void init(const char *title, uint16 w, uint16 h) = 0;
	virtual void destroy() = 0;

	virtual void sleep(uint32 duration) = 0;
  virtual void update() = 0;
  virtual uint32 getTimeStamp() = 0;

  virtual uint8 readSurface(string filename, uint32 bgColor)=0;;

  virtual void drawSprite(uint8 surfId, float x, float y) = 0;
  
  virtual void drawSpriteFade(byte s,float x,float y,float blend)=0;
  virtual void drawSpriteColored(byte s,float x,float y,float r,float g,float b,float blend)=0;
  virtual void drawRect(float x,float y,float x2,float y2,float r,float g,float b,float blend)=0;
  virtual void drawLine(float x,float y,float x2,float y2,float r,float g,float b,float blend)=0;

//  virtual void drawImage(uint8 surfId, Rect* srcImg, Point* dstRect) = 0;
//  virtual void drawString(Point* loc, string msg) = 0;
//  virtual void drawPixel(Point* loc) = 0;
//  virtual void drawLine(Point* start, Point* end) = 0;
};

extern SystemStub *SystemStub_SDL_create();
//extern SystemStub *SystemStub_Win32_create();

#endif // __SYSTEMSTUB_H__
