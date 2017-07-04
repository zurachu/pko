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

void Stage_3_a()
{
int i;

	switch( boss ) {
		case 0:
			switch( time ) {
				case 270:
					Enemy_Generate( 12, 128, 36 );
					en[0].range = 15;	en[0].angle = 180;	en[0].power = 800;
					boss = 1;
					count = 30 * 40;
					break;
				
				case 1050:	case 1060:	case 1070:	case 1080:	case 1090:
					Enemy_Generate( 13, 64, -9 );	break;
				case 1130:	case 1140:	case 1150:	case 1160:	case 1170:
					Enemy_Generate( 14, 80, 88 );	break;
				
				case 1240:
					Enemy_Generate( 15, 128, 44 );	break;
				case 1280:
					Enemy_Generate( 16, 128, 32 );	break;
				
				case 1330:
					Enemy_Generate( 6, 128, 16 );	break;
				case 1340:
					Enemy_Generate( 3, 128, 32 );	break;
				case 1350:
					Enemy_Generate( 0, 128, 48 );	break;
				case 1420:
					Enemy_Generate( 6, 128, 60 );	break;
				case 1430:
					Enemy_Generate( 3, 128, 44 );	break;
				case 1440:
					Enemy_Generate( 0, 128, 28 );	break;
				
				case 1500:
					Enemy_Generate( 17, 128, 16 );	break;
				case 1530:
					Enemy_Generate( 17, 128, 36 );	break;
				case 1600:
					Enemy_Generate( 17, 128, 60 );	break;
				case 1630:
					Enemy_Generate( 17, 128, 40 );	break;
				
				case 1800:
					Enemy_Generate( 12, 128, 36 );
					en[0].range = 15;	en[0].angle = 180;	en[0].power = 700;
					boss = 3;
					count = 30 * 40;
					break;
				
				case 2080:	case 2090:	case 2100:	case 2110:
					Enemy_Generate( 13, 80, -9 );
					Enemy_Generate( 14, 80, 88 );	break;
					
				case 2140:
					Enemy_Generate( 17, 128, 20 );
					Enemy_Generate( 17, 128, 56 );	break;
				
				case 2200:
					Enemy_Generate( 8, 128, 68 );	break;
				case 2210:
					Enemy_Generate( 8, 128, 44 );	break;
				case 2220:
					Enemy_Generate( 8, 128, 20 );
					Enemy_Generate( 13, 80, -9 );	break;
				case 2230:
					Enemy_Generate( 8, 128, 56 );
					Enemy_Generate( 14, 80, 88 );	break;
				case 2240:
					Enemy_Generate( 8, 128,  8 );
					Enemy_Generate( 13, 80, -9 );	break;
				case 2250:
					Enemy_Generate( 8, 128, 32 );
					Enemy_Generate( 14, 80, 88 );	break;
				
				case 2300:
					Enemy_Generate( 16, 128, 16 );
					Enemy_Generate( 15, 128, 60 );	break;
				
				case 2400:
					Enemy_Generate( 18, 128, 16 );
					Enemy_Generate( 18, 128, 38 );
					Enemy_Generate( 18, 128, 60 );	break;
				
				case 2430:
					Enemy_Generate( 6, 128, 24 );	break;
				case 2440:
					Enemy_Generate( 6, 128, 52 );	break;
				case 2460:
					Enemy_Generate( 6, 128, 12 );	break;
				case 2470:
					Enemy_Generate( 6, 128, 64 );	break;
				
				case 2650:
					stage++;	Stage_Init();	break;
			}
			break;
		
		case 1:
			if( en[0].angle == 180 ) {
				if( en[0].x > 96 * 10000 ) { break; }
				else {
					en[0].range = 0;
					en[0].angle = 90;
					en[0].time = 0;
				}
			}
			switch( en[0].time % 380 ) {
				case 20:	case 210:
					for( i = 22; i < 360; i += 45 ) {
						Eshot_Generate( 7, 3, 6, 12, i, 0 );
					}
					break;
				case 70:
					en[0].angle = -75;	en[0].range = 10;	break;
				case 0:	case 95:	case 190:	case 285:
					en[0].range = 0;	break;
				case 115:	case 305:
					for( i = 22; i < 360; i += 45 ) {
						Eshot_Generate( 7, 3, 6, 10, i, 0 );
					}
					break;
				case 165:
					en[0].angle = 105;	en[0].range = 10;	break;
				case 260:
					en[0].angle = 75;	en[0].range = 10;	break;
				case 355:
					en[0].angle = -105;	en[0].range = 10;	break;
			}
			count--;
			if( en[0].power <= 0 ) {
				Item_Generate( 1, 0, 0 );
				Score_Add( count * 20 );
				bonus = count * 20;
				count = 0;
			}
			if( !count ) {
				boss = 0;	muteki = TRUE;
				Eshot_Clear();
				en[0].power = 1;	en[0].range = 10;	en[0].angle = 0;
				time = 1000;
				pceWaveDataOut( 2, &CR );
			}
			break;
		
		case 3:
			if( en[0].angle == 180 ) {
				if( en[0].x > 96 * 10000 ) { break; }
				else {
					en[0].range = 5;
					en[0].angle = 90;
					en[0].time = 0;
				}
			}
			if( en[0].time % 40 == 20 ) {
				for( i = 12; i < 372; i += 24 ) {
					Eshot_Generate( 8, 3, 6, 22, i, 0 );
					Eshot_Generate( 8, 3, 6, 22, i+1, 0 );
				}
				en[0].angle = -en[0].angle;
			}
			count--;
			if( en[0].power <= 0 ) {
				for( i = -60; i <= 60; i += 30 ) {
					Item_Generate( 0,  i, 0 );
				}
				Score_Add( count * 20 );
				bonus = count * 20;
				count = 0;
			}
			if( !count ) {
				boss++;
				muteki = TRUE;
				Eshot_Clear();
				count = 2*40;
				en[0].angle = 90;	en[0].range = 10;	en[0].power = 1;
				pceWaveDataOut( 2, &CR );
			}
			break;
		
		case 4:
			if( en[0].y <= 8*10000 ) {
				en[0].range = 0;
				en[0].power = 500;
			}
			count--;
			if( count <= 0 ) {
				boss++;
				muteki =FALSE;
				count = 30*40;
				en[0].time = 0;	en[0].angle = -90;	en[0].range = 15;
			}
			break;
		
		case 5:
			switch( en[0].time % 60 ) {
				case  4:	case 10:	case 16:
				case 22:	case 28:	case 34:
					i = Eshot_SnipeAngle( 12, -5, 6, 0 );
					Eshot_Generate(  9, -3, 5, 35, i, 0 );
					Eshot_Generate( 10, -4, 5, 30, i, 0 );
					Eshot_Generate( 11, -4, 6, 25, i, 0 );
					Eshot_Generate( 12, -5, 6, 20, i, 0 );	break;
				case 38:
					en[0].range = 0;	break;
				case 0:
					en[0].range = 15;	en[0].angle = -en[0].angle;	break;
			}
			count--;
			if( en[0].power <= 0 ) {
				Item_Generate( 2, 0, 0 );
				Score_Add( count * 10 );
				bonus = count * 10;
				count = 0;
			}
			if( count <= 0 ) {
				boss = 0;	muteki = TRUE;
				Eshot_Clear();
				en[0].power = 1;	en[0].range = 10;	en[0].angle = 0;
				time = 2000;
				pceWaveDataOut( 2, &CR );
			}
			break;
	}
}


void Stage_3_b()
{
int i, j;

	switch( boss ) {
		case 0:
			Enemy_Generate( 12, 128, 36 );
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
					en[0].time = 0;
				}
				else				   { break; }
			}
			switch( en[0].time % 20 ) {
				case 0:
					Eshot_Generate( 9, -3, 5, rand()%10+10, Eshot_SnipeAngle( 9, -3, 5, 0 )+rand()%80-40, 0 );	break;
				case 5:
					Eshot_Generate( 10, -4, 5, rand()%10+10, Eshot_SnipeAngle( 9, -3, 5, 0 )+rand()%80-40, 0 );	break;
				case 10:
					Eshot_Generate( 11, -4, 6, rand()%10+10, Eshot_SnipeAngle( 9, -3, 5, 0 )+rand()%80-40, 0 );	break;
				case 15:
					for( i = 0; i < 2; i++ ) {
						Eshot_Generate( 12, -5, 6, rand()%10+10, Eshot_SnipeAngle( 9, -3, 5, 0 )+rand()%100-50, 0 );
					}
					break;
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
				en[0].power = 1;
				pceWaveDataOut( 2, &CR );
			}
			break;
		
		case 2:
			en[0].power = 800;
			count--;
			if( count <= 0 ) {
				boss++;
				muteki =FALSE;
				count = 30*40;
				en[0].time = 0;	en[0].angle = 90;	en[0].range = 5;
			}
			break;
		
		case 3:
			if( en[0].time % 20 == 0 ) {
				j = rand()% 24;
				for( i = j; i < 360+j; i += 24 ) {
					Eshot_Generate( 8, 3, 6, 22, i, 0 );
				}
			}
			if( en[0].time % 40 == 20 ) { en[0].angle = -en[0].angle; }
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
		
		case 4:
			if( en[0].y < 8*10000 ) {
				en[0].range = 0;	en[0].angle = -90;	en[0].power = 900;
			}
			count--;
			if( count <= 0 ) {
				boss++;
				muteki = FALSE;
				count = 60*40;
				en[0].range = 10;
				en[0].time = 0;
			}
			break;
		
		case 5:
			switch( en[0].time % 80 ) {
				case 10:	case 23:	case 37:	case 50:
					i = rand()%40-20;
					switch( en[0].time % 80 ) {
						case 10:	i += 135;	break;
						case 23:	i +=  20;	break;
						case 37:	i -=  20;	break;
						case 50:	i -= 135;	break;
					}
					Eshot_Generate( 13, 1, 5, 28, i, 0 );
					Eshot_Generate( 14, 2, 5, 25, i, 0 );
					Eshot_Generate( 15, 2, 6, 22, i, 0 );
					Eshot_Generate( 16, 3, 6, 19, i, 0 );	break;
				case 59:
					en[0].range = 0;	break;
				case 79:
					en[0].range = 10;	en[0].angle = -en[0].angle;	break;
			}
			count--;
			if( en[0].power <= 0 ) {
				for( i = -60; i <= 60; i += 30 ) {
					Item_Generate( 0, i, 0 );
				}
				Score_Add( count * 6 );
				bonus = count * 6;
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
		
		case 6:
			if( en[0].y >= 35*10000 && en[0].y <= 37*10000 ) {
				en[0].range = 0;	en[0].power = 1000;
			}
			count--;
			if( count <= 0 ) {
				boss++;
				muteki =FALSE;
				count = 60*40;
				en[0].time = 0;
			}
			break;
		
		case 7:
			switch( en[0].time % 20 ) {
				case 0:
					Eshot_Generate( 13, 1, 5, rand()%10+10, rand()%90-45, 0 );
					break;
				case 5:
					Eshot_Generate( 14, 2, 5, rand()%10+10, rand()%90-45, 0 );
					break;
				case 10:
					Eshot_Generate( 15, 2, 6, rand()%10+10, rand()%90-45, 0 );
					break;
				case 15:
					for( i = 0; i < 2; i++ ) {
						Eshot_Generate( 16, 3, 6, rand()%10+10, rand()%90+135, 0 );
					}
					break;
			}
			count--;
			if( en[0].power <= 0 ) {
				Score_Add( count * 8 );
				bonus = count * 8;
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
		
		case 8:
		if( time % 5 == 0 && time < 50 ) {
			pceWaveDataOut( 2, &CR );
			en[0].range ++;
		}
		if( time == 50 ) { Eshot_Clear(); }
		if( time > 200 ) { FadeOut = 80; }
	}


}



