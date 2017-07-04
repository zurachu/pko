/* Ｐ・Ｋ・Ｏ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <piece.h>
#include "gamelib.h"

void Title_Init();
void Title_Free();
void Ending_Init();
void Ending_Free();
void Logo( int cnt );
void Title();
void HiScore( int flag );
void Character_Select();
void Draw_FireBackGround( int flag );
void Hiscore_Init();
void Hiscore_Update();
void Ending();

extern void Graphic_Free();
extern void Game_Init();
extern void Stage_Init();
extern void Game_Main();

unsigned char *arc;

extern unsigned char *PLAYER, *BOMB;
extern PIECE_BMP PL;
extern unsigned char *ITEMGET, *WEAPON;
extern PCEWAVEINFO IG, WP;
unsigned char *TITLEBG, *TITLE, *CHARASEL, *SHADOW;
PIECE_BMP BMP;
extern unsigned char *BGM;

extern int stage, score, cont, chara;

typedef struct {
	int chara;
	int score;
	int cont;
} HSC;
HSC HighScore[11];

int FadeOut;

BOOL demo;

unsigned short GameMode;
#define GM_LOGO		1
#define GM_TITLE	2
#define GM_HISCORE	4
#define GM_CHARASEL	8
#define GM_ENDING	16
#define GM_MAIN		64
#define GM_HISCORE2	128
#define GM_INIT		256
#define GM_END		4096
#define GM_NODATA	8192

//=============================================================================
//  アプリケーションの初期化
//=============================================================================
void pceAppInit( void )
{
	FILEINFO pfi;
	FILEACC  pfa;
	short i = 0;

	pceAppSetProcPeriod( 25 );
	App_Init();	// 起動時処理

	if( pcePadGet() & PAD_D ) { demo = TRUE; }
	else					  { demo = FALSE; }

	pceFileFindOpen( &pfi );	// ファイル検索
	while( pceFileFindNext( &pfi ) ) {
		if( !strcmp( "pko.dat", pfi.filename ) ) {
			arc = pceHeapAlloc( pfi.length );
			pceFileOpen( &pfa, "pko.dat", FOMD_RD );
			while( pceFileReadSct( &pfa, arc+i*4096, i, 4096 ) ) { i++; }
			pceFileClose( &pfa );
			break;
		}
	}
	pceFileFindClose( &pfi );

	if( i == 0 ) {
		GameMode = GM_NODATA | GM_INIT;
		return;
	}
	
	PLAYER = ppack_findPackData( arc, "player.pgd" );
	Get_PieceBmp( &PL, PLAYER );
	WEAPON = ppack_findPackData( arc, "weapon.ppd" );
	Get_PieceWave( &WP, WEAPON );
	ITEMGET = ppack_findPackData( arc, "itemget.ppd" );
	Get_PieceWave( &IG, ITEMGET );


	Hiscore_Init();
	Title_Init();
	chara = 0;
	GameMode = GM_LOGO;
}


//=============================================================================
//  メインプロセス
//=============================================================================
void pceAppProc( int cnt )
{
	if( demo ) { pceAppActiveResponse( AAR_ACTIVE ); }	// デモモード

	if( GameMode & GM_INIT ) {
		LCDUpdate = TRUE;
		GameMode ^= GM_INIT;
	}
	
	if( FadeOut ) {
		if( !( FadeOut % 10 ) ) {
			hanToumeiAll( 4 );
			LCDUpdate = TRUE;
		}
		FadeOut--;
		if( FadeOut == 30 ) {
			StopMusic();
		}
		if( FadeOut <= 0 ) {
			switch( GameMode ) {
				case GM_TITLE:
					Title_Init();
					PlayMusic( BGM );
					GameMode |= GM_INIT;
					break;
				case GM_MAIN:
					stage++;
					if( stage == 9 ) {
						Graphic_Free();
						Ending_Init();
						GameMode = GM_ENDING | GM_INIT;
						Ending();
						break;
					}
					Stage_Init();
					break;
				case GM_HISCORE2:
					Graphic_Free();
					Ending_Free();
					Title_Init();
					Hiscore_Update();
					break;
			}
		}
	}
	else {
		switch( GameMode ) {
			case GM_LOGO:
				Logo( cnt );
				break;
			case GM_TITLE:
				Title();
				break;
			case GM_HISCORE:
				HiScore( 0 );
				break;
			case GM_HISCORE2:
				HiScore( 1 );
				break;
			case GM_CHARASEL:
				Character_Select();
				break;
			case GM_ENDING:
				Ending();
				break;
			case GM_MAIN:
				Game_Main();
				break;
			case GM_END:
				pceAppReqExit( 0 );
				break;
			case GM_NODATA:
				if( LCDUpdate ) {
					pceFontSetType( 0 );
					pceFontSetTxColor( 3 );
					pceFontSetBkColor( FC_SPRITE );
					pceFontSetPos( 7, 39 );
					pceFontPutStr( "pko.datが見つかりません" );
				}
				if( pcePadGet() & ( TRG_A | TRG_B | TRG_C | TRG_D ) ) {
					pceAppReqExit( 0 );
				}
				break;
		}
	}
	
	if( LCDUpdate ) {
		LCDUpdate = FALSE;
		pceLCDTrans();
	}
}

//=============================================================================
//  アプリケーションの終了
//=============================================================================
void pceAppExit( void )
{
	Graphic_Free();
	Title_Free();
	Ending_Free();
	App_Exit();	// 終了時処理
}


//=============================================================================
//  
//=============================================================================
void Title_Init()
{
	TITLEBG = ppack_findPackData( arc, "titlebg.pgd" );
	TITLE = ppack_findPackData( arc, "title.pgd" );
	CHARASEL = ppack_findPackData( arc, "charasel.pgd" );
	SHADOW = ppack_findPackData( arc, "shadow.pgd" );
	BGM = ppack_findPackData( arc, "02.pmd" );
	stage = 0;
}


//=============================================================================
//  
//=============================================================================
void Title_Free()
{
	StopMusic();
	pceWaveAbort( 0 );

	pceHeapFree( TITLEBG );
	pceHeapFree( TITLE );
	pceHeapFree( CHARASEL );
	pceHeapFree( SHADOW );
	pceHeapFree( BGM );
}


//=============================================================================
//  
//=============================================================================
void Ending_Init()
{
	CHARASEL = ppack_findPackData( arc, "charasel.pgd" );
	if( !chara ) { BOMB = ppack_findPackData( arc, "uruto.pgd" ); }
	else		 { BOMB = ppack_findPackData( arc, "kamyu.pgd" ); }
	BGM = ppack_findPackData( arc, "ending.pmd" );
}


//=============================================================================
//  
//=============================================================================
void Ending_Free()
{
	StopMusic();
	pceWaveAbort( 0 );

	pceHeapFree( CHARASEL );
	pceHeapFree( BOMB );
	pceHeapFree( BGM );
}


//=============================================================================
//  てとら★ぽっと
//=============================================================================
void Logo( int cnt )
{
	if( pcePadGet() & TRG_A || cnt >= 140 ) {
		PlayMusic( BGM );
		GameMode = GM_TITLE | GM_INIT;
	}

	Draw_FireBackGround( 0 );
	if( !( cnt%10 ) ) { LCDUpdate = TRUE; }
	if( LCDUpdate ) {
		pceFontSetType( 0 );
		wFontPrintf( 1, 9, 39, "(c)2003 てとら★ぽっと" );
		if( cnt >  80 ) { hanToumeiAll( 4 ); }
		if( cnt > 100 ) { hanToumeiAll( 4 ); }
		if( cnt > 120 ) { hanToumeiAll( 4 ); }
	}
}


//=============================================================================
//  タイトル画面
//=============================================================================
void Title()
{
	static int cursor = 0;
	int pad = pcePadGet();

	if( cursor == 0 && pad & TRG_LF ) {
		stage -= 2;
		if( stage < 0 ) { stage = 6; }
		LCDUpdate = TRUE;
		pceWaveDataOut( 3, &IG );
	}
	if( cursor == 0 && pad & TRG_RI ) {
		stage += 2;
		if( stage > 6 ) { stage = 0; }
		LCDUpdate = TRUE;
		pceWaveDataOut( 3, &IG );
	}
	if( pad & TRG_UP ) {
		stage = 0;
		cursor--;
		if( cursor < 0 ) { cursor = 2; }
		LCDUpdate = TRUE;
		pceWaveDataOut( 3, &IG );
	}
	if( pad & TRG_DN ) {
		stage = 0;
		cursor++;
		if( cursor > 2 ) { cursor = 0; }
		LCDUpdate = TRUE;
		pceWaveDataOut( 3, &IG );
	}
	if( pad & TRG_A ) {
		pceWaveDataOut( 3, &WP );
		switch( cursor ) {
			case 0:
				GameMode = GM_CHARASEL | GM_INIT;
				break;
			case 1:
				GameMode = GM_HISCORE | GM_INIT;
				break;
			case 2:
				FadeOut = 30;
				GameMode = GM_END;
				break;
		}
	}

	Draw_FireBackGround( 1 );
	if( LCDUpdate ) {
		hanToumeiAll( 2 );
		Get_PieceBmp( &BMP, TITLE );
		Draw_Object( &BMP, 10, 10, 0, 0, 108, 33, DRW_NOMAL );
		
		pceFontSetType( 0 );
		if( cursor == 0 ) { wFontPrintf( 0, 29, 50, "←　　　　　→" ); }
		switch( stage / 2 ) {
			case 0:	wFontPrintf( 1, 49, 50, "開　始" );	break;
			case 1:	wFontPrintf( 1, 59, 50, "弐" );	break;
			case 2:	wFontPrintf( 1, 59, 50, "参" );	break;
			case 3:	wFontPrintf( 1, 59, 50, "四" );	break;
		}
		wFontPrintf( 1, 49, 62, "戦　歴" );
		wFontPrintf( 1, 49, 74, "終　了" );
		hanToumei( 5, 44, cursor*12+49, 41, 11 );
	}
}


//=============================================================================
//  ハイスコア画面
//=============================================================================
void HiScore( int flag )
{
	static int cnt = 0;
	int i;

	if( pcePadGet() & ( TRG_A | TRG_B ) ) {
		GameMode = GM_TITLE | GM_INIT;
		pceWaveDataOut( 3, &WP );
		if( flag ) {
			Title_Free();
			FadeOut = 30;
		}
	}

	cnt++;
	if( !(cnt%10) ) { LCDUpdate = TRUE; }
	
	Draw_FireBackGround( 1 );
	if( LCDUpdate ) {
		hanToumeiAll( 2 );
		pceFontSetType( 0 );
		wFontPrintf( 0, 49, 6, "戦　歴" );
		pceFontSetType( 2 );
		for( i = 0; i < 10; i++ ) {
			Get_PieceBmp( &BMP, SHADOW );
			Draw_Object( &BMP, (i/5)*62+4, (i%5)*13+19, ((cnt+20*i)%80)/20*18, HighScore[i].chara*14, 18, 14, DRW_CLR(COLOR_MASK,COLOR_WHITE) );
			Draw_Object( &PL, (i/5)*62+5, (i%5)*13+20, ((cnt+20*i)%80)/20*16, HighScore[i].chara*12, 16, 12, DRW_NOMAL );
			if( !( flag && score == HighScore[i].score && cont == HighScore[i].cont && ( cnt % 20 < 10 ) ) ) {
				wFontPrintf( 0,(i/5)*62+24, (i%5)*13+24, "%07d0%d", HighScore[i].score, HighScore[i].cont );
			}
		}
	}
}


//=============================================================================
//  キャラクターセレクト画面
//=============================================================================
void Character_Select()
{
	if( pcePadGet() & ( TRG_LF | TRG_RI ) ) {
		chara++;
		if( chara > 1 ) { chara = 0; }
		LCDUpdate = TRUE;
		pceWaveDataOut( 3, &IG );
	}
	if( pcePadGet() & TRG_A ) {
		Title_Free();
		Game_Init();
		GameMode = GM_MAIN;
		FadeOut = 30;
		pceWaveDataOut( 3, &WP );
	}
	if( pcePadGet() & TRG_B ) {
		GameMode = GM_TITLE | GM_INIT;
		pceWaveDataOut( 3, &WP );
	}

	Draw_FireBackGround( 1 );
	if( LCDUpdate ) {
		hanToumeiAll( 2 );
		Get_PieceBmp( &BMP, CHARASEL );
		if( !chara ) { Draw_Object( &BMP, 48, 0, 80, 0, 80, 88, DRW_ADD ); }
		else		 { Draw_Object( &BMP,  0, 0,  0, 0, 80, 88, DRW_ADD ); }
		if( !chara ) { Draw_Object( &BMP,  0, 0,  0, 0, 80, 88, DRW_NOMAL ); }
		else		 { Draw_Object( &BMP, 48, 0, 80, 0, 80, 88, DRW_NOMAL ); }
		
		pceFontSetType( 0 );
		wFontPrintf( 0, 29, 3, "戦　体　選　択" );
	}

}


//=============================================================================
//  炎の背景
//=============================================================================
void Draw_FireBackGround( int flag )
{
	static int past;
	
	if( !flag ) { flag = rand() % 40; }
	
	if( !past || !flag ) { LCDUpdate = TRUE; }
	if( LCDUpdate ) {
		Get_PieceBmp( &BMP, TITLEBG );
		Draw_Object( &BMP, 0, 0, 0, 0, 128, 88, DRW_NOMAL );
		if( flag ) { hanToumeiAll( 4 ); }
	}
	past = flag;
}


//=============================================================================
//  ハイスコア初期化
//=============================================================================
void Hiscore_Init()
{
	FILEACC pfa;
	int i;

	for( i = 0; i < 10; i++ ) {
		HighScore[i].chara = i % 2;
		HighScore[i].score = (10-i) * 10000;
		HighScore[i].cont  = 0;
	}
	if( pceFileOpen( &pfa, "pko.sav", FOMD_RD ) ) {
		pceFileCreate( "pko.sav", sizeof(HSC)*10 );
		pceFileOpen( &pfa, "pko.sav", FOMD_WR );
		pceFileWriteSct( &pfa, &HighScore, 0, sizeof(HSC)*10 );
	}
	else {
		pceFileReadSct( &pfa, &HighScore, 0, sizeof(HSC)*10 );
	}
	pceFileClose( &pfa );
}

//=============================================================================
//  ハイスコア書き換え
//=============================================================================
void Hiscore_Update()
{
	FILEACC pfa;
	int i = 9;
	
	while( i >= 0 && ( score > HighScore[i].score || ( score == HighScore[i].score && cont >= HighScore[i].cont ) ) ) {
		HighScore[i+1].chara = HighScore[i].chara;
		HighScore[i+1].score = HighScore[i].score;
		HighScore[i+1].cont  = HighScore[i].cont;
		HighScore[i].chara = chara;
		HighScore[i].score = score;
		HighScore[i].cont  = cont;
		i--;
	}
	
	pceFileOpen( &pfa, "pko.sav", FOMD_WR );
	pceFileWriteSct( &pfa, &HighScore, 0, sizeof(HSC)*10 );
	pceFileClose( &pfa );
	
	GameMode = GM_HISCORE2 | GM_INIT;
}


//=============================================================================
//  エンディング
//=============================================================================
void Ending()
{
	static int count = 0;
	
	int i;
	
	if( GameMode & GM_INIT ) {
		count = 0;
		pceFontSetType( 0 );
		pceFontSetBkColor( FC_SPRITE );
		Get_PieceBmp( &BMP, CHARASEL );
		PlayMusic( BGM );
	}

	count++;

	if( count < 440 ) {
		pceLCDPaint( 3, 0, 0, 128, 88 );
		if( !chara ) {
			pceLCDPaint( 0, 0, 0, 80, 88 );
			Draw_Object( &BMP, 0, 0, 0, 0, 80, 88, DRW_NOMAL );
		}
		else {
			pceLCDPaint( 0, 48, 0, 80, 88 );
			Draw_Object( &BMP, 48, 0, 80, 0, 80, 88, DRW_NOMAL );
		}
	}
	
	if( count >= 30 && count < 130 ) {
		wFontPrintf( 0, 29, 88+30-count, "『Ｐ・Ｋ・Ｏ』" );
		LCDUpdate = TRUE;
	}
	if( count >= 45 && count < 145 ) {
		wFontPrintf( 0, 22, 88+45-count, "Development Staff" );
		LCDUpdate = TRUE;
	}
	if( count >= 100 && count < 200 ) {
		wFontPrintf( 0, 47, 88+100-count, "Program" );
		LCDUpdate = TRUE;
	}
	if( count >= 110 && count < 210 ) {
		wFontPrintf( 0, 34, 88+110-count, "Stage Design" );
		LCDUpdate = TRUE;
	}
	if( count >= 120 && count < 220 ) {
		wFontPrintf( 0, 34, 88+120-count, "Game Graphic" );
		LCDUpdate = TRUE;
	}
	if( count >= 135 && count < 235 ) {
		wFontPrintf( 0, 39, 88+135-count, "ヅラＣｈｕ" );
		LCDUpdate = TRUE;
	}
	if( count >= 190 && count < 290 ) {
		wFontPrintf( 0, 22, 88+190-count, "Character Graphic" );
		LCDUpdate = TRUE;
	}
	if( count >= 205 && count < 305 ) {
		wFontPrintf( 0, 49, 88+205-count, "KARASU" );
		LCDUpdate = TRUE;
	}
	if( count >= 260 && count < 360 ) {
		wFontPrintf( 0, 39, 88+260-count, "Game Music" );
		LCDUpdate = TRUE;
	}
	if( count >= 275 && count < 375 ) {
		wFontPrintf( 0, 57, 88+275-count, "MKT" );
		LCDUpdate = TRUE;
	}
	if( count >= 295 && count < 395 ) {
		wFontPrintf( 0, 32, 88+295-count, "Arranged from" );
		LCDUpdate = TRUE;
	}
	if( count >= 310 && count < 410 ) {
		wFontPrintf( 0, 24, 88+310-count, "(c)Leaf/AQUAPLUS" );
		LCDUpdate = TRUE;
	}
	if( count == 410 ) { LCDUpdate = TRUE; }

	if( count == 440 || count == 460 || count == 480 ) {
		hanToumeiAll( 4 );
		LCDUpdate = TRUE;
	}

	if( count >= 500 && count < 961 ) {
		pceLCDPaint( 3, 0, 0, 128, 88 );
		i = 88 - ( count - 500 )/2;
		if( i < 4 ) { i = 4; }
		Get_PieceBmp( &BMP, BOMB );
		Draw_Object( &BMP, 0, i, 0, 0, 128, 64, DRW_NOMAL );
		LCDUpdate = TRUE;
	}
	if( count >= 700 && count < 800 ) {
		wFontPrintf( 0, 29, 88+700-count, "Special Thanks" );
		LCDUpdate = TRUE;
	}
	if( count >= 715 && count < 815 ) {
		wFontPrintf( 0, 52, 88+715-count, "ryong" );
		LCDUpdate = TRUE;
	}
	if( count >= 730 && count < 830 ) {
		wFontPrintf( 0, 42, 88+730-count, "Iwasaki++" );
		LCDUpdate = TRUE;
	}
	if( count >= 745 && count < 845 ) {
		wFontPrintf( 0, 42, 88+745-count, "Randomist" );
		LCDUpdate = TRUE;
	}
	if( count >= 760 && count < 860 ) {
		wFontPrintf( 0, 32, 88+760-count, "Leaf/AQUAPLUS" );
		LCDUpdate = TRUE;
	}


	if( count >= 815 && count < 915 ) {
		wFontPrintf( 0, 44, 88+815-count, "(c) 2003" );
		LCDUpdate = TRUE;
	}
	if( count >= 830 && count < 930 ) {
		wFontPrintf( 0, 29, 88+830-count, "てとら★ぽっと" );
		LCDUpdate = TRUE;
	}
	if( count >= 845 && count < 945 ) {
		wFontPrintf( 0, 44, 88+845-count, "Presents" );
		LCDUpdate = TRUE;
	}
	
	if( count >= 945 && count < 961 ) {
		wFontPrintf( 0, 12, 88+945-count, "Thank you for Playing" );
		LCDUpdate = TRUE;
	}

	if( count > 1500 || pcePadGet() & PAD_C ) {
		GameMode = GM_HISCORE2;
		FadeOut = 50;
	}

}

