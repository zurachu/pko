// 圧縮展開関連 機能拡張版 byヅラChu

#include <string.h>
#include <piece.h>
#include "filepack.h"

#define PPACK_HEADER 0x1c0258

/* ここからzlib展開ルーチンサンプル(from P/ECE HAND BOOK Vol.2 緋色の霧) */

#include "piecezl.h"
// 0x13e000～0x13ffff まで、展開用バッファ
#define	EXTSIZE		0x0400
#define	WORKSADR	0x13d400
#define	WORKS 		((void *)WORKSADR)
#define	EXTBUFF		((unsigned char *)(WORKSADR-EXTSIZE))
static char *pzLibDstBuf;
static long declen = 0;

static int dummy( zlibIO *zi ) {
	return 0xff;
}

static void DecodeProc1( zlibIO *zi ) {
	int n = zi->ptr - zi->ptr0;
	zi->ptr = zi->ptr0;
	memcpy( &pzLibDstBuf[declen], zi->ptr0, n );
	declen += n;
}

// pArcData: 圧縮データ
// pOutBuff: 展開したデータが入るバッファ
int unpack( char *pArcData, char *pOutBuff ) {
	zlibIO Zin, Zout;
	declen = 0;
	pzLibDstBuf = pOutBuff;
	Zin.ptr = (unsigned char *)(pArcData+36);
	Zin.ptre = (unsigned char *)-1;
	Zin.fn.fil = dummy;
	Zout.ptr0 = EXTBUFF;
	Zout.ptr = EXTBUFF;
	Zout.ptre = EXTBUFF + EXTSIZE;
	Zout.fn.fls = DecodeProc1;
	pceZlibExpand( &Zin, &Zout, WORKS );
	return declen;
}

/* ここまでzlib展開ルーチンサンプル(from P/ECE HAND BOOK Vol.2 緋色の霧) */


// 圧縮ファイルかどうか確認
BOOL ppack_checkHeader( unsigned char* arcData )
{
	if( *(long*)(arcData) == PPACK_HEADER ) { return TRUE; }
	return FALSE;
}


// 圧縮元ファイルの長さ取得
long ppack_getExpandSize( unsigned char* arcData )
{
	return( *(long*)(arcData+28) );
}


// 展開用ヒープ領域を確保してから展開
unsigned char* ppack_heapUnpack( unsigned char* arcData )
{
	unsigned char* ret = NULL;
	long size;
	
	if( !ppack_checkHeader( arcData ) ) { return NULL; }
	size = ppack_getExpandSize( arcData );
	if( ( ret = pceHeapAlloc( size ) ) == NULL ) { return NULL; }
	unpack( arcData, ret );
	return( ret );
}


// pvnspackでパックされた可能性のあるfpkファイルからデータを抽出
unsigned char* ppack_findPackData( unsigned char* packData, char* fName )
{
	unsigned char* ret = NULL;
	
	if( !strncmp( packData, "PVNA", 4 ) ) {
		memcpy( packData, "KAPF", 4 );
	}
	if( ( ret = FPK_FindPackData( packData, fName ) ) == NULL ) { return NULL; }
	if( !ppack_checkHeader( ret ) ) { return( ret ); }
	ret = ppack_heapUnpack( ret );
	return( ret );
}

