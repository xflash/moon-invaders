#include "tower.h"
#include "sprites.h"
#include "bullet.h"
#include "invader.h"
#include "input.h"
#include "player.h"
#include "sound.h"
#include "systemstub.h"

tower_t tower[MAX_TOWERS];

byte towerPrice[TWR_TYPES]={0,3,4,5,6,10,7};

void ClearTowers(void)
{
	int i;

	for(i=0;i<MAX_TOWERS;i++)
		tower[i].type=0;
}

void PlaceTower(float x,byte type)
{
	int i,j;

	for(i=0;i<MAX_TOWERS;i++)
	{
		if(tower[i].type==0)
		{
			tower[i].type=type;
			tower[i].x=x;
			tower[i].charge=0;
			tower[i].chargeAmt=0;
			for(j=0;j<10;j++)
				tower[i].upg[j]=0;
			switch(type)
			{
				case TWR_GUN:
					tower[i].dmg=2;
					tower[i].maxReload=30;
					break;
				case TWR_ACID:
					tower[i].dmg=4;
					tower[i].maxReload=30*2;
					tower[i].range=SPR_SIZE/2;
					tower[i].freeze=30;
					break;
				case TWR_MINE:
					tower[i].maxReload=30*20;
					break;
				case TWR_MISSILE:
					tower[i].dmg=7;
					tower[i].maxReload=30*5;
					tower[i].range=SPR_SIZE;
					tower[i].freeze=0;
					break;
				case TWR_CRYO:
					tower[i].dmg=1;
					tower[i].maxReload=30*3;
					tower[i].range=SPR_SIZE;
					tower[i].freeze=30;
					break;
				case TWR_POWER:
					tower[i].dmg=1;
					tower[i].maxReload=30*5;
					tower[i].range=SPR_SIZE;
					tower[i].freeze=30;
					break;
			}
			tower[i].upgCost=1;
			tower[i].reload=tower[i].maxReload;
			return;
		}
	}
}

void PowerTowers(float x,float range,word duration,byte dmg)
{
	int i;

	for(i=0;i<MAX_TOWERS;i++)
	{
		if(tower[i].x+SPR_SIZE>=x-range && tower[i].x<=x+range)
		{
			if(tower[i].chargeAmt+dmg<=10)
				tower[i].chargeAmt+=dmg;
			else
				tower[i].chargeAmt=10;
			if(tower[i].charge<duration)
				tower[i].charge=duration;
		}
	}
}

void UpdateTower(tower_t *me)
{
	word w;
	if(me->reload==0)
	{
		switch(me->type)
		{
			case TWR_GUN:
				FireBullet(me,0);
				Sound(SND_GUN);
				me->reload=me->maxReload;
				break;
			case TWR_MINE:
				if(player.money<160)
				{
					player.money++;
					if(me->chargeAmt && player.money<160)
						player.money++;
					me->reload=me->maxReload;
					Sound(SND_MINE);
				}
				break;
			case TWR_MISSILE:
				FireBullet(me,0);
				Sound(SND_MISSILE);
				me->reload=me->maxReload;
				break;
			case TWR_CRYO:
				w=FindTarget(me->x+SPR_SIZE/2,me->range,0);
				if(w)
				{
					FireBullet(me,w-1);
					Sound(SND_LASER);
					me->reload=me->maxReload;
				}
				break;
			case TWR_POWER:
				PowerTowers(me->x+SPR_SIZE/2,me->range,me->freeze,me->dmg);
				me->reload=me->maxReload;
				Sound(SND_POWER);
				FireBullet(me,0);
				break;
			case TWR_ACID:
				FireBullet(me,0);
				Sound(SND_GUN);
				me->reload=me->maxReload;
				break;
		}
	}
	else
	{
		me->reload--;
	}
	if(me->charge)
		me->charge--;
	else
		me->chargeAmt=0;
}

void UpdateTowers(void)
{
	int i;

	for(i=0;i<MAX_TOWERS;i++)
	{
		if(tower[i].type)
			UpdateTower(&tower[i]);
	}
}

byte RoomForYou(float x)
{
	int i;

	for(i=0;i<MAX_TOWERS;i++)
	{
		if(tower[i].type && tower[i].x>x-SPR_SIZE && tower[i].x<x+SPR_SIZE)
			return 0;
	}
	return 1;
}

byte TowerPic(byte type)
{
	switch(type)
	{
		case TWR_GUN:
			return SPR_GUNTOWER;
			break;
		case TWR_MINE:
			return SPR_MINE;
			break;
		case TWR_CRYO:
			return SPR_TANK;
			break;
		case TWR_MISSILE:
			return SPR_LAUNCHER;
			break;
		case TWR_POWER:
			return SPR_POWER;
			break;
		case TWR_ACID:
			return SPR_ACID;
			break;
	}
	return 0;
}

void RenderTowers(void)
{
	int i,h;

	for(i=0;i<MAX_TOWERS;i++)
	{
		if(player.mode==PM_UPGRADE && i==player.upTower)
		{
			systemStub->drawSpriteColored(TowerPic(tower[i].type),tower[i].x-2,TOWER_Y-SPR_SIZE-2,0,1,1,0.5f);
			systemStub->drawSpriteColored(TowerPic(tower[i].type),tower[i].x-2,TOWER_Y-SPR_SIZE+2,0,1,1,0.5f);
			systemStub->drawSpriteColored(TowerPic(tower[i].type),tower[i].x+2,TOWER_Y-SPR_SIZE+2,0,1,1,0.5f);
			systemStub->drawSpriteColored(TowerPic(tower[i].type),tower[i].x+2,TOWER_Y-SPR_SIZE-2,0,1,1,0.5f);

			if(tower[i].type==TWR_CRYO)
				systemStub->drawRect(tower[i].x+SPR_SIZE/2-tower[i].range,0,tower[i].x+SPR_SIZE/2+tower[i].range,TOWER_Y,0,1,1,0.3f);
			if(tower[i].type==TWR_POWER)
				systemStub->drawRect(tower[i].x+SPR_SIZE/2-tower[i].range,0,tower[i].x+SPR_SIZE/2+tower[i].range,TOWER_Y,1,1,0,0.3f);
		}
		if(tower[i].type==TWR_MINE)
		{
			h=(int)(18.5f-18.5f*(float)tower[i].reload/(float)tower[i].maxReload);
			systemStub->drawRect((float)(tower[i].x+10),(float)(TOWER_Y-h),tower[i].x+22,TOWER_Y,0,1,0,1);
		}
		if(tower[i].type)
		{
			if(tower[i].charge)
				systemStub->drawSpriteColored(TowerPic(tower[i].type),tower[i].x,TOWER_Y-SPR_SIZE,1,1,0,1);
			else
				systemStub->drawSprite(TowerPic(tower[i].type),tower[i].x,TOWER_Y-SPR_SIZE);
		}
	}
}

byte OnTower(void)
{
	int i;

	for(i=0;i<MAX_TOWERS;i++)
	{
		if(tower[i].type && MouseIsIn(tower[i].x,TOWER_Y-SPR_SIZE,tower[i].x+SPR_SIZE,TOWER_Y))
		{
			return (byte)(i+1);
		}
	}
	return 0;
}

void SellTower(void)
{
	int i;
	byte c;

	c=towerPrice[tower[player.upTower].type];

	for(i=0;i<10;i++)
		if(tower[player.upTower].upg[i])
			c+=(i+1);
	if(player.money+(c/2)<=160)
		player.money+=(c/2);
	else
		player.money=160;
	tower[player.upTower].type=0;
}

void UpgradeTower(byte upType)
{
	int i,amt;
	
	if(player.money<tower[player.upTower].upgCost)
	{
		Sound(SND_NOMONEY);
		return;	// can't afford
	}

	for(i=0;i<10;i++)
		if(!tower[player.upTower].upg[i])
		{
			tower[player.upTower].upg[i]=upType;
			player.money-=tower[player.upTower].upgCost;
			if(tower[player.upTower].upgCost<10)
				tower[player.upTower].upgCost++;
			switch(upType)
			{
				case UG_DMG:
					tower[player.upTower].dmg++;
					if(tower[player.upTower].type==TWR_MISSILE)
						tower[player.upTower].dmg++;
					break;
				case UG_RELOAD:
					if(tower[player.upTower].type==TWR_GUN || tower[player.upTower].type==TWR_ACID)
						amt=6;
					else
						amt=10;
					if(tower[player.upTower].maxReload>amt)
						tower[player.upTower].maxReload-=amt;
					if(tower[player.upTower].reload>tower[player.upTower].maxReload)
						tower[player.upTower].reload=tower[player.upTower].maxReload;
					break;
				case UG_DRILL:
					if(tower[player.upTower].maxReload>30)
						tower[player.upTower].maxReload-=30;
					if(tower[player.upTower].reload>tower[player.upTower].maxReload)
						tower[player.upTower].reload=tower[player.upTower].maxReload;
					break;
				case UG_FREEZE:
					tower[player.upTower].freeze+=30;
					break;
				case UG_TIME:
					tower[player.upTower].freeze+=15;
					break;
				case UG_RADIUS:
					tower[player.upTower].range+=10.0f;
					break;
				case UG_RANGE:
					tower[player.upTower].range+=SPR_SIZE/2.0f;
					break;
				case UG_BUNKER:
					// nothing
					break;
			}
			Sound(SND_UPGRADE);
			return;
		}

	Sound(SND_NOMONEY);
	return;	// no room for upgrades
}

byte SaveLife(void)
{
	int i,j;

	for(i=0;i<MAX_TOWERS;i++)
	{
		if(tower[i].type==TWR_MINE)
		{
			for(j=0;j<10;j++)
			{
				if(tower[i].upg[j]==UG_BUNKER)
				{
					tower[i].upg[j]=0;
					return 1;
				}
			}
		}
	}
	return 0;
}