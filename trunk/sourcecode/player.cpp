#include "player.h"
#include "menu.h"
#include "tower.h"
#include "input.h"
#include "sound.h"
#include "systemstub.h"

player_t player;

void InitPlayer(void) {
	int i;

	for(i=0;i<MAX_RESEARCH;i++)
		player.research[i]=0;
	player.life=MAX_LIFE;
	player.money=15;
	player.mode=PM_PLAY;
	player.oldBtn=1;
	player.oldRBtn=1;
	player.level=4;
	player.gameSpeed=30;
}

void SetPlaceX(void) {
	player.placeX=MouseX();
	if(player.placeX<SPR_SIZE/2)
		player.placeX=SPR_SIZE/2;
	if(player.placeX>SCR_WID-SPR_SIZE*1.5f)
		player.placeX=SCR_WID-SPR_SIZE*1.5f;
}

void UpdatePlayer() {
	byte b,r,btn,rbtn;

	btn=systemStub->_pi.leftClick;
	rbtn=systemStub->_pi.rightClick;
	SetPlaceX();

	if(systemStub->_pi.space) {
		if(player.gameSpeed!=60) {
			player.gameSpeed=60;
//			win->setMaxFrameRate(60);
		}
	} else {
		if(player.gameSpeed!=30) {
			player.gameSpeed=30;
		}
	}

  switch(player.mode) {
		case PM_PLAY:
			if(btn && !player.oldBtn) {
				b=OnBuy();
				r=OnResearch();
				if(b) {
					if(player.money>=towerPrice[b]) {
						player.mode=PM_PLACE;
						player.placingType=b;
						Sound(SND_SELECT);
					}
				} else if(r && player.research[r]==0) {
					if(player.money>=RS_COST) {
						Sound(SND_RESEARCH);
						player.research[r]=1;
						player.money-=RS_COST;
					} else
						Sound(SND_NOMONEY);
				} else {
					b=OnTower();
					if(b) {
						player.mode=PM_UPGRADE;
						player.upTower=b-1;
						Sound(SND_SELECT);
					}
				}
			}
			break;
		case PM_PLACE:
			if(player.money<towerPrice[player.placingType]) {
				player.mode=PM_PLAY;
				Sound(SND_NOMONEY);
			} else if(rbtn && !player.oldRBtn) {
				player.mode=PM_PLAY;
				Sound(SND_CANCEL);
			} else if(btn && !player.oldBtn) {
				if(RoomForYou(player.placeX)) {
					player.money-=towerPrice[player.placingType];
					PlaceTower(player.placeX,player.placingType);
					Sound(SND_PLACE);
				}
			}
			break;
		case PM_UPGRADE:
			if(rbtn && !player.oldRBtn) {
				Sound(SND_CANCEL);
				player.mode=PM_PLAY;
			} else if(btn && !player.oldBtn) {
				b=OnTower();
				if(b) {
					player.upTower=b-1;
					Sound(SND_SELECT);
				} else {
					switch(tower[player.upTower].type) {
						case TWR_GUN:
							if(MouseIsIn((float)(15+0*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)0*(SPR_SIZE+18)),SCR_HEI)) {
								UpgradeTower(UG_DMG);
							} else if(MouseIsIn((float)(15+1*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)1*(SPR_SIZE+18)),SCR_HEI)) {
								UpgradeTower(UG_RELOAD);
							} else if(MouseIsIn((float)(15+2*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)2*(SPR_SIZE+18)),SCR_HEI)) {
								SellTower();
								Sound(SND_SELL);
								player.mode=PM_PLAY;
							}
							break;
						case TWR_MINE:
							if(MouseIsIn((float)(15+0*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)0*(SPR_SIZE+18)),SCR_HEI)) {
								UpgradeTower(UG_DRILL);
							}
							else if(MouseIsIn((float)(15+1*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)1*(SPR_SIZE+18)),SCR_HEI)) {
								UpgradeTower(UG_BUNKER);
							} else if(MouseIsIn((float)(15+2*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)2*(SPR_SIZE+18)),SCR_HEI)) {
								SellTower();
								Sound(SND_SELL);
								player.mode=PM_PLAY;
							}
							break;
						case TWR_MISSILE:
							if(MouseIsIn((float)(15+0*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)0*(SPR_SIZE+18)),SCR_HEI)) {
								UpgradeTower(UG_DMG);
							}
							else if(MouseIsIn((float)(15+1*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)1*(SPR_SIZE+18)),SCR_HEI)) {
								UpgradeTower(UG_RELOAD);
							}
							else if(MouseIsIn((float)(15+2*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)2*(SPR_SIZE+18)),SCR_HEI)) {
								UpgradeTower(UG_RADIUS);
							}
							else if(MouseIsIn((float)(15+3*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)3*(SPR_SIZE+18)),SCR_HEI)) {
								SellTower();
								Sound(SND_SELL);
								player.mode=PM_PLAY;
							}
							break;
						case TWR_CRYO:
							if(MouseIsIn((float)(15+0*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)0*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_DMG);
							}
							else if(MouseIsIn((float)(15+1*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)1*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_RELOAD);
							}
							else if(MouseIsIn((float)(15+2*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)2*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_RANGE);
							}
							else if(MouseIsIn((float)(15+3*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)3*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_FREEZE);
							}
							else if(MouseIsIn((float)(15+4*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)4*(SPR_SIZE+18)),SCR_HEI))
							{
								SellTower();
								Sound(SND_SELL);
								player.mode=PM_PLAY;
							}
							break;
						case TWR_POWER:
							if(MouseIsIn((float)(15+0*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)0*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_DMG);
							}
							else if(MouseIsIn((float)(15+1*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)1*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_RELOAD);
							}
							else if(MouseIsIn((float)(15+2*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)2*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_RANGE);
							}
							else if(MouseIsIn((float)(15+3*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)3*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_TIME);
							}
							else if(MouseIsIn((float)(15+4*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)4*(SPR_SIZE+18)),SCR_HEI))
							{
								SellTower();
								Sound(SND_SELL);
								player.mode=PM_PLAY;
							}
							break;
						case TWR_ACID:
							if(MouseIsIn((float)(15+0*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)0*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_RELOAD);
							}
							else if(MouseIsIn((float)(15+1*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)1*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_TIME);
							}
							else if(MouseIsIn((float)(15+2*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)2*(SPR_SIZE+18)),SCR_HEI))
							{
								UpgradeTower(UG_RADIUS);
							}
							else if(MouseIsIn((float)(15+3*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)3*(SPR_SIZE+18)),SCR_HEI))
							{
								SellTower();
								Sound(SND_SELL);
								player.mode=PM_PLAY;
							}
							break;
					}
				}
			}
			break;
	}
	player.oldBtn=btn;
	player.oldRBtn=rbtn;
}