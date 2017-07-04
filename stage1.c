#include <piece.h>


typedef struct { int rx, ry, x, y; int range, angle; int anim; int type; int time; int power; } CHARA_DATA;

extern void Stage_Init();
extern void Enemy_Generate( int type, int x, int y );
extern int Eshot_SnipeAngle( int type, int x, int y, int enemy );
extern void Eshot_Generate( int type, int x, int y, int range, int angle, int enemy );
extern void Eshot_Clear();
extern void Item_Generate( int type, int angle, int enemy );
extern void Score_Add( int point );
extern int time, boss, count, stage, score, bonus;
extern BOOL muteki;
extern CHARA_DATA en[0];
extern PCEWAVEINFO CR;
extern int FadeOut;

void Stage_1_a()
{
	switch( boss ) {
		case 0:
			switch( time ) {
				case 220:	case 230:	case 240:
					Enemy_Generate( 2, 128, 16 );	break;
				case 260:	case 2320:
					Enemy_Generate( 0, 128, 16 );	break;
				case 360:	case 370:	case 380:
					Enemy_Generate( 2, 128, 60 );	break;
				case 400:	case 2330:	case 2400:
					Enemy_Generate( 0, 128, 60 );	break;
				case 500:	case 510:	case 520:
					Enemy_Generate( 2, 128, 38 );	break;
				case 540:	case 2410:
					Enemy_Generate( 0, 128, 38 );	break;
				
				case 640:
					Enemy_Generate( 1, 128, 16 );	break;
				case 650:
					Enemy_Generate( 1, 128, 12 );	break;
				case 660:
					Enemy_Generate( 1, 128,  8 );	break;
				case 670:
					Enemy_Generate( 1, 128,  4 );	break;
					
				case 760:
					Enemy_Generate( 1, 128, 64 );	break;
				case 770:
					Enemy_Generate( 1, 128, 68 );	break;
				case 780:
					Enemy_Generate( 1, 128, 72 );	break;
				case 790:
					Enemy_Generate( 1, 128, 76 );	break;
				
				case 900:
					Enemy_Generate( 3, 128, 14 );
					Enemy_Generate( 3, 128, 30 );
					Enemy_Generate( 3, 128, 46 );
					Enemy_Generate( 3, 128, 62 );	break;
				
				case 1020:	case 1100:
					Enemy_Generate( 0, 128, 14 );	break;
				case  980:	case 1140:
					Enemy_Generate( 0, 128, 30 );	break;
				case 1040:	case 1080:
					Enemy_Generate( 0, 128, 46 );	break;
				case 1000:	case 1120:
					Enemy_Generate( 0, 128, 62 );	break;
				
				case 1250:
					Enemy_Generate( 4, 128, 36 );
					en[0].range = 15;	en[0].angle = 180;	en[0].power = 700;
					boss = 1;
					count = 30 * 40;
					break;
				
				case 2030:
					Enemy_Generate( 3, 128, 20 );	break;
				case 2070:
					Enemy_Generate( 3, 128, 56 );	break;
				
				case 2180:	case 2300:	case 2360:
					Enemy_Generate( 5, 128, 68 );	break;
				case 2190:	case 2260:	case 2340:
					Enemy_Generate( 5, 128, 44 );	break;
				case 2200:	case 2280:	case 2390:
					Enemy_Generate( 5, 128, 20 );	break;
				case 2210:	case 2310:	case 2350:
					Enemy_Generate( 5, 128, 56 );	break;
				case 2220:	case 2270:	case 2380:
					Enemy_Generate( 5, 128,  8 );	break;
				case 2230:	case 2290:	case 2370:
					Enemy_Generate( 5, 128, 32 );	break;
				
				case 2520:
					Enemy_Generate( 6, 128, 50 );	break;
				case 2550:
					Enemy_Generate( 6, 128, 26 );	break;
				
				case 2600:
					Enemy_Generate( 3, 128, 14 );
					Enemy_Generate( 6, 128, 38 );
					Enemy_Generate( 3, 128, 62 );	break;

				case 2750:
					stage++;	Stage_Init();	break;
			}
			break;
		case 1:
			if( en[0].angle == 180 ) {
				if( en[0].x > 96 * 10000 ) { break; }
				else {
					en[0].time = 0;
					en[0].angle = 90;
				}
			}
			switch( en[0].time % 165 ) {
				case 0:
					en[0].range = 0;	break;
				case 20:	case 25:	case 30:
				case 40:	case 45:	case 50:
				case 60:	case 65:	case 70:
				case 80:	case 85:	case 90:
					Eshot_Generate( 0, -5, 6, rand()%10+15, rand()%40+160, 0 );	break;
				case 35:	case 55:	case 75:	case 95:
					Eshot_Generate( 0, -5, 6, rand()%10+15, Eshot_SnipeAngle( 0, -5, 6, 0 ), 0 );	break;
				case 135:
					en[0].range = 10;	break;
			}
			if( en[0].time % 660 == 165 || en[0].time % 660 == 495 ) {
				en[0].angle = -en[0].angle;
			}
			count--;
			if( en[0].power <= 0 ) {
				Item_Generate( 2, 0, 0 );
				Score_Add( count * 10 );
				bonus = count * 10;
				count = 0;
			}
			if( !count ) {
				boss = 0;	muteki = TRUE;
				Eshot_Clear();
				en[0].power = 1;	en[0].range = 10;	en[0].angle = 0;
				time = 2000;
				pceWaveDataOut( 2, &CR );
			}
			break;
	}
}

void Stage_1_b()
{
	int i;

	switch( boss ) {
		case 0:
			Enemy_Generate( 4, 128, 36 );
			en[0].range = 15;	en[0].angle = 180;	en[0].power = 600;
			boss++;
			muteki = TRUE;
			count = 30 * 40;
			break;
		
		case 1:
			if( en[0].angle == 180 ) {
				if( en[0].x > 96 * 10000 ) { break; }
				else {
					en[0].time = 0;
					en[0].angle = 90;
					en[0].range = 0;
				}
			}
			if( muteki ) {
				if( en[0].time == 40 ) {
					muteki = FALSE;
					en[0].range = 5;
					en[0].time = 0;
				}
				else				   { break; }
			}
			switch( en[0].time % 20 ) {
				case 5:
					for( i = 164; i <= 196; i += 16 ) {
						Eshot_Generate( 0, -7, 3, rand()%10+15, i, 0 );
					}
					break;
				case 15:
					for( i = 172; i <= 188; i += 16 ) {
						Eshot_Generate( 0, -7, 3, rand()%10+15, i, 0 );
					}
					break;
			}
			if( en[0].time % 20 == 10 ) { en[0].angle = -en[0].angle; }
			count--;
			if( en[0].power <= 0 ) {
				for( i = -60; i <= 60; i += 30 ) {
					Item_Generate( 0,  i, 0 );
				}
				Score_Add( count * 10 );
				bonus = count * 10;
				count = 0;
			}
			if( count <= 0 ) {
				boss++;
				muteki = TRUE;
				Eshot_Clear();
				count = 2*40;
				en[0].angle = 90;	en[0].range = 10;	en[0].power = 1;
				pceWaveDataOut( 2, &CR );
			}
			break;
		
		case 2:
			if( en[0].y <= 8*10000 ) {
				en[0].range = 0;
				en[0].power = 700;
			}
			count--;
			if( count <= 0 ) {
				boss++;
				muteki =FALSE;
				count = 30*40;
				en[0].time = 0;	en[0].angle = -90;	en[0].range = 10;
			}
			break;
		
		case 3:
			switch( en[0].time % 80 ) {
				case  4:	case  8:	case 12:	case 20:	case 24:
				case 28:	case 36:	case 40:	case 44:	case 52:
				case 56:	case 60:
					Eshot_Generate( 0, -5, 6, rand()%10+15, rand()%40+160, 0 );
					break;
				case 16:	case 32:	case 48:
					Eshot_Generate( 0, -5, 6, rand()%10+15, Eshot_SnipeAngle( 0, -5, 6, 0 ), 0 );	break;
				case 61:
					en[0].range = 0;	break;
				case 79:
					en[0].range = 10;	en[0].angle = -en[0].angle;	break;
			}
			count--;
			if( en[0].power <= 0 ) {
				for( i = -60; i <= 60; i += 30 ) {
					Item_Generate( 0,  i, 0 );
				}
				Score_Add( count * 10 );
				bonus = count * 10;
				count = 0;
			}
			if( count <= 0 ) {
				boss++;
				muteki = TRUE;
				Eshot_Clear();
				count = 2*40;
				en[0].range = 10;	en[0].power = 1;
				if( en[0].y < 36*10000 ) { en[0].angle = -90; }
				else					 { en[0].angle =  90; }
				pceWaveDataOut( 2, &CR );
			}
			break;
		
		case 4:
			if( en[0].y >= 35*10000 && en[0].y <= 37*10000 ) {
				en[0].range = 0;
				en[0].power = 800;
			}
			count--;
			if( count <= 0 ) {
				boss++;
				muteki =FALSE;
				count = 30*40;
				en[0].time = 0;
			}
			break;
		
		case 5:
			if( en[0].time % 4 == 0 ) {
					Eshot_Generate( 0, -5, 6, rand()%10+15, Eshot_SnipeAngle( 0, -5, 6, 0 )+rand()%60-30, 0 );
			}
			count--;
			if( en[0].power <= 0 ) {
				Score_Add( count * 10 );
				bonus = count * 10;
				count = 0;
			}
			if( count <= 0 ) {
				boss++;
				time = 0;
				muteki = TRUE;
				Score_Add( 10000 );
				bonus += 10000;
				en[0].angle = -90;	en[0].range = 5;	en[0].power = 1;
			}
			break;
		
		case 6:
		if( time % 5 == 0 && time < 50 ) {
			pceWaveDataOut( 2, &CR );
			en[0].range ++;
		}
		if( time == 50 ) { Eshot_Clear(); }
		if( time > 200 ) { FadeOut = 80; }
	}


}
