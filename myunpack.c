// ���k�W�J�֘A �@�\�g���� by�d��Chu

#include <string.h>
#include <piece.h>
#include "filepack.h"

#define PPACK_HEADER 0x1c0258

/* ��������zlib�W�J���[�`���T���v��(from P/ECE HAND BOOK Vol.2 ��F�̖�) */

#include "piecezl.h"
// 0x13e000�`0x13ffff �܂ŁA�W�J�p�o�b�t�@
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

// pArcData: ���k�f�[�^
// pOutBuff: �W�J�����f�[�^������o�b�t�@
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

/* �����܂�zlib�W�J���[�`���T���v��(from P/ECE HAND BOOK Vol.2 ��F�̖�) */


// ���k�t�@�C�����ǂ����m�F
BOOL ppack_checkHeader( unsigned char* arcData )
{
	if( *(long*)(arcData) == PPACK_HEADER ) { return TRUE; }
	return FALSE;
}


// ���k���t�@�C���̒����擾
long ppack_getExpandSize( unsigned char* arcData )
{
	return( *(long*)(arcData+28) );
}


// �W�J�p�q�[�v�̈���m�ۂ��Ă���W�J
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


// pvnspack�Ńp�b�N���ꂽ�\���̂���fpk�t�@�C������f�[�^�𒊏o
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

