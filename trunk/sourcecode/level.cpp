#include "level.h"
#include "player.h"
#include "invader.h"


void NextLevel(void)
{
	float spd;
	int cnt;
	word life;
	float space;

	if(NoInvaders() && player.life>0)
	{
		player.level++;
		if(player.money+player.level/2<=160)
			player.money+=player.level/2;
		else
			player.money=160;
		cnt=player.level*2;
		spd=0.20f+(float)player.level*0.04f;
		life=3+(word)((float)player.level*1.1f);
		if(player.level>50)
			life=(word)((float)life*((float)player.level)/50.0f);

		space=48;
		switch(player.level%4)
		{
			case 0:	// 'boss' - quarter as many guys with double HP
				life*=10;
				cnt/=10;
				space*=3;	// widen them out
				spd*=0.8f;
				break;
			case 1:	// normal - normal guys, normal speed, normal HP
				break;
			case 2:	// speed - half as many guys, move twice as fast, half HP
				life/=2;
				spd*=1.5f;
				space=48;	// tighten them up
				cnt/=2;
				break;
			case 3:	// horde - 3/4 speed, twice as many guys with half HP
				life/=2;
				spd*=0.75f;
				space=36;	// tighten up
				cnt*=2;
				break;
			
		}
		if(cnt>255)
			cnt=255;
		InvaderWave(cnt,life,spd,space);
	}
}
