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

void Stage_2_a()
{
int i, j;

	switch( boss ) {
		case 0:
			switch( time ) {
				case 240:
					Enemy_Generate( 7, 128, 10 );	break;
				case 255:
					Enemy_Generate( 7, 128, 66 );	break;
				case 270:	case 2360:
					Enemy_Generate( 7, 128, 36 );	break;
				case 285:	case 2490:
					Enemy_Generate( 7, 128, 52 );	break;
				case 300:	case 2380:
					Enemy_Generate( 7, 128, 24 );	break;
				case 315:	case 2470:
					Enemy_Generate( 7, 128, 40 );	break;
				
				case 400:	case 510:	case 550:
				case 2100:	case 2290:	case 2320:
					Enemy_Generate( 8, 128, 68 );	break;
				case 410:	case 470:	case 530:
				case 2110:	case 2250:	case 2330:
					Enemy_Generate( 8, 128, 44 );	break;
				case 420:	case 490:	case 580:
				case 2120:	case 2270:	case 2450:
					Enemy_Generate( 8, 128, 20 );	break;
				case 430:	case 520:	case 540:
				case 2130:	case 2300:	case 2340:
					Enemy_Generate( 8, 128, 56 );	break;
				case 440:	case 480:	case 570:
				case 2140:	case 2260:	case 2440:
					Enemy_Generate( 8, 128,  8 );	break;
				case 450:	case 500:	case 560:
				case 2150:	case 2280:	case 2430:
					Enemy_Generate( 8, 128, 32 );	break;
				
				case 650:
					Enemy_Generate( 1, 128, 16 );	break;
				case 660:
					Enemy_Generate( 1, 128, 64 );	break;
				case 670:
					Enemy_Generate( 1, 128,  8 );	break;
				case 680:
					Enemy_Generate( 1, 128, 72 );	break;
				
				case 740:
					Enemy_Generate( 7, 128,  8 );
					Enemy_Generate( 7, 128, 68 );	break;
				case 770:
					Enemy_Generate( 7, 128, 20 );
					Enemy_Generate( 7, 128, 56 );	break;
				case 800:
					Enemy_Generate( 7, 128, 32 );
					Enemy_Generate( 7, 128, 44 );	break;
				
				case 850:	case 860:	case 870:	case 880:
					Enemy_Generate( 2, 128, 38 );	break;
				
				case 900:	case 2550:
					Enemy_Generate( 9, 128, 20 );
					Enemy_Generate( 9, 128, 56 );	break;
				case 940:
					Enemy_Generate( 5, 128, 44 );	break;
				case 960:	case 1040:
					Enemy_Generate( 5, 128, 20 );	break;
				case 980:	case 1020:
					Enemy_Generate( 5, 128, 56 );	break;
				case 1000:
					Enemy_Generate( 5, 128, 32 );	break;
				
				case 1100:	case 1110:
					Enemy_Generate( 2, 128, 16 );	break;
				case 1130:	case 2180:	case 2400:
					Enemy_Generate( 10, 128, 16 );	break;
				case 1180:	case 1190:
					Enemy_Generate( 2, 128, 60 );	break;
				case 1210:	case 2200:	case 2510:
					Enemy_Generate( 10, 128, 60 );	break;
				
				case 1350:
					Enemy_Generate( 11, 128, 36 );
					en[0].range = 15;	en[0].angle = 180;	en[0].power = 700;
					boss = 1;
					count = 30 * 40;
					break;
				
				
				case 2050:
					Enemy_Generate( 10, 128, 14 );
					Enemy_Generate( 10, 128, 30 );
					Enemy_Generate( 10, 128, 46 );
					Enemy_Generate( 10, 128, 62 );	break;
					
				case 2590:
					Enemy_Generate( 8, 128, 44 );	break;
				case 2610:	case 2690:
					Enemy_Generate( 8, 128, 20 );	break;
				case 2630:	case 2670:
					Enemy_Generate( 8, 128, 56 );	break;
				case 2650:
					Enemy_Generate( 8, 128, 32 );	break;
				
				case 2900:
					stage++;	Stage_Init();	break;
			}
			break;
		case 1:
			if( en[0].angle == 180 ) {
				if( en[0].x > 96 * 10000 ) { break; }
				else {
					en[0].range = 5;
					en[0].angle = 90;
					en[0].time = 0;
				}
			}
			switch( en[0].time % 140 ) {
				case 10:	case 30:	case 50:
					j = Eshot_SnipeAngle( 1, -8, 6, 0 );
					for( i = 25; i <= 30; i += 5 ) {
						Eshot_Generate( 1, -8, 6, i, j-15, 0 );
						Eshot_Generate( 1, -8, 6, i, j, 0 );
						Eshot_Generate( 1, -8, 6, i, j+15, 0 );
					}
					break;
				case 80:	case 84:	case 88:
				case 96:	case 100:	case 104:
					j = Eshot_SnipeAngle( 1, -8, 6, 0 )+rand()%40-20;
					for( i = 25; i <= 30; i += 5 ) {
						Eshot_Generate( 1, -8, 6, i, j, 0 );
					}
					break;
				case 92:	case 108:
					j = Eshot_SnipeAngle( 1, -8, 6, 0 );
					for( i = 25; i <= 30; i += 5 ) {
						Eshot_Generate( 1, -8, 6, i, j, 0 );
					}
					break;
			}
			if( en[0].time % 20 == 10 ) { en[0].angle = -en[0].angle; }
			count--;
			if( en[0].power <= 0 ) {
				Item_Generate( 1, 0, 0 );
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


void Stage_2_b()
{
int i, j;

	switch( boss ) {
		case 0:
			Enemy_Generate( 11, 128, 60 );
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
			if( en[0].time % 20 == 0 ) {
				for( i = 21; i <= 33; i += 3 ) {
					Eshot_Generate( 1, -8, 6, i, Eshot_SnipeAngle( 1, -8, 6, 0 ), 0 );
				}
			}
			if( en[0].time % 100 == 99 ) { en[0].angle = -en[0].angle; }
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
				en[0].power = 600;
			}
			count--;
			if( count <= 0 ) {
				boss++;
				muteki =FALSE;
				count = 30*40;
				en[0].time = 0;	en[0].angle = -90;	en[0].range = 0;
			}
			break;
		
		case 3:
			switch( en[0].time % 200 ) {
				case 10:
					for( i = 190; i <= 260; i += 35 ) {
						for( j = 3; j <= 6; j++ ) {
							Eshot_Generate( j, -8, 6, 15, i, 0 );
						}
					}
					en[0].angle = -90;	en[0].range = 15;	break;
				case 30:	case 80:	case 130:	case 180:
					en[0].range = 0;	break;
				case 60:	case 160:
					for( i = 145; i <= 215;	i += 35 ) {
						for( j = 3; j <= 6; j++ ) {
							Eshot_Generate( j, -8, 6, 15, i, 0 );
						}
					}
					en[0].range = 15;	break;
				case 110:
					for( i = 100; i <= 170; i += 35 ) {
						for( j = 3; j <= 6; j++ ) {
							Eshot_Generate( j, -8, 6, 15, i, 0 );
						}
					}
					en[0].angle = 90;	en[0].range = 15;	break;
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
				en[0].angle = -90;	en[0].range = 10;	en[0].power = 1;
				pceWaveDataOut( 2, &CR );
			}
			break;
		
		case 4:
			if( en[0].y >= 64*10000 ) {
				en[0].range = 0;	en[0].angle = 90;	en[0].power = 600;
			}
			count--;
			if( count <= 0 ) {
				boss++;
				muteki = FALSE;
				count = 30*40;
				en[0].time = 0;
			}
			break;
		
		case 5:
			switch( en[0].time % 80 ) {
				case 1:
					en[0].range = 10;	break;
				case  5:	case 20:	case 35:	case 50:
					j =  Eshot_SnipeAngle( 1, -8, 6, 0 );
					for( i = j-8; i <= j+8; i += 8 ) {
						Eshot_Generate( 1, -8, 6, 25, i, 0 );
					}
					for( i = j- 4; i <= j+ 4; i += 8 ) {
						Eshot_Generate( 1, -8, 6, 20, i, 0 );
					}
					Eshot_Generate( 1, -8, 6, 15, j, 0 );
					break;
				case 60:
					en[0].range = 0;	en[0].angle = -en[0].angle;	break;
			}
			count--;
			if( en[0].power <= 0 ) {
				for( i = -60; i <= 60; i += 30 ) {
					Item_Generate( 0, i, 0 );
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
			switch( en[0].time % 170 ) {
				case  4:	case  8:	case 12:	case 16:	case 20:
				case 24:	case 28:	case 32:	case 36:	case 40:
				case 44:	case 48:	case 52:	case 56:	case 60:
				case 64:
					Eshot_Generate( 0, -5, 6, rand()%5+5, Eshot_SnipeAngle( 0, -5, 6, 0 )+rand()%90-45, 0 );	break;
				case 83:
					en[0].angle = 72;	en[0].range = 20;	break;
				case 104:
					en[0].angle = -90;	en[0].range = 40;	break;
				case 106:	case 110:	case 114:	case 118:	case 122:
					for( i = 30; i <= 42; i += 3 ) {
						Eshot_Generate( 1, -8, 6, i, 180, 0 );
					}
					break;
				case 124:
					en[0].angle = 108;	en[0].range = 20;	break;
				case 145:
					en[0].range = 0;	break;
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



