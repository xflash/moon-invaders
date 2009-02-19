#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include "systemstub.h"
#include <sstream>
#include <iostream>
#include <string>
#include <map>
using namespace std;

class SystemException {
private:
  string _msg;
public:
  SystemException(string msg, ...):_msg(msg) {}
};

class SystemStub_SDL : public SystemStub {
public:
  enum {
    SCREEN_BPP = 24,
  };
	SDL_Surface* _screen;
  TTF_Font* _font;
  SDL_Surface* _txtSurf;
	map<uint8, SDL_Surface*> _surfaces;
  uint8 _surfaceCount;
	uint16 _screenW, _screenH;

	virtual ~SystemStub_SDL() {}
	virtual void init(const char *title, uint16 w, uint16 h);
	virtual void destroy();
	virtual void processEvents();
	virtual void sleep(uint32 duration);
  virtual void update();
  virtual uint32 getTimeStamp();
  virtual uint8 readSurface(string filename, uint32 bgColor);

  virtual void drawSprite(uint8 surfId, float x, float y);
  virtual void drawSpriteFade(byte s,float x,float y,float blend);
  virtual void drawSpriteColored(byte s,float x,float y,float r,float g,float b,float blend);
  virtual void drawRect(float x,float y,float x2,float y2,float r,float g,float b,float blend);
  virtual void drawLine(float x,float y,float x2,float y2,float r,float g,float b,float blend);

  /*
virtual void drawImage(uint8 resId, Rect* srcImg, Point* dstRect);
  virtual void drawString(Point* loc, string msg);
  virtual void drawPixel(Point* loc);
  virtual void drawLine(Point* start, Point* end);
*/
};

SystemStub *SystemStub_SDL_create() {
	return new SystemStub_SDL();
}

void SystemStub_SDL::init(const char *title, uint16 w, uint16 h) {
	SDL_Init(SDL_INIT_VIDEO);
	//SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_SetCaption(title, NULL);
	memset(&_pi, 0, sizeof(_pi));
	_screenW = w;
	_screenH = h;
  _screen = SDL_SetVideoMode(w, h, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (!_screen)
		throw new SystemException("SystemStub_SDL Unable to allocate _screen buffer");

  _surfaceCount=0;

  TTF_Init();
  _font = TTF_OpenFont("datas/arial.ttf", 12);
  _txtSurf = NULL;

}
void SystemStub_SDL::destroy() {
/*
if (_resources) {
    for(int i=0; i<Resources::MAX_ID; i++) {
  		SDL_FreeSurface(_resources[i]);
    }

    delete[] _resources;
		_resources = 0;
	}
*/
  if (_screen) {
		// freed by SDL_Quit()
		_screen = 0;
	}
  TTF_CloseFont(_font);
  TTF_Quit();
	SDL_Quit();
}

void SystemStub_SDL::update() {
  
  SDL_Flip(_screen);
  SDL_FillRect(_screen, &_screen->clip_rect, SDL_MapRGB(_screen->format, 0x00, 0x00, 0x00) ); 

	static uint32 tstamp = 0;
	int32 delay = getTimeStamp() - tstamp;
	int32 pause = (_pi.dbgMask & PlayerInput::DF_FASTMODE) ? 5 : 30;
	pause -= delay;
	if (pause > 0) {
		this->sleep(pause);
	}
	tstamp = this->getTimeStamp();

  processEvents();
}

void SystemStub_SDL::processEvents() {
	SDL_Event ev;


	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			_pi.quit = true;
			break;
    case SDL_MOUSEMOTION:
      _pi.mouseX = ev.motion.x;
      _pi.mouseY = ev.motion.y;
      break;
    case SDL_MOUSEBUTTONDOWN:
      switch(ev.button.button) {
      case SDL_BUTTON_LEFT:
        _pi.leftClick = true;
        break;
      case SDL_BUTTON_RIGHT:
        _pi.rightClick = true;
        break;
      }
      break;
    case SDL_MOUSEBUTTONUP:
      switch(ev.button.button) {
      case SDL_BUTTON_LEFT:
        _pi.leftClick = false;
        break;
      case SDL_BUTTON_RIGHT:
        _pi.rightClick = false;
        break;
      }
      break;
      break;
		case SDL_KEYDOWN:
			_pi.lastChar = ev.key.keysym.sym;
			switch (ev.key.keysym.sym) {
			case SDLK_LEFT:
				_pi.dirMask |= PlayerInput::DIR_LEFT;
				break;
			case SDLK_RIGHT:
				_pi.dirMask |= PlayerInput::DIR_RIGHT;
				break;
			case SDLK_UP:
				_pi.dirMask |= PlayerInput::DIR_UP;
				break;
			case SDLK_DOWN:
				_pi.dirMask |= PlayerInput::DIR_DOWN;
				break;
      case SDLK_TAB:
        _pi.dbgMask |= PlayerInput::DF_FASTMODE;
        break;
			case SDLK_SPACE:
				_pi.space = true;
				break;
			case SDLK_ESCAPE:
				_pi.escape = true;
				break;
			default:
				break;
			}
			break;
    case SDL_KEYUP:
			switch (ev.key.keysym.sym) {
			case SDLK_LEFT:
				_pi.dirMask &= ~PlayerInput::DIR_LEFT;
				break;
			case SDLK_RIGHT:
				_pi.dirMask &= ~PlayerInput::DIR_RIGHT;
				break;
			case SDLK_UP:
				_pi.dirMask &= ~PlayerInput::DIR_UP;
				break;
			case SDLK_DOWN:
				_pi.dirMask &= ~PlayerInput::DIR_DOWN;
				break;
			case SDLK_SPACE:
				_pi.space = false;
				break;
      case SDLK_TAB:
        _pi.dbgMask &= ~PlayerInput::DF_FASTMODE;
        break;
			case SDLK_ESCAPE:
				_pi.escape = false;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

void SystemStub_SDL::sleep(uint32 duration) {
	SDL_Delay(duration);
}

uint32 SystemStub_SDL::getTimeStamp() {
	return SDL_GetTicks();
}


/*
void SystemStub_SDL::drawString(Point* loc, string msg) {
  if(_txtSurf!=NULL)
    SDL_FreeSurface(_txtSurf);
  SDL_Color bg = {0,0,0};
  SDL_Color fg = {0x1f,0x2f,0x3f};
  _txtSurf = TTF_RenderText_Blended(_font, msg.c_str(), fg);

  SDL_Rect blitDst;
  blitDst.x=loc->x; blitDst.y=loc->y;
  SDL_BlitSurface(_txtSurf, NULL, _screen, &blitDst);
}
*/

uint8 SystemStub_SDL::readSurface(string filename, uint32 bgColor) {
  cout << "Loading Surface from ("<<filename<<") - ";
  SDL_Surface* loadedImage=IMG_Load(filename.c_str());
  if(loadedImage!=NULL) { 
    SDL_Surface* optimized=SDL_DisplayFormat(loadedImage);
    //SDL_Surface* optimized=loadedImage;
    SDL_SetColorKey(optimized, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(optimized->format, (bgColor>>16)&0x0000FF, (bgColor>>8)&0x00FF, (bgColor)&0xFF));
    cout << "OK : size (" << loadedImage->w << "x" << loadedImage->h << ")" << endl;
    SDL_FreeSurface(loadedImage);
    _surfaces[_surfaceCount]=optimized;
    uint8 res=_surfaceCount;
    _surfaceCount++;
    return res;
  }
  cout << "NOK" << endl;
  throw new SystemException("Unable to read Surface from %s",filename.c_str());
}
/*
void SystemStub_SDL::drawPixel(Point* loc) {
  pixelColor(_screen, loc->x, loc->y, 0);
}

void SystemStub_SDL::drawLine(Point* start, Point* end) {
  lineColor(_screen, start->x, start->y, end->x, end->y, 0x0F0F0FFF);
}
*/

void SystemStub_SDL::drawSprite(uint8 surfId, float x, float y) {
  SDL_Surface* surfToBlit=_surfaces[surfId];
  SDL_Rect blitDst;
  blitDst.x=(int16)x;
  blitDst.y=(int16)y;
  blitDst.w=surfToBlit->w;
  blitDst.h=surfToBlit->h;
  SDL_BlitSurface(surfToBlit, NULL, _screen, &blitDst);
}

void SystemStub_SDL::drawSpriteFade(byte surfId,float x,float y,float blend) {
  SDL_Surface* surfToBlit=_surfaces[surfId];
  SDL_Rect blitDst;
  blitDst.x=(int16)x;
  blitDst.y=(int16)y;
  blitDst.w=surfToBlit->w;
  blitDst.h=surfToBlit->h;

  SDL_SetAlpha(surfToBlit, SDL_SRCALPHA | SDL_RLEACCEL, (uint8)blend*0xFF);
  SDL_BlitSurface(surfToBlit, NULL, _screen, &blitDst);
}

void SystemStub_SDL::drawSpriteColored(byte surfId,float x,float y,float r,float g,float b,float blend) {
  SDL_Surface* surfToBlit=_surfaces[surfId];
  SDL_Rect blitDst;
  blitDst.x=(int16)x;
  blitDst.y=(int16)y;
  blitDst.w=surfToBlit->w;
  blitDst.h=surfToBlit->h;

  //boxRGBA(_screen, (int16)x, (int16)y, (int16)x + surfToBlit->h, (int16)y +surfToBlit->h, (uint8)r*255, (uint8)g*255, (uint8)b*255, (uint8)blend*255);
//  SDL_SetAlpha(surfToBlit, SDL_SRCALPHA | SDL_RLEACCEL, (uint8)blend*0xFF);
  SDL_BlitSurface(surfToBlit, NULL, _screen, &blitDst);
}

void SystemStub_SDL::drawRect(float x,float y,float x2,float y2,float r,float g,float b,float blend) {
  boxRGBA(_screen, (int16)x, (int16)y, (int16)x2, (int16)y2, (uint8)r*255, (uint8)g*255, (uint8)b*255, (uint8)blend*255);
}

void SystemStub_SDL::drawLine(float x,float y,float x2,float y2,float r,float g,float b,float blend) {
  lineRGBA(_screen, (int16)x, (int16)y, (int16)x2, (int16)y2, (uint8)r*255, (uint8)g*255, (uint8)b*255, (uint8)blend*255);
}


