#include "invader.h"
#include "sprites.h"
#include "player.h"
#include "tower.h"
#include <math.h>
#include "sound.h"
#include "bullet.h"
#include <stdlib.h>
#include "systemstub.h"

invader_t inv[MAX_INVADERS];
static int downClock;
static float allDx,allDy,nextDx,nextDy;

void ClearInvaders(void)
{
	int i;

	for(i=0;i<MAX_INVADERS;i++)
		inv[i].life=0;
}

void InvaderWave(byte num,int life,float spd,float spacing)
{
	int i,b;
	float x,y;

	ClearInvaders();

	x=SPR_SIZE;
	y=-(num/((SCR_WID-SPR_SIZE*3)/spacing))*SPR_SIZE;

	b=0;
	for(i=0;i<num;i++)
	{
		inv[i].life=life;
		inv[i].maxLife=life;
		inv[i].freeze=0;
		inv[i].x=x;
		inv[i].y=y;
		inv[i].anim=((i+b)%2)*32;
		inv[i].acid=0;
		x+=spacing;
		if(x>=SCR_WID-SPR_SIZE*2)
		{
			x=SPR_SIZE;
			y+=SPR_SIZE;
			b=1-b;
		}
	}
	allDx=spd;
	allDy=SPR_SIZE/30.0f;
	downClock=30;
}

void RenderInvader(invader_t *me)
{
	float r;
	byte sp;

	r=((float)me->life/(float)me->maxLife);
	sp=0;
	if((player.level%4)==0)
		sp=SPR_INV2;
	else if((player.level%4)==1)
		sp=0;
	else if((player.level%4)==2)
		sp=SPR_INV3;
	else if((player.level%4)==3)
		sp=SPR_INV4;

	if(me->acid)
	{
		if(me->freeze)
			systemStub->drawSpriteColored(sp+(me->anim>=32),me->x,me->y,r,1,1,1);
		else
			systemStub->drawSpriteColored(sp+(me->anim>=32),me->x,me->y,r,1,r,1);
	}
	else if(me->freeze)
		systemStub->drawSpriteColored(sp+(me->anim>=32),me->x,me->y,r,r,1,1);
	else
		systemStub->drawSpriteColored(sp+(me->anim>=32),me->x,me->y,1,r,r,1);
}

void RenderInvaders(void)
{
	int i;

	for(i=0;i<MAX_INVADERS;i++)
		if(inv[i].life)
			RenderInvader(&inv[i]);
}

void SetMovement(float dx,float dy)
{
	nextDx=dx;
	nextDy=dy;

	if(dy>0)
	{
		if(player.level/6<=28)
			downClock=30-(player.level/6);
		else
			downClock=2;
		if((player.level%4)==2)
		{
			if(downClock>3)
				downClock/=2;
		}
		nextDy=SPR_SIZE/(float)(downClock-1);
	}
}

void KillSomebody(void)
{
	if(!SaveLife())
	{
		if(player.life)
			player.life--;
		Sound(SND_LAND);
	}
	else
		Sound(SND_SAVE);
}

void UpdateInvader(invader_t *me)
{
	int n;

	if(me->freeze)
	{
		me->freeze--;
		if(allDy==0)
			me->x+=allDx/2.0f;
		else
			me->y+=allDy/2.0f;
		n=(abs((int)(allDx*3.0f)));
		if(n>31)
			n=31;
		me->anim+=n;
		if(me->anim>=64)
			me->anim=0;
	}
	else
	{
		if(allDy==0)
			me->x+=allDx;
		else
			me->y+=allDy;
		n=(abs((int)(allDx*6.0f)));
		if(n>31)
			n=31;
		me->anim+=n;
		if(me->anim>=64)
			me->anim=0;
	}
	if(me->acid)
	{
		if(player.research[RS_CORRODE] && (me->acid%5)==0)
		{
			me->life--;
			if(me->life<=0)
			{
				me->life=0;
				allDx*=1.05f;
				if(allDx>6.0f)
					allDx=6.0f;
				nextDx=allDx;
				Sound(SND_BOOM);
			}
		}
		me->acid--;
	}

	if(me->x>SCR_WID-SPR_SIZE*1.5f && allDx>0 && nextDx>0)
	{
		SetMovement(-allDx,SPR_SIZE/30);
	}
	else if(me->x<SPR_SIZE/2 && allDx<0 && nextDx<0)
	{
		SetMovement(-allDx,SPR_SIZE/30);
	}
	if(me->y>TOWER_Y-SPR_SIZE*2)
	{
		me->life=0;
		if((player.level%4)==0)
		{
			KillSomebody();
			KillSomebody();
		}
		else if((player.level%4)==3)
		{
			if(rand()%2)
				KillSomebody();
		}
		else
			KillSomebody();
	}
}

void UpdateInvaders(void)
{
	int i;

	nextDx=allDx;
	nextDy=allDy;
	if(downClock)
	{
		downClock--;
		if(!downClock)
		{
			allDy=0;
			nextDy=0;
		}
	}

	for(i=0;i<MAX_INVADERS;i++)
		if(inv[i].life)
			UpdateInvader(&inv[i]);

	allDx=nextDx;
	allDy=nextDy;
}

void HurtInvader(int i,word dmg)
{
	if(inv[i].acid)
		dmg*=2;
	if(player.research[RS_PIERCE])
		dmg=(dmg*4)/3;
	inv[i].life-=dmg;
	if(inv[i].life<=0)
	{
		inv[i].life=0;
		allDx*=1.05f;
		if(allDx>6.0f)
			allDx=6.0f;
		nextDx=allDx;
		Sound(SND_BOOM);
	}
}

byte HitInvaders(float x,float y,byte dmg)
{
	int i;

	for(i=0;i<MAX_INVADERS;i++)
	{
		if(inv[i].life && x>inv[i].x && y>inv[i].y && x<inv[i].x+SPR_SIZE && y<inv[i].y+SPR_SIZE)
		{
			HurtInvader(i,dmg);
			return 1;
		}
	}
	return 0;
}

byte HitInvadersFrz(float x,float y,byte dmg,word frz)
{
	int i;

	for(i=0;i<MAX_INVADERS;i++)
	{
		if(inv[i].life && inv[i].freeze==0 && x>inv[i].x && y>inv[i].y && x<inv[i].x+SPR_SIZE && y<inv[i].y+SPR_SIZE)
		{
			HurtInvader(i,dmg);
			if(inv[i].life)
				inv[i].freeze=frz;
			return 1;
		}
	}
	return 0;
}

void AcidSplash(float x,float y,float radius,word acid)
{
	float dist,xx,yy;
	int i;

	for(i=0;i<MAX_INVADERS;i++)
	{
		if(inv[i].life)
		{
			xx=inv[i].x+SPR_SIZE/2;
			yy=inv[i].y+SPR_SIZE/2;
			dist=(float)((xx-x)*(xx-x)+(yy-y)*(yy-y));
			if(dist<radius*radius)
			{
				inv[i].acid=acid;
			}
		}
	}
}

byte HitInvadersAcid(float x,float y,byte dmg,byte acid,float radius)
{
	int i;

	for(i=0;i<MAX_INVADERS;i++)
	{
		if(inv[i].life && x>inv[i].x && y>inv[i].y && x<inv[i].x+SPR_SIZE && y<inv[i].y+SPR_SIZE)
		{
			HurtInvader(i,dmg);
			inv[i].acid=acid;
			AcidSplash(x,y,radius,acid);
			return 1;
		}
	}
	return 0;
}

byte NoInvaders(void)
{
	int i;

	for(i=0;i<MAX_INVADERS;i++)
	{
		if(inv[i].life)
			return 0;
	}
	return 1;
}

word FindTarget(float x,float range,float speed)
{
	int i;
	float tx;

	for(i=0;i<MAX_INVADERS;i++)
	{
		if(inv[i].life && inv[i].freeze==0)
		{
			tx=inv[i].x+SPR_SIZE/2;
			if(tx>(x-range-SPR_SIZE/2) && tx<(x+range+SPR_SIZE/2))
				return (word)(i+1);
		}
	}
	return 0;
}

byte PullMissile(float x,float y,float *dx,float *dy)
{
	int i,dist;

	for(i=0;i<MAX_INVADERS;i++)
	{
		if(inv[i].life)
		{
			if(inv[i].x+SPR_SIZE/2<x)
				(*dx)-=0.2f;
			if(inv[i].x+SPR_SIZE/2>x)
				(*dx)+=0.2f;
			if(inv[i].y+SPR_SIZE/2<y)
				(*dy)-=0.2f;
			if(inv[i].y+SPR_SIZE/2>y)
				(*dy)+=0.2f;

			if((*dx)>6.0f)
				(*dx)=6.0f;
			if((*dx)<-6.0f)
				(*dx)=-6.0f;
			if((*dy)>6.0f)
				(*dy)=6.0f;
			if((*dy)<-6.0f)
				(*dy)=-6.0f;
			if((*dx)==0)
				(*dx)=0.1f;
			
			dist=abs((int)(inv[i].x+SPR_SIZE/2-x))+abs((int)(inv[i].y+SPR_SIZE/2-y));
			if(dist<SPR_SIZE)
				return 1;
		}
	}
	return 0;
}

void BlowUpMissile(float x,float y,float radius,word damage)
{
	float dist,xx,yy;
	int i;

	for(i=0;i<MAX_INVADERS;i++)
	{
		if(inv[i].life)
		{
			xx=inv[i].x+SPR_SIZE/2;
			yy=inv[i].y+SPR_SIZE/2;
			dist=(float)((xx-x)*(xx-x)+(yy-y)*(yy-y));
			if(dist<radius*radius)
			{
				HurtInvader(i,damage);
				if(player.research[RS_GRAVITY] && inv[i].life)
				{
					dist=(x-inv[i].x)/2.0f;
					inv[i].x+=dist;
					dist=(y-inv[i].y)/2.0f;
					if(dist<0)
						inv[i].y+=dist;
				}
			}
		}
	}
}

void InvaderPos(word w,float *x,float *y)
{
	if(inv[w].life)
	{
		(*x)=inv[w].x+SPR_SIZE/2;
		(*y)=inv[w].y+SPR_SIZE/2;
	}
}

void LaserGuys(float x,float y,float x2,float y2,word dmg,word frz)
{
	int i,j;
	float tx,ty,dx,dy,xx,yy,dist;

	tx=x;
	ty=y;
	dx=(x2-x)/320.0f;
	dy=(y2-y)/320.0f;

	for(i=0;i<MAX_INVADERS;i++)
	{
		if(inv[i].life)
		{
			xx=inv[i].x+SPR_SIZE/2;
			yy=inv[i].y+SPR_SIZE/2;
				
			for(j=0;j<320;j++)
			{
				tx=x+dx*(float)j;
				ty=y+dy*(float)j;

				dist=(float)((xx-tx)*(xx-tx)+(yy-ty)*(yy-ty));
				if(dist<SPR_SIZE*SPR_SIZE)
				{
					HurtInvader(i,dmg);
					inv[i].freeze=frz;
					if(player.research[RS_FREEZE])
					{
						AddBullet(BL_FREEZER,inv[i].x+SPR_SIZE/2,inv[i].y+SPR_SIZE/2,-8,0,1,(float)frz);
						AddBullet(BL_FREEZER,inv[i].x+SPR_SIZE/2,inv[i].y+SPR_SIZE/2,8,0,1,(float)frz);
					}
					break;
				}
			}
		}
	}
}