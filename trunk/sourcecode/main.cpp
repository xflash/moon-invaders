// Moon Invaders
#include "basics.h"
#include "input.h"
#include "sprites.h"
#include "invader.h"
#include "menu.h"
#include "player.h"
#include "tower.h"
#include "bullet.h"
#include "level.h"
#include "sound.h"
#include "game.h"
#include "title.h"
#include "systemstub.h"

SystemStub* systemStub;

int main(int argc, char *argv[]) {
	systemStub
    = SystemStub_SDL_create();

  systemStub->init("MI", SCR_WID, SCR_HEI);

	InitSound();
	InitSprites();

	Title();

	ExitSound();
	ExitSprites();

	return 0;
}
