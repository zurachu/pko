/* Ｐ・Ｋ・Ｏ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <piece.h>
#include "gamelib.h"

void Title_Init();
void Title_Free();
void Logo( int cnt );
void Title();
void HiScore( int flag );
void Character_Select();
void Draw_FireBackGround( int flag );
void Hiscore_Init();
void Hiscore_Update();

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
		if( !strcmp( "pkotrial.dat", pfi.filename ) ) {
			arc = pceHeapAlloc( pfi.length );
			pceFileOpen( &pfa, "pkotrial.dat", FOMD_RD );
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
					if( stage == 3 ) {
						Graphic_Free();
						Title_Init();
						Hiscore_Update();
						break;
					}
					Stage_Init();
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
					pceFontSetPos( 29, 34 );
					pceFontPutStr( "pkotrial.datが" );
					pceFontSetPos( 29, 44 );
					pceFontPutStr( "見つかりません" );
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

	if( pad & TRG_UP ) {
		cursor--;
		if( cursor < 0 ) { cursor = 2; }
		LCDUpdate = TRUE;
		pceWaveDataOut( 3, &IG );
	}
	if( pad & TRG_DN ) {
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
		wFontPrintf( 1, 49, 50, "開　始" );
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


