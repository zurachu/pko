#include <piece.h>
#include "gamelib.h"

extern int Get_Sin( int index );
extern int Get_Cos( int index );
extern int Get_ArcTan( int x, int y );

extern void Stage_1_a();
extern void Stage_1_b();
extern void Stage_2_a();
extern void Stage_2_b();
extern void Stage_3_a();
extern void Stage_3_b();
extern void Stage_4_a();
extern void Stage_4_b();

void Stage_Init();
void Player_Init( int flag );
void Player_Action();
void Pshot_Generate( int type, int x, int y, int range, int angle );
void Pshot_Action();
void Enemy_Generate( int type, int x, int y );
void Enemy_Action();
int Eshot_SnipeAngle( int type, int x, int y, int enemy );
void Eshot_Generate( int type, int x, int y, int range, int angle, int enemy );
void Eshot_Action();
void Eshot_Clear();
void Item_Generate( int type, int angle, int enemy );
void Item_Action();
void Score_Add( int point );
void Life_Down();
void Game_Graphics();

extern unsigned char *arc;
extern int FadeOut;
extern unsigned short GameMode;
#define GM_TITLE	2
#define GM_HISCORE2	128

unsigned char *PLAYER, *PSHOT, *ENEMY, *ESHOT, *ITEM, *BOMB, *LASER;
PIECE_BMP PL, PS, EN, ES, IT, BM, LS;
unsigned char *BGM;
unsigned char *WEAPON, *ITEMGET, *POWERUP, *DOWN, *CRASH, *EFIRE, *BFIRE, *EXTEND;
PCEWAVEINFO WP, IG, PU, DN, CR, EF, BF, EX;

typedef struct {
	int rx, ry, x, y;	// 実空間座標、仮想空間座標
	int range, angle;	// 移動ベクトルの大きさ、角度
	int anim;			// アニメーション管理（ホーミング弾のターゲット（仮））
	int type;			// 自機のウェポン、弾、敵の種類
	int time;			// 動作管理（自機復活時、弾、敵）
	int power;			// パワー
} CHARA_DATA;

CHARA_DATA pl, ps[64], en[64], es[64], it[64];

const int pshit[7][2] = { {7,6}, {7,2}, {7,5}, {4,4}, {6,6} };
const int psgrp[9] = { 0, 1, 2, 3, 3, 4, 4, 2, 2 };
const int pspow[9] = { 9, 4, 3, 3, 3, 5, 5, 4, 2 };
const int enhit[7][2] = { { 16, 12 }, { 12, 16 }, { 9, 9 }, { 9, 16 }, { 12, 16 }, { 9, 10 }, { 24, 29 } };
const int engrp[26] = { 0, 0, 0, 0, 1, 2, 0, 0, 2, 0, 0, 3, 4, 2, 2, 0, 0, 0, 0, 5, 6, 5, 5, 5, 5, 5 };
const int eshit[7][2] = { { 5, 5 }, { 8, 5 }, { 8, 8 }, { 7, 7 }, { 6, 6 }, { 5, 5 }, { 6, 6 } };
const int esgrp[19] = { 0, 1, 0, 1, 1, 1, 1, 5, 5, 2, 3, 4, 5, 2, 3, 4, 5, 6, 6 };
const int ithit[3] = { 8, 12, 12 };

int stage;
int time;
int chara;
BOOL gameover, pause, kurai;
int triger, subtriger, bombtime;
int life, cont, score, pastscore, level, bomb, bonus;
int itemChain, crashChain;

int boss, count;
BOOL muteki;

int cursor;

void Graphic_Init()
{
	PSHOT = ppack_findPackData( arc, "pshot.pgd" );
	Get_PieceBmp( &PS, PSHOT );
	ENEMY = ppack_findPackData( arc, "enemy.pgd" );
	Get_PieceBmp( &EN, ENEMY );
	ESHOT = ppack_findPackData( arc, "eshot.pgd" );
	Get_PieceBmp( &ES, ESHOT );
	ITEM = ppack_findPackData( arc, "item.pgd" );
	Get_PieceBmp( &IT, ITEM );
	if( !chara ) {
		LASER = ppack_findPackData( arc, "laser.pgd" );
		Get_PieceBmp( &LS, LASER );
		BOMB = ppack_findPackData( arc, "uruto.pgd" );
	}
	else		 { BOMB = ppack_findPackData( arc, "kamyu.pgd" ); }
	Get_PieceBmp( &BM, BOMB );
	
	POWERUP = ppack_findPackData( arc, "powerup.ppd" );
	Get_PieceWave( &PU, POWERUP );
	DOWN = ppack_findPackData( arc, "down.ppd" );
	Get_PieceWave( &DN, DOWN );
	CRASH = ppack_findPackData( arc, "crash.ppd" );
	Get_PieceWave( &CR, CRASH );
	EFIRE = ppack_findPackData( arc, "eshot.ppd" );
	Get_PieceWave( &EF, EFIRE );
	BFIRE = ppack_findPackData( arc, "bomb.ppd" );
	Get_PieceWave( &BF, BFIRE );
	EXTEND = ppack_findPackData( arc, "extend.ppd" );
	Get_PieceWave( &EX, EXTEND );
}


void Graphic_Free()
{
	StopMusic();
	pceWaveAbort( 0 );

	pceHeapFree( PSHOT );
	pceHeapFree( ENEMY );
	pceHeapFree( ESHOT );
	pceHeapFree( ITEM );
	pceHeapFree( BOMB );
	pceHeapFree( LASER );
	
	pceHeapFree( BGM );
	
	pceHeapFree( POWERUP );
	pceHeapFree( DOWN );
	pceHeapFree( CRASH );
	pceHeapFree( EFIRE );
	pceHeapFree( BFIRE );
	pceHeapFree( EXTEND );
}


void Game_Init()
{
	life = 4;
	Player_Init( 1 );
	cont = 0;
	pl.type = 0;
	gameover = pause = FALSE;
	score = pastscore = level = 0;
	bomb = 3;
	if( stage >= 2 ) {
		cont = 1;
		level = 15;
	}
	
	if( !chara )	{ pl.range = 20; }	// ウルトリィ
	else			{ pl.range = 25; }	// カミュ
	Graphic_Init();
}


void Stage_Init()
{
	int i;

	time = 0;
	boss = 0;
	muteki = FALSE;
	pceWaveAbort( 0 );
	pceHeapFree( BGM );
	switch( stage ) {
		case 1:
			BGM = ppack_findPackData( arc, "01.pmd" );
			break;
		case 2:
			BGM = ppack_findPackData( arc, "11.pmd" );
			break;
		case 3:
			BGM = ppack_findPackData( arc, "15.pmd" );
			break;
		case 4:
			BGM = ppack_findPackData( arc, "10.pmd" );
			break;
		case 5:
			BGM = ppack_findPackData( arc, "17.pmd" );
			break;
		case 6:
			BGM = ppack_findPackData( arc, "19.pmd" );
			break;
		case 7:
			BGM = ppack_findPackData( arc, "22.pmd" );
			break;
		case 8:
			BGM = ppack_findPackData( arc, "lastboss.pmd" );
			break;
	}
	PlayMusic( BGM );
	if( stage % 2 ) {
		for( i = 0; i < 64; i++ ) {
			ps[i].power = en[i].power = es[i].power = it[i].power = 0;
		}
		Player_Init( 0 );
	}
}


void Player_Init( int flag )
{
	if( flag ) {
		life--;
		if( !life ) {
			if( cont >= 3 ) {
				Graphic_Free();
				FadeOut = 30;
				GameMode = GM_HISCORE2;
			}
			else {
				life = 1;
				gameover = TRUE;
				cursor = 0;
			}
		}
		else {
			if( level >= 10 && level < 15 ) { level  = 10; }
			if( level >= 15 && level < 20 ) { level -=  5; }
			if( level >= 20 && level < 25 ) { level =  14; }
			if( level >= 25 && level < 40 ) { level -= 10; }
			if( level >= 40 ) { level = 29; }
			bomb = 3;
			itemChain = crashChain = 1;
		}
	}
	pl.x = -16 * 10000;
	pl.y =  38 * 10000;
	pl.anim = 0;
	pl.time = 120;
	triger = subtriger = 0;
	bombtime = 0;
	kurai = FALSE;
}


void Game_Main()
{
	int pad = pcePadGet();
	
	if( gameover ) {
		if( pad & ( TRG_UP | TRG_DN ) ) {
			cursor++;
			if( cursor > 1 ) { cursor = 0; }
			LCDUpdate = TRUE;
			pceWaveDataOut( 3, &IG );
		}
		if( pad & TRG_A ) {
			if( cursor == 0 ) {
				life = 3;
				Player_Init( 0 );
				cont++;
				gameover = FALSE;
				score = pastscore = 0;
				bomb = 3;
				level = 45;
				pceWaveDataOut( 3, &EX );
			}
			else {
				Graphic_Free();
				pceWaveDataOut( 3, &WP );
				FadeOut = 30;
				GameMode = GM_HISCORE2;
			}
		}
	
	}
	else {
		// ポーズ、ポーズ解除
		if( pad & TRG_C ) {
			pause = !pause;
			cursor = 0;
			pceWaveDataOut( 3, &WP );
		}
		if( pause ) {
			if( pad & ( TRG_UP | TRG_DN ) ) {
				cursor++;
				if( cursor > 1 ) { cursor = 0; }
				LCDUpdate = TRUE;
				pceWaveDataOut( 3, &IG );
			}
			if( pad & TRG_A ) {
				if( cursor == 0 ) { pause = FALSE; }
				else {
					Graphic_Free();
					pceWaveDataOut( 3, &WP );
					FadeOut = 30;
					GameMode = GM_TITLE;
				}
			}
		}
		else {
			time++;
			switch( stage ) {
				case 1:
					Stage_1_a();	break;
				case 2:
					Stage_1_b();	break;
				case 3:
					Stage_2_a();	break;
				case 4:
					Stage_2_b();	break;
				case 5:
					Stage_3_a();	break;
				case 6:
					Stage_3_b();	break;
				case 7:
					Stage_4_a();	break;
				case 8:
					Stage_4_b();	break;
			}
			
			Player_Action();
			Pshot_Action();
			Enemy_Action();
			Eshot_Action();
			Item_Action();
		}
	}
	
	Game_Graphics();
}


void Player_Action()
{
	int pad = pcePadGet();
	
	// 弾発射
	if( triger ) { triger--; }
	else {
		if( pad & PAD_A && ( pl.time <= 80 || ( chara && bombtime ) ) ) {
			if( !chara ) {	// ウルトリィ
				Pshot_Generate( 0, 13, 3, 30, 0 );
				triger = 8;
				if( !pl.type ) {	// 集中型
					if( ( level >= 15 && level < 30 ) || ( ( ( level >= 30 && level < 45 ) || level < 15 ) && !subtriger ) ) {
						Pshot_Generate( 4, 12, -3, 30, -15 );
						Pshot_Generate( 4, 12, 12, 30,  15 );
					}
					if( ( level >= 45 ) || ( level >= 30 && subtriger ) ) {
						Pshot_Generate( 6, 12, -4, 30, -15 );
						Pshot_Generate( 6, 12, 11, 30,  15 );
					}
				}
				else {	// ホーミング
					if( ( level >= 15 && level < 30 ) || ( ( ( level >= 30 && level < 45 ) || level < 15 ) && !subtriger ) ) {
						Pshot_Generate( 3, 10, -5, 30, 0 );
						Pshot_Generate( 3, 10, 13, 30, 0 );
					}
					if( ( level >= 45 ) || ( level >= 30 && subtriger ) ) {
						Pshot_Generate( 5, 10, -6, 30, 0 );
						Pshot_Generate( 5, 10, 12, 30, 0 );
					}
				}
			}
			else {			// カミュ
				Pshot_Generate( 1, 13, 3, 30, 0 );
				Pshot_Generate( 1, 13, 7, 30, 0 );
				triger = 6;
				if( !pl.type ) {	// 集中型
					if( level >= 15 || !subtriger ) {
						Pshot_Generate( 2, 10, -2, 30, 0 );
						Pshot_Generate( 2, 10,  9, 30, 0 );
					}
					if( level >= 45 || ( level >= 30 && !subtriger ) ) {
						Pshot_Generate( 8, 7, -5, 30, 0 );
						Pshot_Generate( 8, 7, 12, 30, 0 );
					}
				}
				else {	// 拡散型
					if( level >= 15 || !subtriger ) {
						Pshot_Generate( 7, 10, -3, 30, 15 );
						Pshot_Generate( 7, 10, 10, 30, -15 );
					}
					if( level >= 45 || ( level >= 30 && !subtriger ) ) {
						Pshot_Generate( 7, 8, -5, 30, 30 );
						Pshot_Generate( 7, 8, 12, 30, -30 );
					}
				}
			}
			if( !subtriger ) { subtriger = 2; }
			subtriger--;
		}
	}
	
	// ボム発動
	if( pad & TRG_B && pl.time <= 60 && bomb ) {
		bomb--;
		bombtime = 80;
		pl.time = 160;
		kurai = FALSE;	// 喰らいボム
		pceWaveDataOut( 2, &BF );
	}
	if( bombtime ) { bombtime--; }
	
	// サブウェポン変更
	if( pad & TRG_D ) {
		pl.type++;
		if( pl.type > 1 ) { pl.type = 0; }
		pceWaveDataOut( 3, &WP );
	}
	
	// アニメーション
	pl.anim++;
	if( pl.anim >= 20 ) { pl.anim = 0; }
	
	if( pl.time ) {
		pl.time--;
		if( pl.time <= 0 && kurai ) {
			Player_Init( 1 );
		}
	}
	if( pl.time > 80 && bombtime <= 0 ) {	// 自機移動（復活時）
		pl.x += 10000;
	}
	else {					// 自機移動
		if( pad & ( PAD_UP | PAD_DN | PAD_LF | PAD_RI ) ) {
			if( pad & PAD_UP ) { pl.angle =  90; }
			if( pad & PAD_DN ) { pl.angle = -90; }
			if( pad & PAD_LF ) {
				pl.angle = 180;
				if( pad & PAD_UP ) { pl.angle = 135; }
				if( pad & PAD_DN ) { pl.angle = 225; }
			}
			if( pad & PAD_RI ) {
				pl.angle =  0;
				if( pad & PAD_UP ) { pl.angle =  45; }
				if( pad & PAD_DN ) { pl.angle = -45; }
			}
			if( bombtime ) { pl.range += 4; }	// ボム処理落ち対策ここから
			pl.x += pl.range * Get_Cos( pl.angle );
			pl.y -= pl.range * Get_Sin( pl.angle );
			if( bombtime ) { pl.range -= 4; }	// ボム処理落ち対策ここまで
			if( pl.x <           0 ) { pl.x =           0; }
			if( pl.x > 112 * 10000 ) { pl.x = 112 * 10000; }
			if( pl.y <           0 ) { pl.y =           0; }
			if( pl.y >  76 * 10000 ) { pl.y =  76 * 10000; }
		}
	}
	pl.rx = pl.x / 10000;
	pl.ry = pl.y / 10000;
}


void Pshot_Generate( int type, int x, int y, int range, int angle )
{
	int i = 0, j, k, close = 128;
	
	while( ps[i].power ) { i++; }
	if( i >= 64 ) { return; }
	
	ps[i].type = type;
	ps[i].x = pl.x + x * 10000;
	ps[i].y = pl.y + y * 10000;
	ps[i].rx = ps[i].x / 10000;
	ps[i].ry = ps[i].y / 10000;
	ps[i].range = range;
	ps[i].angle = angle;
	ps[i].power = pspow[type];
	
	if( type == 3 || type == 5 ) {	// ホーミング
		ps[i].anim = -1;
		for( j = 0; j < 64; j++ ) {
			if( en[j].power ) {
				k = (en[j].rx+enhit[engrp[en[j].type]][0]/2) - (ps[i].rx+pshit[type][0]/2);
				if( k >= 0 && k < close ) {
					close = k;
					ps[i].anim = j;
				}
			}
		}
	}
	if( type == 4 || type == 6 ) {	// ウルトリィ集中型
		if( angle < 0 ) { ps[i].anim = -15; }
		else			{ ps[i].anim =  15; }
	}
}


void Pshot_Action()
{
	int i, j, psx, psy, enx, eny ;
	
	for( i = 0; i < 64; i++ ) {
		// ホーミング
		if( ( ps[i].type == 3 || ps[i].type == 5 ) && ps[i].anim >= 0 ) {
			if( ( !muteki || ps[i].anim ) && en[ps[i].anim].power ) {
				psx = ps[i].rx + pshit[ps[i].type][0] / 2;
				psy = ps[i].ry + pshit[ps[i].type][1] / 2;
				enx = en[ps[i].anim].rx + enhit[engrp[en[ps[i].anim].type]][0] / 2;
				eny = en[ps[i].anim].ry + enhit[engrp[en[ps[i].anim].type]][1] / 2;
				j = Get_ArcTan( enx - psx, psy - eny ) - ps[i].angle;
				while( j >  180 ) { j -= 360; }
				while( j < -180 ) { j += 360; }
				if( j >   0 ) { ps[i].angle +=  3; }
				if( j >  30 ) { ps[i].angle += 30; }
				if( j <   0 ) { ps[i].angle -=  3; }
				if( j < -30 ) { ps[i].angle -= 30; }
			}
		}
		if( ps[i].type == 4 || ps[i].type == 6 ) {
			ps[i].angle += ps[i].anim;
			if( ps[i].angle >= 45 || ps[i].angle <= -45 ) {
				ps[i].anim = -ps[i].anim;
			}
		}
		
		if( ps[i].power ) {
			ps[i].x += ps[i].range * Get_Cos( ps[i].angle );
			ps[i].y -= ps[i].range * Get_Sin( ps[i].angle );
			if( ps[i].x < -16 * 10000 || ps[i].x > 128 * 10000 || \
				ps[i].y < -16 * 10000 || ps[i].y >  96 * 10000 ) {
				ps[i].power = 0;
			}
			ps[i].rx = ps[i].x / 10000;
			ps[i].ry = ps[i].y / 10000;
		}
	}
}


void Enemy_Generate( int type, int x, int y )
{
	int i = 0;

	while( en[i].power ) {
		i++;
		if( i >= 64 ) { return; }
	}
	
	en[i].x = x * 10000;		en[i].y = y * 10000;
	en[i].rx = en[i].x / 10000;	en[i].ry = en[i].y / 10000;
	en[i].type = type;
	switch( type ) {
		case 0:	case 1:
			en[i].range = 20;	en[i].angle = 180;	en[i].power = 22;
			break;
			break;
		case 2:
			en[i].range = 20;	en[i].angle = 180;	en[i].power = 15;
			break;
		case 3:	case 6:	case 15:	case 16:	case 17:	case 18:
			en[i].range = 18;	en[i].angle = 180;	en[i].power = 30;
			break;
		case 5:	case 8:
			en[i].range = 22;	en[i].angle = 180;	en[i].power = 5;
			break;
		case 7:	case 10:
			en[i].range = 18;	en[i].angle = 180;	en[i].power = 20;
			break;
		case 9:
			en[i].range = 18;	en[i].angle = 180;	en[i].power = 50;
			break;
		case 13:
			en[i].range = 22;	en[i].angle = -90;	en[i].power = 5;
			break;
		case 14:
			en[i].range = 22;	en[i].angle =  90;	en[i].power = 5;
			break;
		case 19:	case 22:
			en[i].range = 15;	en[i].angle = 180;	en[i].power = 22;
			break;
		case 21:
			en[i].range = 20;	en[i].angle = 180;	en[i].power = 22;
			break;
		case 23:
			en[i].range = 15;	en[i].angle = -90;	en[i].power = 22;
			break;
		case 24:
			en[i].range = 15;	en[i].angle =  90;	en[i].power = 22;
			break;
		case 25:
			en[i].range = 20;	en[i].angle = 180;	en[i].power = 1000;
	}
	en[i].time  = 0;
}


void Enemy_Action()
{
	int i, j, k;
	
	for( i = 0; i < 64; i++ ) {
		if( en[i].power ) {
			// アニメーション
			en[i].anim++;
			if( en[i].anim >= 20 ) { en[i].anim = 0; }
			
			en[i].time++;
			switch( en[i].type ) {
				case 0:
					if( en[i].time == 10 ) { en[i].range = 0; }
					if( en[i].time == 20 ) {
						Eshot_Generate( 0, -7, 3, 30, Eshot_SnipeAngle( 0, -7, 3, i ), i ); }
					if( en[i].time == 30 ) { en[i].range = 18; }
					break;
				case 1:
					if( en[i].time < 20 ) {
						if( en[i].ry < 38 ) { en[i].angle += 3; }
						else				{ en[i].angle -= 3; }
					}
					if( en[i].time >= 30 && en[i].time < 45 ) { 
						if( en[i].angle >= 180 ) { en[i].angle -= 3; }
						else					 { en[i].angle += 3; }
					}
					if( en[i].time == 10 || en[i].time == 30 ) {
						if( en[i].ry < 38 ) {
							Eshot_Generate( 0, -7, 3, 20, 175, i );
							Eshot_Generate( 0, -7, 3, 20, 205, i );
						}
						else {
							Eshot_Generate( 0, -7, 3, 20, 195, i );
							Eshot_Generate( 0, -7, 3, 20, 165, i );
						}
					}
					break;
				case 3:
					if( en[i].time == 12 ) { en[i].range = 0; }
					if( en[i].time == 24 ) {
						j = Eshot_SnipeAngle( 0, -7, 3, i );
						for( k = j-16; k <= j+16; k += 16 ) {
							Eshot_Generate( 0, -7, 3, 20, k, i );
							Eshot_Generate( 0, -7, 3, 15, k, i );
						}
					}
					if( en[i].time == 36 ) {
						en[i].range = 16;
						if( en[i].ry < 38 ) { en[i].angle = 150; }
						else				{ en[i].angle = 210; }
					}
					break;
				
				case 6:
					if( en[i].time == 12 ) { en[i].range = 0; }
					if( en[i].time == 24 ) {
						j = Eshot_SnipeAngle( 0, -7, 3, i );
						for( k = j-15; k <= j+15; k += 10 ) {
							Eshot_Generate( 0, -7, 3, 20, k, i );
							Eshot_Generate( 0, -7, 3, 25, k, i );
						}
					}
					if( en[i].time == 36 ) {
						en[i].range = 16;
						if( en[i].ry < 38 ) { en[i].angle = 150; }
						else				{ en[i].angle = 210; }
					}
					break;
				
				case 7:
					if( en[i].time == 12 ) { en[i].range = 0; }
					if( en[i].time == 24 ) {
						j = Eshot_SnipeAngle( 1, -10, 3, i );
						for( k = 18; k <= 30; k += 3 ) {
							Eshot_Generate( 1, -10, 3, k, j, i );
						}
					}
					if( en[i].time == 36 ) {
						en[i].range = 16;
						if( en[i].ry < 38 ) { en[i].angle = 150; }
						else				{ en[i].angle = 210; }
					}
					break;
				
				case 8:
					if( en[i].time == 10 ) {
						for( j = 0; j < 2; j++ ) {
							Eshot_Generate( 0, 3, 3, 12, rand()%90+135, i );
						}
					}
					break;
				
				case 9:
					if( en[i].time == 12 ) { en[i].range = 0; }
					if( en[i].time % 24 == 0 && en[i].time < 132 ) {
						j = rand() % 20;
						for( k = 90; k < 270; k += 20 ) {
							Eshot_Generate( 0, -7, 3, 20, j+k, i );
						}
					}
					if( en[i]. time == 132 ) {
						en[i].range = 16;
						if( en[i].ry < 38 ) { en[i].angle = 150; }
						else				{ en[i].angle = 210; }
					}
					break;
				
				case 10:
					if( en[i].time == 12 ) { en[i].range = 0; }
					if( en[i].time == 24 ) {
						j = Eshot_SnipeAngle( 1, -10, 3, i );
						for( k = 18; k <= 30; k += 3 ) {
							Eshot_Generate( 1, -10, 3, k, j+15, i );
							Eshot_Generate( 1, -10, 3, k, j, i );
							Eshot_Generate( 1, -10, 3, k, j-15, i );
						}
					}
					if( en[i].time == 36 ) { en[i].range = 16; }
					break;
				
				case 13:	case 14:
					if( en[i].time % 10 == 0 ) {
						for( j = 0; j < 2; j++ ) {
							Eshot_Generate( 0, 3, 3, 12, rand()%120+120, i );
						}
					}
					break;
					
				case 15:
					if( en[i].time == 12 ) { en[i].range = 0; }
					if( en[i].time == 24 ) {
						for( k = 0; k < 360; k += 20 ) {
							Eshot_Generate( 8, 3, 3, 25, k, i );
						}
					}
					if( en[i]. time == 36 ) { en[i].range = 16; }
					break;
				case 16:
					if( en[i].time == 12 ) { en[i].range = 0; }
					if( en[i].time == 24 ) {
						for( k = 1; k < 361; k += 20 ) {
							Eshot_Generate( 8, 3, 3, 25, k, i );
						}
					}
					if( en[i]. time == 36 ) { en[i].range = 16; }
					break;
				case 17:
					if( en[i].time == 12 ) { en[i].range = 0; }
					if( en[i].time == 24 ) {
						for( j = 22; j < 360; j += 45 ) {
							Eshot_Generate( 7, 3, 3, 12, j, i );
						}
					}
					if( en[i]. time == 60 ) { en[i].range = 16; }
					break;
					
				case 18:
					if( en[i].time == 12 ) { en[i].range = 0; }
					if( en[i].time == 24 ) {
						j = Eshot_SnipeAngle( 10, -9, 2, i );
						for( k = j - 30; k <= j + 30; k += 30 ) {
							Eshot_Generate( 10, -9, 2, 30, k, i );
							Eshot_Generate( 11, -9, 3, 25, k, i );
							Eshot_Generate( 12, -10, 3, 20, k, i );
						}
					}
					if( en[i]. time == 36 ) { en[i].range = 16; }
					break;
				
				case 19:	case 21:
					if( en[i].time == 40 ) {
						j = rand() % 72;
						for( k = j; k <= 360+j; k += 72 ) {
							Eshot_Generate( 17, 2, 2, 15, k, i );
						}
						en[i].power = 0;
						pceWaveDataOut( 1, &CR );
					}
					break;
				
				case 22:
					if( en[i].time == 25 ) {
						j = Eshot_SnipeAngle( 17, 2, 2, i );
						for( k = j-40; k <= j+40; k += 20 ) {
							Eshot_Generate( 17, 2, 2, 20, k, i );
						}
						en[i].power = 0;
						pceWaveDataOut( 1, &CR );
					}
					break;
				
				case 23:	case 24:
					if( en[i].time == 30 ) {
						j = rand() % 30 + 30;
						for( k = j; k <= j+360; k += 90 ) {
							Eshot_Generate( 18, 2, 2, 15, k, i );
						}
						en[i].power = 0;
						pceWaveDataOut( 1, &CR );
					}
					break;
				
				case 25:
					if( en[i].time == 12 ) {
						en[i].range = 0;
					}
					if( en[i].time > 12 && en[i].time % 10 == 0 ) {
						Eshot_Generate( 18, 2, 2, 15, rand()%90-45, i );
					}
					if( en[i].time > 200 ) {
						en[i].power = 0;
						for( j = -60; j <= 60; j += 30 ) {
							Item_Generate( 0, j, i );
						}
						pceWaveDataOut( 1, &CR );
					}
					break;


			}
			
			
			
			en[i].x += en[i].range * Get_Cos( en[i].angle );
			en[i].y -= en[i].range * Get_Sin( en[i].angle );
			if( en[i].x < -16 * 10000 || en[i].x > 128 * 10000 || \
				en[i].y < -16 * 10000 || en[i].y >  88 * 10000 ) {
				en[i].power = 0;
				if( !boss && en[i].x <= 128*10000 ) { crashChain = 1; }
				muteki = FALSE;
				continue;
			}
			en[i].rx = en[i].x / 10000;
			en[i].ry = en[i].y / 10000;
			
			// 自機との当たり判定
			if( pl.rx+10 >= en[i].rx && pl.rx+10 < en[i].rx+enhit[engrp[en[i].type]][0] && pl.ry+6 >= en[i].ry && pl.ry+6 < en[i].ry+enhit[engrp[en[i].type]][1] && !pl.time ) {
				Life_Down();
			}
			
			// 自機弾との当たり判定
			for( j = 0; j < 64; j++ ) {
				if( ps[j].power && en[i].rx+enhit[engrp[en[i].type]][0]<128 && en[i].ry>=0 && en[i].ry+enhit[engrp[en[i].type]][1] < 88 ) {
					if( ( ps[j].rx >= en[i].rx && ps[j].rx < en[i].rx+enhit[engrp[en[i].type]][0] && ps[j].ry >= en[i].ry && ps[j].ry < en[i].ry+enhit[engrp[en[i].type]][1] ) || \
						( ps[j].rx+pshit[ps[j].type][0] >= en[i].rx && ps[j].rx+pshit[ps[j].type][0] < en[i].rx+enhit[engrp[en[i].type]][0] && ps[j].ry >= en[i].ry && ps[j].ry < en[i].ry+enhit[engrp[en[i].type]][1] ) || \
						( ps[j].rx >= en[i].rx && ps[j].rx < en[i].rx+enhit[engrp[en[i].type]][0] && ps[j].ry+pshit[ps[j].type][1] >= en[i].ry && ps[j].ry+pshit[ps[j].type][1] < en[i].ry+enhit[engrp[en[i].type]][1] ) || \
						( ps[j].rx+pshit[ps[j].type][0] >= en[i].rx && ps[j].rx+pshit[ps[j].type][0] < en[i].rx+enhit[engrp[en[i].type]][0] && ps[j].ry+pshit[ps[j].type][1] >= en[i].ry && ps[j].ry+pshit[ps[j].type][1] < en[i].ry+enhit[engrp[en[i].type]][1] ) ) {
						if( !muteki || i ) {
							en[i].power -= ps[j].power;
							ps[j].power = 0;
							if( ( !boss || i ) && !muteki && en[i].power <= 0 ) {
								en[i].power = 0;
								Score_Add( crashChain );
								pceWaveDataOut( 1, &CR );
								Item_Generate( 0, ps[j].angle, i );
								Item_Generate( 3, 0, i );
								if( crashChain < 512 ) { crashChain *= 2; }
								break;
							}
						}
					}
				}
			}
			// ボムとの当たり判定
			if( bombtime && ( !muteki || i ) && en[i].rx+enhit[engrp[en[i].type]][0]<128 ) {
				if( !chara && en[i].rx >= pl.rx+12 && en[i].ry+enhit[engrp[en[i].type]][1] >= pl.ry && en[i].ry < pl.ry+12 ) {
					if( !boss || i ) { en[i].power -= 5; }
					else { en[i].power -= 2; }
				}
				if( chara ) { en[i].power -= 2; }
				if( ( !boss || i ) && !muteki && en[i].power <= 0 ) {
					en[i].power = 0;
					Score_Add( crashChain );
					pceWaveDataOut( 1, &CR );
					Item_Generate( 0, 0, i );
					Item_Generate( 3, 0, i );
				}
			}
		}
	}
}


int Eshot_SnipeAngle( int type, int x, int y, int enemy )
{
	int plx, ply, esx, esy;
	
	plx = pl.rx + 10;
	ply = pl.ry +  6;
	esx = en[enemy].rx + x + eshit[esgrp[type]][0] / 2;
	esy = en[enemy].ry + y + eshit[esgrp[type]][1] / 2;
	return( Get_ArcTan( plx - esx, esy - ply ) );
}


void Eshot_Generate( int type, int x, int y, int range, int angle, int enemy )
{
	int i = 0;
	
	while( es[i].power ) {
		i++; 
		if( i >= 64 ) { return; }
	}
	
	es[i].type = type;
	es[i].x = en[enemy].x + x * 10000;
	es[i].y = en[enemy].y + y * 10000;
	es[i].rx = es[i].x / 10000;
	es[i].ry = es[i].y / 10000;
	es[i].range = range;
	es[i].angle = angle;
	es[i].power = 1;
	es[i].time = 0;
	es[i].anim = enemy;
	pceWaveDataOut( 1, &EF );
}


void Eshot_Action()
{
	int i;
	
	for( i = 0; i < 64; i++ ) {
		if( es[i].power ) {
			es[i].time++;
			switch( es[i].type ) {
				case 3:
					if( es[i].time == 15 ) {
						es[i].angle -= 42;	es[i].range += 5;
						pceWaveDataOut( 1, &EF );
					}
					break;
				case 4:
					if( es[i].time == 15 ) {
						es[i].angle -= 14;	es[i].range += 5;
					}
					break;
				case 5:
					if( es[i].time == 15 ) {
						es[i].angle += 14;	es[i].range += 5;
					}
					break;
				case 6:
					if( es[i].time == 15 ) {
						es[i].angle += 42;	es[i].range += 5;
					}
					break;
				case 7:
					if( es[i].time == 15 ) { es[i].range = 0; }
					if( es[i].time == 30 ) {
						es[i].angle = Eshot_SnipeAngle( 7, 3, 6, es[i].anim ) -(es[i].angle/15-12);
						es[i].range = 30;
					}
					break;
				case 8:
					if( es[i].angle % 2 ) { es[i].angle += 2; }
					else				  { es[i].angle -= 2; }
					break;
				
				case 13:	case 14:	case 15:	case 16:	case 18:
					if( es[i].rx+eshit[esgrp[es[i].type]][0]/2 > 128 ) {
						es[i].angle = 180 - es[i].angle;
					}
					if( es[i].ry+eshit[esgrp[es[i].type]][1]/2 < 0 || \
						es[i].ry+eshit[esgrp[es[i].type]][1]/2 > 88 ) {
						es[i].angle = -es[i].angle;
					}
					break;
			}
			
			
			es[i].x += es[i].range * Get_Cos( es[i].angle );
			es[i].y -= es[i].range * Get_Sin( es[i].angle );
			if( es[i].x < -8 * 10000 || es[i].x > 136 * 10000 || \
				es[i].y < -8 * 10000 || es[i].y >  96 * 10000 ) {
				es[i].power = 0;
			}
			es[i].rx = es[i].x / 10000;
			es[i].ry = es[i].y / 10000;
			
			// 自機との当たり判定
			if( pl.rx+10 >= es[i].rx && pl.rx+10 < es[i].rx+eshit[esgrp[es[i].type]][0] && pl.ry+6 >= es[i].ry && pl.ry+6 < es[i].ry+eshit[esgrp[es[i].type]][1] && !pl.time ) {
				Life_Down();
			}
			// ボム（ウルトレーザーのみ）との当たり判定
			if( bombtime && !chara && es[i].rx >= pl.rx+20 && es[i].ry+eshit[esgrp[es[i].type]][1] >= pl.ry-3 && es[i].ry < pl.ry+15 ) {
				es[i].power = 0;
				Score_Add( 1 );
				pceWaveDataOut( 1, &CR );
			}
		}
	}
}


void Eshot_Clear()
{
	int i;

	for( i = 0; i < 64; i++ ) {
		if( es[i].power ) {
			Score_Add( 1 );
			es[i].power = 0;
		}
	}
}

void Item_Generate( int type, int angle, int enemy )
{
	int i = 0;
	
	while( it[i].power ) {
		i++;
		if( i >= 64 ) { return; }
	}
	
	if( type < 3 ) {
		it[i].type = type;
		it[i].x = en[enemy].x + ( enhit[engrp[en[enemy].type]][0] - ithit[type] ) * 5000;
		it[i].y = en[enemy].y + ( enhit[engrp[en[enemy].type]][1] - ithit[type] ) * 5000;
		it[i].range = 20;
		it[i].angle = angle;
		it[i].power = 1;
	}
	else {
		it[i].power = crashChain;
		it[i].type = 3;
		it[i].x = en[enemy].x;
		if( it[i].x > 100 * 10000 ) { it[i].x = 100 * 10000; }
		it[i].y = en[enemy].y;
		it[i].range = 10000;
		it[i].angle = 90;
	}
	it[i].rx = it[i].x / 10000;
	it[i].ry = it[i].y / 10000;
}


void Item_Action()
{
	int i, vx, vy;
	
	for( i = 0; i < 64; i++ ) {
		if( it[i].power ) {
			if( it[i].type < 3 ) {
				vx = it[i].range * Get_Cos( it[i].angle );
				vy = it[i].range * Get_Sin( it[i].angle );
				if( vx > 0 ) {
					it[i].range--;
					it[i].y -= vy;
					if( it[i].y < 0 || it[i].y > (88-ithit[it[i].type])*10000 ) { it[i].angle = -it[i].angle; }
				}
				else {
					if( it[i].range < 20 )
					it[i].range++;
					it[i].angle = 180;
				}
				it[i].x += vx;
				if( it[i].x < -12 * 10000 ) {
					it[i].power = 0;
					if( it[i].type == 0 ) { itemChain = 1; }
				}
				it[i].rx = it[i].x / 10000;
				it[i].ry = it[i].y / 10000;
				
				// 自機との当たり判定
				if( ( pl.rx <= it[i].rx && pl.rx+16 > it[i].rx && pl.ry <= it[i].ry && pl.ry+12 > it[i].ry ) || \
					( pl.rx < it[i].rx+ithit[it[i].type] && pl.rx+16 >= it[i].rx+ithit[it[i].type] && pl.ry <= it[i].ry && pl.ry+12 > it[i].ry ) || \
					( pl.rx <= it[i].rx && pl.rx+16 > it[i].rx && pl.ry < it[i].ry+ithit[it[i].type] && pl.ry+12 >= it[i].ry+ithit[it[i].type] ) || \
					( pl.rx < it[i].rx+ithit[it[i].type] && pl.rx+16 >= it[i].rx+ithit[it[i].type] && pl.ry < it[i].ry+ithit[it[i].type] && pl.ry+12 >= it[i].ry+ithit[it[i].type] ) ) {
					switch( it[i].type ) {
						case 0:
							Score_Add( itemChain );
							level++;
							if( level % 15 ) { pceWaveDataOut( 3, &IG ); }
							else			 { pceWaveDataOut( 3, &PU ); }
							if( level > 45 ) { level = 45; }
							it[i].power = itemChain;
							if( itemChain < 512 ) { itemChain *= 2; }
							break;
						case 1:
							bomb++;
							pceWaveDataOut( 3, &PU );
							break;
						case 2:
							life++;
							pceWaveDataOut( 3, &EX );
							break;
					}
					it[i].type += 3;	// アイテム取った表示に切り替え
					if( it[i].x > 100 * 10000 ) { it[i].x = 100 * 10000; }
					it[i].rx = it[i].x / 10000;
					it[i].range = 10000;
					it[i].angle = 90;
				}
			}
			else {
				it[i].range -= 2000;
				if( it[i].range <= 0 ) { it[i].power = 0; }
				it[i].y -= it[i].range;
				if( it[i].x < -12 * 10000 || it[i].x > 128 * 10000 || \
					it[i].y < -12 * 10000 || it[i].y >  88 * 10000 ) {
					it[i].power = 0;
				}
				it[i].ry = it[i].y / 10000;
			}
		}
	}
}


void Score_Add( int point )
{
	score += point;
	if( score/100000 > pastscore/100000 ) {
		life++;
		pceWaveDataOut( 3, &EX );
	}
	pastscore = score;
}


void Life_Down()
{
	pceWaveDataOut( 2, &DN );
	kurai = TRUE;
	pl.time = 8;
}

void Game_Graphics()
{
	static int cnt = 0;
	int i;
	static int dlevel = 0, dscore = 0, dvital = 0, dbonus, bonusvisible = 0;

	cnt++;
	pceLCDPaint( 0, 0, 0, 128, 88 );
	if( bombtime ) {
		i = bombtime * 4 - 232;
		if( i < 12 ) { i = 12; }
		Draw_Object( &BM, 0, i, 0, 0, 128, 64, DRW_NOMAL );
	}
	
	// 自機
	if( pl.time % 10 < 5 && !kurai ) {
		Draw_Object( &PL, pl.rx, pl.ry, pl.anim/5*16, chara*12, 16, 12, DRW_NOMAL );
	}
	
	
	// 自機弾
	for( i = 0; i < 64; i++ ) {
		if( ps[i].power ) {
			Draw_Object( &PS, ps[i].rx, ps[i].ry, psgrp[ps[i].type]*8, 0, 8, 8, DRW_NOMAL );
		}
	}
	
	// 敵
	for( i = 0; i < 64; i++ ) {
		if( ( !muteki || cnt % 10 < 5 || i ) && en[i].power ) {
			if( engrp[en[i].type] == 6 ) {
				Draw_Object( &EN, en[i].rx, en[i].ry, en[i].anim/10*32, engrp[en[i].type]*16, 32, 32, DRW_NOMAL );
			}
			else {
				Draw_Object( &EN, en[i].rx, en[i].ry, en[i].anim/5*16, engrp[en[i].type]*16, 16, 16, DRW_NOMAL );
			}
		}
	}
	
	
	// ウルトレーザー
	if( bombtime && !chara ) {
		Draw_Object( &LS, pl.rx+16, pl.ry-6, 0, 0, 8, 24, DRW_NOMAL );
		pceLCDPaint( 2, pl.rx+24, pl.ry-6, 128, 24 );
		pceLCDPaint( 1, pl.rx+24, pl.ry-4, 128, 20 );
		pceLCDPaint( 0, pl.rx+24, pl.ry,   128, 12 );
	}
	
	// アイテム
	for( i = 0; i < 64; i++ ) {
		if( it[i].power ) {
			pceFontSetType( 2 );
			switch( it[i].type ) {
				case 0:
				case 1:
				case 2:
					Draw_Object( &IT, it[i].rx, it[i].ry, it[i].type*12, 0, 12, 12, DRW_NOMAL );
					break;
				case 3:
					wFontPrintf( 1, it[i].rx, it[i].ry, "%d00", it[i].power );
					break;
				case 4:
					wFontPrintf( 1, it[i].rx, it[i].ry, "BOM" );
					break;
				case 5:
					wFontPrintf( 1, it[i].rx, it[i].ry, "1up" );
					break;
			}
		}
	}
	
	// 敵弾
	for( i = 0; i < 64; i++ ) {
		if( es[i].power ) {
			Draw_Object( &ES, es[i].rx, es[i].ry, esgrp[es[i].type]*8, 0, 8, 8, DRW_NOMAL );
		}
	}
	
	// カミュ反転
	if( bombtime && chara && bombtime < 60 ) {
		hanToumeiAll( 5 );
	}
	
	// ライフ
	pceFontSetType( 2 );
	wFontPrintf( 0, 1, 1, "x%02d", life-1 );
	
	// ボム数
	for( i = 0; i < bomb; i++ ) {
		pceLCDPaint( 1, i*8+14, 0, 6, 3 );
		pceLCDPaint( 2, i*8+15, 0, 4, 3 );
		pceLCDPaint( 3, i*8+16, 0, 2, 3 );
	}
	
	// レベルゲージ
	if( gameover )       { dlevel = 0; }
	if( level == 0     ) { dlevel = 0; }
	if( dlevel < level ) { dlevel++;   }
	if( dlevel > level ) { dlevel--;   }
	pceLCDPaint( 1, 14, 4, dlevel, 3 );
	if( cnt%10 > 5 ) { pceLCDPaint( 2, 14, 4, (dlevel/15)*15, 3 ); }
	
	// スコア表示
	if( gameover )               { dscore = score; }
	if( dscore         > score ) { dscore =     0; }
	if( dscore         < score ) { dscore++;       }
	if( dscore +    10 < score ) { dscore +=   10; }
	if( dscore +   100 < score ) { dscore +=  100; }
	if( dscore +  1000 < score ) { dscore += 1000; }
	wFontPrintf( 0, 1, 82, "SC:%07d0%d", dscore, cont );
	
	// ボス時カウント・体力
	if( boss ) {
		if( boss % 2 ) { wFontPrintf( 0, 60, 82, "%02d", count/40 ); }
		if( en[0].power/20 == 0 )	  { dvital = 0; }
		if( dvital < en[0].power/20 ) { dvital++; }
		if( dvital > en[0].power/20 ) { dvital--; }
		if( dvital > 58 )			  { dvital = 58; }
		pceLCDPaint( 2, 128-dvital, 84, dvital, 3 );
	}
	
	// ボーナス
	if( bonus ) {
		dbonus = bonus;
		bonus = 0;
		bonusvisible = 50;
	}
	if( bonusvisible ) {
		bonusvisible--;
		if( bonusvisible % 10 < 5 ) {
			wFontPrintf( 0, 44, 37, "TIME BONUS" );
			wFontPrintf( 0, 48, 44, "%06d00", dbonus );
		}
	}
	
	// ステージ
	pceFontSetType( 0 );
	if( time < 80 && time % 16 >= 8 && stage % 2 ) {
		wFontPrintf( 0, 34, 39, "S T A G E  %d", stage/2+1 );
	}
	
	// ポーズ画面
	if( pause ) {
		hanToumeiAll( 4 );
		wFontPrintf( 0, 42, 20, "P A U S E" );
		wFontPrintf( 1, 49, 40, "再　開" );
		wFontPrintf( 1, 49, 52, "終　了" );
		hanToumei( 5, 44, cursor*12+39, 41, 11 );
	}
	
	// コンティニュー画面
	if( gameover ) {
		hanToumeiAll( 4 );
		wFontPrintf( 0, 27, 20, "G A M E O V E R" );
		wFontPrintf( 1, 49, 40, "復　活" );
		wFontPrintf( 1, 49, 52, "終　了" );
		hanToumei( 5, 44, cursor*12+39, 41, 11 );
	}
	
	LCDUpdate = TRUE;
}

