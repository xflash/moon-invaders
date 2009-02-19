#include "sprites.h"
#include "systemstub.h"
#include <stdio.h>

//KGraphic *sprite[NUM_SPRITES];

char sprName[NUM_SPRITES][32]={"invader1.png","invader1b.png","guntower.png","life.png","damage.png","reload.png","sell.png",
								   "mine.png","drill.png","tank.png","launcher.png","range.png","radius.png","freeze.png","power.png",
								   "time.png","invader2.png","invader2b.png","triple.png","gravity.png","freezersch.png",
								   "pierce.png","truck.png","acidup.png","invader3.png","invader3b.png","invader4.png","invader4b.png",
									"bunker.png","discharge.png", 
                  "moon.png", "title_invaders.png", "exit.png", "book.png"};

void InitSprites(void) {
	int i;

	char s[32];
	for(i=0;i<NUM_SPRITES;i++) {
		sprintf(s,"pix\\%s",sprName[i]);
    systemStub->readSurface(s, 0);
	}
}

void ExitSprites(void) {
}

/*
void DrawSprite(byte s,float x,float y) {
	sprite[s]->stretchAlphaRect(0,0,16,16,x,y,x+32,y+32);
}

void DrawSpriteFade(byte s,float x,float y,float blend)
{
	sprite[s]->setBlitColor(1,1,1,blend);
	DrawSprite(s,x,y);
	sprite[s]->setBlitColor(1,1,1,1);
}

void DrawSpriteColored(byte s,float x,float y,float r,float g,float b,float blend)
{
	sprite[s]->setBlitColor(r,g,b,blend);
	DrawSprite(s,x,y);
	sprite[s]->setBlitColor(1,1,1,1);
}


void DrawRect(float x,float y,float x2,float y2,float r,float g,float b,float blend)
{
	sprite[0]->drawRect(x,y,x2,y2,r,g,b,blend);
}

void DrawLine(float x,float y,float x2,float y2,float r,float g,float b,float blend)
{
	sprite[0]->drawLine(x,y,x2,y2,r,g,b,blend);
}
*/