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

void Stage_4_a()
{
	switch( boss ) {
		case 0:
			switch( time + 200 ) {
				case 430:
					Enemy_Generate( 19, 128, 60 );	break;
				case 450:
					Enemy_Generate( 19, 128, 10 );	break;
				case 470:
					Enemy_Generate( 19, 128, 50 );	break;
				case 490:
					Enemy_Generate( 19, 128, 30 );	break;
				case 510:
					Enemy_Generate( 19, 128, 20 );	break;
				case 530:
					Enemy_Generate( 19, 128, 70 );	break;
				case 550:
					Enemy_Generate( 19, 128, 40 );	break;
				case 590:
					Enemy_Generate( 21, 128, 60 );	break;
				case 610:
					Enemy_Generate( 21, 128, 10 );	break;
				case 630:
					Enemy_Generate( 21, 128, 50 );	break;
				case 650:
					Enemy_Generate( 21, 128, 30 );	break;
				case 670:
					Enemy_Generate( 21, 128, 20 );	break;
				case 690:
					Enemy_Generate( 21, 128, 70 );	break;
				case 710:
					Enemy_Generate( 21, 128, 40 );	break;
				
				case 800:
					Enemy_Generate( 16, 128, 12 );
					Enemy_Generate( 16, 128, 32 );	break;
				case 900:
					Enemy_Generate( 15, 128, 64 );
					Enemy_Generate( 15, 128, 44 );	break;
				
				case 1000:	case 1190:
					Enemy_Generate( 22, 128, 16 );	break;
				case 1010:	case 1180:
					Enemy_Generate( 22, 128, 38 );	break;
				case 1020:	case 1170:
					Enemy_Generate( 22, 128, 60 );	break;
				
				case 1070:
					Enemy_Generate( 8, 128, 68 );	break;
				case 1080:
					Enemy_Generate( 8, 128, 44 );	break;
				case 1090:
					Enemy_Generate( 8, 128, 20 );
					Enemy_Generate( 13, 80, -9 );	break;
				case 1100:
					Enemy_Generate( 8, 128, 56 );
					Enemy_Generate( 14, 80, 88 );	break;
				case 1110:
					Enemy_Generate( 8, 128,  8 );
					Enemy_Generate( 13, 80, -9 );	break;
				case 1120:
					Enemy_Generate( 8, 128, 32 );
					Enemy_Generate( 14, 80, 88 );	break;
				
				case 1240:
					Enemy_Generate( 18, 128, 20 );
					Enemy_Generate( 18, 128, 38 );
					Enemy_Generate( 18, 128, 56 );	break;
				
				case 1400:	case 1420:	case 1440:	case 1460:
					Enemy_Generate( 23, 96, -10 );	break;
				case 1500:	case 1520:	case 1540:	case 1560:
					Enemy_Generate( 24, 96, 88 );	break;
				
				case 1700:
					Enemy_Generate( 25, 128, 12 );
					Enemy_Generate( 25, 128, 38 );
					Enemy_Generate( 25, 128, 64 );	break;
					

				case 2150:
					stage++;	Stage_Init();	break;
			}
			break;
	}
}


void Stage_4_b()
{
int i, j=0, k=0;

	switch( boss ) {
		case 0:
			Enemy_Generate( 20, 128, 30 );
			en[0].range = 15;	en[0].angle = 180;	en[0].power = 800;
			boss++;
			muteki = TRUE;
			count = 60 * 40;
			break;
		
		case 1:
			if( en[0].angle == 180 ) {
				if( en[0].x > 84 * 10000 ) { break; }
				else {
					en[0].time = 0;
					en[0].angle = 90;
					en[0].range = 0;
				}
			}
			if( muteki ) {
				if( en[0].time == 80 ) {
					muteki = FALSE;
					en[0].time = 0;
				}
				else				   { break; }
			}
			if( en[0].time % 15 == 0 ) {
				j = rand()%20 - 10;
				switch( en[0].time % 60 ) {
					case 15:	j +=  45;	break;
					case 30:	j +=  90;	break;
					case 45:	j += 135;	break;
				}
				for( i = 0; i <= 180; i += 180 ) {
					Eshot_Generate( 12, 10, 12, 35,     j+i, 0 );
					Eshot_Generate( 12, 10, 12, 33,  10+j+i, 0 );
					Eshot_Generate( 12, 10, 12, 30,  20+j+i, 0 );
					Eshot_Generate( 12, 10, 12, 26,  40+j+i, 0 );
					Eshot_Generate( 12, 10, 12, 23,  60+j+i, 0 );
					Eshot_Generate( 12, 10, 12, 22,  80+j+i, 0 );
					Eshot_Generate( 12, 10, 12, 22, 100+j+i, 0 );
					Eshot_Generate( 12, 10, 12, 23, 120+j+i, 0 );
					Eshot_Generate( 12, 10, 12, 26, 140+j+i, 0 );
					Eshot_Generate( 12, 10, 12, 30, 160+j+i, 0 );
					Eshot_Generate( 12, 10, 12, 33, 170+j+i, 0 );
				}
			}
			count--;
			if( en[0].power <= 0 ) {
				for( i = -60; i <= 60; i += 30 ) {
					Item_Generate( 0,  i, 0 );
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
				en[0].power = 1;
				pceWaveDataOut( 2, &CR );
			}
			break;
		
		case 2:
			en[0].power = 1000;
			count--;
			if( count <= 0 ) {
				boss++;
				muteki =FALSE;
				count = 60*40;
				en[0].time = 0;
			}
			break;
		
		case 3:
			if( en[0].time % 4 == 0 ) {
				i = en[0].time % 45 * 2;
				for( j = i; j <= i + 360; j += 90 ) {
					Eshot_Generate( 12, 10, 12, 30, j, 0 );
				}
				i = 90 - en[0].time % 45 * 2;
				for( j = i; j <= i + 360; j += 90 ) {
					Eshot_Generate( 12, 10, 12, 30, j, 0 );
				}
			}
			count--;
			if( en[0].power <= 0 ) {
				for( i = -60; i <= 60; i += 30 ) {
					Item_Generate( 0,  i, 0 );
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
				en[0].power = 1;
				pceWaveDataOut( 2, &CR );
			}
			break;
		
		case 4:
			en[0].power = 1000;
			count--;
			if( count <= 0 ) {
				boss++;
				muteki = FALSE;
				count = 60*40;
				en[0].time = 0;
			}
			break;
		
		case 5:
			if( en[0].time % 8 == 4 ) {
				i = 12;	j = rand()%30+165;
				switch( en[0].time % 32 ) {
					case  4:	i -= 36;	j += 30;	break;
					case 12:	i += 36;	j -= 30;	break;
					case 20:	i -= 16;	j += 15;	break;
					case 28:	i += 16;	j -= 15;	break;
				}
				for( k = 3; k <= 6; k++ ) {
					Eshot_Generate( k, 4, i, 15, j, 0 );
				}
			}
			count--;
			if( en[0].power <= 0 ) {
				for( i = -60; i <= 60; i += 30 ) {
					Item_Generate( 0,  i, 0 );
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
				en[0].power = 1;
				pceWaveDataOut( 2, &CR );
			}
			break;
		
		case 6:
			en[0].power = 1000;
			count--;
			if( count <= 0 ) {
				boss++;
				muteki =FALSE;
				count = 60*40;
				en[0].time = 0;
			}
			break;
		
		case 7:
			switch( en[0].time % 160 ) {
				case  0:	case 10:	case 20:	case 30:
				case 40:	case 50:	case 60:	case 70:
					j = (en[0].time/10)%5 * 4;
					for( i = j+20; i < j+380; i += 20 ) {
						Eshot_Generate( 8, 10, 12, 25, i, 0 );
					}
					break;
				case 80:	case 90:	case 100:	case 110:
				case 120:	case 130:	case 140:	case 150:
					j = (en[0].time/8)%5 * 4;
					for( i = j+21; i < j+381; i += 20 ) {
						Eshot_Generate( 8, 10, 12, 25, i, 0 );
					}
					break;
			}
			count--;
			if( en[0].power <= 0 ) {
				for( i = -60; i <= 60; i += 30 ) {
					Item_Generate( 0,  i, 0 );
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
				en[0].angle = 0;	en[0].range = 10;	en[0].power = 1;
				pceWaveDataOut( 2, &CR );
			}
			break;
		
		case 8:
			if( en[0].x >= 92*10000 ) {
				en[0].range = 0;
				en[0].power = 1600;
			}
			count--;
			if( count <= 0 ) {
				boss++;
				muteki =FALSE;
				count = 90*40;
				en[0].time = 0;
			}
			break;
		
		case 9:
			if( en[0].time % 20 == 10 ) {
				j = rand()%10 - 5;
				switch( en[0].time % 80 ) {
					case 10:	j += 210;	break;
					case 30:	j +=  30;	break;
					case 50:	j += 300;	break;
					case 70:	j += 120;	break;
				}
				Eshot_Generate( 15, 10, 12, 15, j, 0 );
				Eshot_Generate( 14, 10, 12, 15, j+10, 0 );
				Eshot_Generate( 13, 10, 12, 15, j+20, 0 );
				Eshot_Generate( 14, 10, 12, 15, j+30, 0 );
				Eshot_Generate( 15, 10, 12, 15, j+40, 0 );
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
				en[0].angle = -90;	en[0].range = 0;	en[0].power = 1;
			}
			break;
		
		case 10:
		if( time % 8 == 0 && time < 100 ) {
			pceWaveDataOut( 2, &CR );
			en[0].range ++;
		}
		if( time == 100 ) { Eshot_Clear(); }
		if( time > 250 ) { FadeOut = 100; }
	}


}



