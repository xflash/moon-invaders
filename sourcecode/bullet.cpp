#include "bullet.h"
#include "tower.h"
#include "invader.h"
#include "sprites.h"
#include "player.h"
#include "sound.h"
#include <stdlib.h>
#include "systemstub.h"


bullet_t bul[MAX_BULLETS];

void ClearBullets(void)
{
	int i;

	for(i=0;i<MAX_BULLETS;i++)
		bul[i].type=0;
}

int AddBullet(byte type,float x,float y,float dx,float dy,byte dmg,float radius)
{
	int i;

	for(i=0;i<MAX_BULLETS;i++)
		if(bul[i].type==0)
		{
			bul[i].x=x;
			bul[i].y=y;
			bul[i].type=type;
			bul[i].dx=dx;
			bul[i].dy=dy;
			bul[i].dmg=dmg;
			bul[i].radius=radius;
			bul[i].acid=0;
			return (word)i;
		}
	return -1;
}

float IGiveUpLetsDoThisWay(float x,float y,float x2,float y2)
{
	float tx,ty,dx,dy;
	int i;

	dx=(x2-x)/320.0f;
	dy=(y2-y)/320.0f;
	tx=x;
	ty=y;
	i=0;
	while(ty>0)
	{
		i++;
		tx=x+dx*(float)i;
		ty=y+dy*(float)i;
	}
	return tx;
}

void FireBullet(tower_t *who,word tgt)
{
	int i;
	switch(who->type)
	{
		case TWR_GUN:
			AddBullet(BL_GUN,who->x+SPR_SIZE/2-1,TOWER_Y-SPR_SIZE,0,-24,who->dmg+who->chargeAmt,0);
			if(player.research[RS_TRIPLE])
			{
				AddBullet(BL_GUN,who->x+SPR_SIZE/2-1,TOWER_Y-SPR_SIZE,-8,-16,(who->dmg+who->chargeAmt)/4+1,0);
				AddBullet(BL_GUN,who->x+SPR_SIZE/2-1,TOWER_Y-SPR_SIZE,8,-16,(who->dmg+who->chargeAmt)/4+1,0);
			}
			break;
		case TWR_ACID:
			i=AddBullet(BL_ACID,who->x+SPR_SIZE/4-1,TOWER_Y-SPR_SIZE,0,-8,who->dmg+who->chargeAmt,who->range);
			if(i!=-1)
				bul[i].acid=who->freeze;
			Sound(SND_TRUCKSHOOT);
			break;
		case TWR_MISSILE:
			AddBullet(BL_MISSILE,who->x+SPR_SIZE/2-1,TOWER_Y-SPR_SIZE,0,-4,who->dmg+who->chargeAmt,who->range);
			break;
		case TWR_CRYO:
			float tx,ty;
			tx=who->x+SPR_SIZE/2-1;
			ty=0;
			InvaderPos(tgt,&tx,&ty);
			tx=IGiveUpLetsDoThisWay(who->x+SPR_SIZE/2-1,TOWER_Y-SPR_SIZE+4,tx,ty);
			ty=0;
			AddBullet(BL_LASER,who->x+SPR_SIZE/2-1,TOWER_Y-SPR_SIZE+4,tx,ty,who->dmg+who->chargeAmt,8.0f);
			LaserGuys(who->x+SPR_SIZE/2-1,TOWER_Y-SPR_SIZE+4,tx,ty,who->dmg+who->chargeAmt,who->freeze);
			break;
		case TWR_POWER:
			if(player.research[RS_DISCHARGE])
			{
				BlowUpMissile(who->x+SPR_SIZE/2,TOWER_Y-SPR_SIZE-SPR_SIZE/2,SPR_SIZE,59999);
				AddBullet(BL_DISCHARGE,who->x+SPR_SIZE/2,TOWER_Y-SPR_SIZE-SPR_SIZE/2,0,0,15,SPR_SIZE);
			}
			break;
	}
}

void UpdateBullets(void)
{
	int i;

	for(i=0;i<MAX_BULLETS;i++)
	{
		switch(bul[i].type)
		{
			case BL_GUN:
				bul[i].x+=bul[i].dx;
				bul[i].y+=bul[i].dy;
				if(bul[i].y<-SPR_SIZE || HitInvaders(bul[i].x,bul[i].y,bul[i].dmg))
					bul[i].type=0;
				break;
			case BL_ACID:
				bul[i].x+=bul[i].dx;
				bul[i].y+=bul[i].dy;
				bul[i].dx+=(float)(rand()%1001)/2000.0f-0.25f;
				if(bul[i].y<-SPR_SIZE || bul[i].x<-SPR_SIZE || bul[i].x>SCR_WID+SPR_SIZE)
					bul[i].type=0;
				else if(HitInvadersAcid(bul[i].x,bul[i].y,bul[i].dmg,(byte)bul[i].acid,bul[i].radius))
				{
					Sound(SND_ACID);
					bul[i].type=BL_ACIDBOOM;
					bul[i].dmg=15;
				}
				break;
			case BL_FREEZER:
				bul[i].x+=bul[i].dx;
				bul[i].y+=bul[i].dy;
				if(bul[i].y<-SPR_SIZE || bul[i].x<-SPR_SIZE || bul[i].x>SCR_WID+SPR_SIZE || HitInvadersFrz(bul[i].x,bul[i].y,bul[i].dmg,(word)bul[i].radius))
					bul[i].type=0;
				break;
			case BL_MISSILE:
				bul[i].x+=bul[i].dx;
				bul[i].y+=bul[i].dy;
				if(PullMissile(bul[i].x,bul[i].y,&bul[i].dx,&bul[i].dy))
				{
					Sound(SND_MISBOOM);
					if(player.research[RS_GRAVITY])
						BlowUpMissile(bul[i].x,bul[i].y,bul[i].radius*3/2,bul[i].dmg);
					else
						BlowUpMissile(bul[i].x,bul[i].y,bul[i].radius,bul[i].dmg);
					bul[i].type=BL_MISBOOM;
					bul[i].dmg=15;
				}
				if(bul[i].x<-SPR_SIZE || bul[i].x>SCR_WID+SPR_SIZE || bul[i].y<-SPR_SIZE || bul[i].y>SCR_HEI+SPR_SIZE)
					bul[i].type=0;
				break;
			case BL_MISBOOM:
			case BL_ACIDBOOM:
			case BL_DISCHARGE:
				bul[i].dmg--;
				bul[i].radius*=0.8f;
				if(!bul[i].dmg)
					bul[i].type=0;
				break;
			case BL_LASER:
				bul[i].radius-=1.0f;
				if(bul[i].radius<=0)
					bul[i].type=0;
				break;
		}
	}
}

void RenderBullets(void)
{
	int i,w;

	for(i=0;i<MAX_BULLETS;i++)
	{
		switch(bul[i].type)
		{
			case BL_GUN:
				w=bul[i].dmg-3;
				if(w<0)
					w=0;
				systemStub->drawRect(bul[i].x-w,bul[i].y,bul[i].x+w+2,bul[i].y+10+w,1,1,1,1);
				break;
			case BL_ACID:
				systemStub->drawRect(bul[i].x-1,bul[i].y,bul[i].x+2,bul[i].y+10,1,1,1,1);
				systemStub->drawRect(bul[i].x-1-3,bul[i].y+3,bul[i].x+2-3,bul[i].y+10+3,0,1,0,1);
				systemStub->drawRect(bul[i].x-1+3,bul[i].y+3,bul[i].x+2+3,bul[i].y+10+3,0,1,0,1);
				break;
			case BL_MISSILE:
				w=bul[i].dmg/2;
				if(w<0)
					w=0;
				systemStub->drawRect(bul[i].x-w,bul[i].y-w,bul[i].x+w+2,bul[i].y+w+2,1,1,1,1);
				break;
			case BL_MISBOOM:
				systemStub->drawRect(bul[i].x-bul[i].radius,bul[i].y-bul[i].radius,bul[i].x+bul[i].radius,bul[i].y+bul[i].radius,1,0,0,0.6f);
				break;
			case BL_DISCHARGE:
				systemStub->drawRect(bul[i].x-bul[i].radius,bul[i].y-bul[i].radius,bul[i].x+bul[i].radius,bul[i].y+bul[i].radius,1,1,0,0.6f);
				break;
			case BL_ACIDBOOM:
				systemStub->drawRect(bul[i].x-bul[i].radius,bul[i].y-bul[i].radius,bul[i].x+bul[i].radius,bul[i].y+bul[i].radius,0,1,0,0.6f);
				break;
			case BL_LASER:
				systemStub->drawLine(bul[i].x-2,bul[i].y,bul[i].dx,bul[i].dy,0,1,1,0.4f);
				systemStub->drawLine(bul[i].x+2,bul[i].y,bul[i].dx,bul[i].dy,0,1,1,0.4f);
				systemStub->drawLine(bul[i].x,bul[i].y,bul[i].dx,bul[i].dy,0,1,1,0.8f);
				break;
			case BL_FREEZER:
				systemStub->drawRect(bul[i].x-5,bul[i].y-1,bul[i].x+6,bul[i].y+2,0.3f,1,1,1);
				break;
		}
	}
}