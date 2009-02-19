#include "menu.h"
#include "player.h"
#include "sprites.h"
#include "input.h"
#include "tower.h"
#include "systemstub.h"

void LifeMeter(void)
{
	int i;

	for(i=0;i<MAX_LIFE;i++)
	{
		if(i<player.life)
			systemStub->drawSprite(SPR_LIFE,SCR_WID-12,SCR_HEI-(i+1.0f)*24);
		else
			systemStub->drawSpriteFade(SPR_LIFE,SCR_WID-12,SCR_HEI-(i+1.0f)*24,0.3f);
	}
}

void MoneyMeter(int hgt,int lvl,float x,float y)
{
	int i;

	for(i=0;i<hgt;i++)
	{
		systemStub->drawRect(x,(float)(y-2-i*3),x+10,(float)(y-i*3),0.7f,0.7f,1.0f,0.4f+0.6f*(i<lvl));
	}
}

void PlayerMoneyMeter(void)
{
	MoneyMeter(MAX_MONEY,player.money,0,SCR_HEI);
}

void BuyOption(float x,float y,byte pic,byte cost,byte on)
{
	float r,g,b;

	if(cost<=player.money)
	{
		r=0;
		g=1.0f;
		b=0;
		if(on)
			r=b=0.6f;
	}
	else
	{
		r=1.0f;
		if(on)
			g=b=0.5f;
		else
			g=b=0;
	}
	systemStub->drawRect(x-1,y-2,x+SPR_SIZE+1,y-1,1.0f,1.0f,1.0f,1.0f);
	systemStub->drawRect(x-1,y+SPR_SIZE+1,x+SPR_SIZE+1,y+SPR_SIZE+2,1.0f,1.0f,1.0f,1.0f);
	systemStub->drawRect(x-2,y-1,x-1,y+SPR_SIZE+1,1.0f,1.0f,1.0f,1.0f);
	systemStub->drawRect(x+SPR_SIZE+1,y-1,x+SPR_SIZE+2,y+SPR_SIZE+1,1.0f,1.0f,1.0f,1.0f);

	systemStub->drawRect(x,y,x+SPR_SIZE,y+SPR_SIZE,r,g,b,0.5f);
	systemStub->drawSpriteFade(pic,x,y,0.7f+0.3f*(on));
	if(cost>0)
		MoneyMeter(10,cost,x+SPR_SIZE+4,y+SPR_SIZE);
}

void ResearchOption(float x,float y,byte pic,byte have,byte on)
{
	float r,g,b;

	if(have)
	{
		b=1.0f;
		r=0.0f;
		g=0.0f;
	}
	else if(player.money>=RS_COST)
	{
		r=1.0f;
		g=1.0f;
		b=0;
		if(on)
			b=0.6f;
	}
	else
	{
		r=1.0f;
		if(on)
			g=b=0.5f;
		else
			g=b=0;
	}
	systemStub->drawRect(x-1,y-2,x+SPR_SIZE+1,y-1,1.0f,1.0f,1.0f,1.0f);
	systemStub->drawRect(x-1,y+SPR_SIZE+1,x+SPR_SIZE+1,y+SPR_SIZE+2,1.0f,1.0f,1.0f,1.0f);
	systemStub->drawRect(x-2,y-1,x-1,y+SPR_SIZE+1,1.0f,1.0f,1.0f,1.0f);
	systemStub->drawRect(x+SPR_SIZE+1,y-1,x+SPR_SIZE+2,y+SPR_SIZE+1,1.0f,1.0f,1.0f,1.0f);

	systemStub->drawRect(x,y,x+SPR_SIZE,y+SPR_SIZE,r,g,b,0.5f);
	systemStub->drawSpriteFade(pic,x,y,0.7f+0.3f*(on && !have));
}

void ShowUpgrade(float x,float y,byte pic)
{
	systemStub->drawRect(x-1,y-2,x+SPR_SIZE+1,y-1,1.0f,1.0f,1.0f,1.0f);
	systemStub->drawRect(x-1,y+SPR_SIZE+1,x+SPR_SIZE+1,y+SPR_SIZE+2,1.0f,1.0f,1.0f,1.0f);
	systemStub->drawRect(x-2,y-1,x-1,y+SPR_SIZE+1,1.0f,1.0f,1.0f,1.0f);
	systemStub->drawRect(x+SPR_SIZE+1,y-1,x+SPR_SIZE+2,y+SPR_SIZE+1,1.0f,1.0f,1.0f,1.0f);

	if(pic!=0)
	{
		switch(pic)
		{
			case UG_DMG:
				pic=SPR_DAMAGE;
				break;
			case UG_RELOAD:
				pic=SPR_RELOAD;
				break;
			case UG_DRILL:
				pic=SPR_DRILL;
				break;
			case UG_FREEZE:
				pic=SPR_FREEZE;
				break;
			case UG_RADIUS:
				pic=SPR_RADIUS;
				break;
			case UG_RANGE:
				pic=SPR_RANGE;
				break;
			case UG_TIME:
				pic=SPR_TIME;
				break;
			case UG_BUNKER:
				pic=SPR_BUNKER;
				break;
		}
		systemStub->drawRect(x,y,x+SPR_SIZE,y+SPR_SIZE,0.5f,0.5f,0.5f,0.5f);
		systemStub->drawSpriteFade(pic,x,y,0.7f);
	}
}

void RenderBuyThing(byte n,byte what,byte cost)
{
	BuyOption((float)(15+n*(SPR_SIZE+18)),SCR_HEI-SPR_SIZE,what,cost,MouseIsIn((float)(15+n*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)n*(SPR_SIZE+18)),SCR_HEI));
}

void RenderBuyThing2(byte n,byte what)
{
	ResearchOption((float)(SCR_WID-18-SPR_SIZE-n*(SPR_SIZE+18)),SCR_HEI-SPR_SIZE,what,player.research[n+1],MouseIsIn((float)(SCR_WID-18-SPR_SIZE-n*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(SCR_WID-18-(float)n*(SPR_SIZE+18)),SCR_HEI));
}

void RenderMenu(void)
{
	int i;

	systemStub->drawRect(12,TOWER_Y,SCR_WID-14,TOWER_Y+2,0.5f,0.5f,0.5f,1.0f);
	LifeMeter();
	PlayerMoneyMeter();
	switch(player.mode)
	{
		case PM_PLAY:
			RenderBuyThing(0,SPR_GUNTOWER,towerPrice[TWR_GUN]);
			RenderBuyThing(1,SPR_MINE,towerPrice[TWR_MINE]);
			RenderBuyThing(2,SPR_LAUNCHER,towerPrice[TWR_MISSILE]);
			RenderBuyThing(3,SPR_TANK,towerPrice[TWR_CRYO]);
			RenderBuyThing(4,SPR_ACID,towerPrice[TWR_ACID]);
			RenderBuyThing(5,SPR_POWER,towerPrice[TWR_POWER]);
			
			RenderBuyThing2(0,SPR_GRAVITY);
			RenderBuyThing2(1,SPR_TRIPLE);
			RenderBuyThing2(2,SPR_FREEZERSCH);
			RenderBuyThing2(3,SPR_PIERCE);
			RenderBuyThing2(4,SPR_ACIDUP);
			RenderBuyThing2(5,SPR_DISCHARGE);
			break;
		case PM_PLACE:
			if(RoomForYou(player.placeX))
			{
				systemStub->drawSpriteColored(TowerPic(player.placingType),player.placeX,TOWER_Y-SPR_SIZE,0,1,0,0.6f);
			}
			else
			{
				systemStub->drawSpriteColored(TowerPic(player.placingType),player.placeX,TOWER_Y-SPR_SIZE,1.0f,0,0,0.6f);
			}
			break;
		case PM_UPGRADE:
			switch(tower[player.upTower].type)
			{
				case TWR_GUN:
					RenderBuyThing(0,SPR_DAMAGE,tower[player.upTower].upgCost);
					RenderBuyThing(1,SPR_RELOAD,tower[player.upTower].upgCost);
					RenderBuyThing(2,SPR_SELL,0);
					break;
				case TWR_MINE:
					RenderBuyThing(0,SPR_DRILL,tower[player.upTower].upgCost);
					RenderBuyThing(1,SPR_BUNKER,tower[player.upTower].upgCost);
					RenderBuyThing(2,SPR_SELL,0);
					break;
				case TWR_MISSILE:
					RenderBuyThing(0,SPR_DAMAGE,tower[player.upTower].upgCost);
					RenderBuyThing(1,SPR_RELOAD,tower[player.upTower].upgCost);
					RenderBuyThing(2,SPR_RADIUS,tower[player.upTower].upgCost);
					RenderBuyThing(3,SPR_SELL,0);
					break;
				case TWR_CRYO:
					RenderBuyThing(0,SPR_DAMAGE,tower[player.upTower].upgCost);
					RenderBuyThing(1,SPR_RELOAD,tower[player.upTower].upgCost);
					RenderBuyThing(2,SPR_RANGE,tower[player.upTower].upgCost);
					RenderBuyThing(3,SPR_FREEZE,tower[player.upTower].upgCost);
					RenderBuyThing(4,SPR_SELL,0);
					break;
				case TWR_POWER:
					RenderBuyThing(0,SPR_DAMAGE,tower[player.upTower].upgCost);
					RenderBuyThing(1,SPR_RELOAD,tower[player.upTower].upgCost);
					RenderBuyThing(2,SPR_RANGE,tower[player.upTower].upgCost);
					RenderBuyThing(3,SPR_TIME,tower[player.upTower].upgCost);
					RenderBuyThing(4,SPR_SELL,0);
					break;
				case TWR_ACID:
					RenderBuyThing(0,SPR_RELOAD,tower[player.upTower].upgCost);
					RenderBuyThing(1,SPR_TIME,tower[player.upTower].upgCost);
					RenderBuyThing(2,SPR_RADIUS,tower[player.upTower].upgCost);
					RenderBuyThing(3,SPR_SELL,0);
					break;
			}
			for(i=0;i<10;i++)
			{
				ShowUpgrade(SCR_WID-SPR_SIZE-16-(SPR_SIZE+4)*(float)i,SCR_HEI-SPR_SIZE,tower[player.upTower].upg[i]);
			}
			break;
	}
}

byte OnBuy(void)
{
	if(MouseIsIn((float)(15+0*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)0*(SPR_SIZE+18)),SCR_HEI))
		return TWR_GUN;
	else if(MouseIsIn((float)(15+1*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)1*(SPR_SIZE+18)),SCR_HEI))
		return TWR_MINE;
	else if(MouseIsIn((float)(15+2*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)2*(SPR_SIZE+18)),SCR_HEI))
		return TWR_MISSILE;
	else if(MouseIsIn((float)(15+3*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)3*(SPR_SIZE+18)),SCR_HEI))
		return TWR_CRYO;
	else if(MouseIsIn((float)(15+4*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)4*(SPR_SIZE+18)),SCR_HEI))
		return TWR_ACID;
	else if(MouseIsIn((float)(15+5*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(15+SPR_SIZE+(float)5*(SPR_SIZE+18)),SCR_HEI))
		return TWR_POWER;
	else
		return 0;
}

byte OnResearch(void)
{
	if(MouseIsIn((float)(SCR_WID-18-SPR_SIZE-0*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(SCR_WID-18-(float)0*(SPR_SIZE+18)),SCR_HEI))
		return RS_GRAVITY;
	else if(MouseIsIn((float)(SCR_WID-18-SPR_SIZE-1*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(SCR_WID-18-(float)1*(SPR_SIZE+18)),SCR_HEI))
		return RS_TRIPLE;
	else if(MouseIsIn((float)(SCR_WID-18-SPR_SIZE-2*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(SCR_WID-18-(float)2*(SPR_SIZE+18)),SCR_HEI))
		return RS_FREEZE;
	else if(MouseIsIn((float)(SCR_WID-18-SPR_SIZE-3*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(SCR_WID-18-(float)3*(SPR_SIZE+18)),SCR_HEI))
		return RS_PIERCE;
	else if(MouseIsIn((float)(SCR_WID-18-SPR_SIZE-4*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(SCR_WID-18-(float)4*(SPR_SIZE+18)),SCR_HEI))
		return RS_CORRODE;
	else if(MouseIsIn((float)(SCR_WID-18-SPR_SIZE-5*(SPR_SIZE+16)),SCR_HEI-SPR_SIZE,(SCR_WID-18-(float)5*(SPR_SIZE+18)),SCR_HEI))
		return RS_DISCHARGE;
	else
		return 0;
}