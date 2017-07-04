//------------------------------------------------------------------------
 //  Project Name   :  未定（仮）
  //  File Name      :  FilePack.c
   //  Copyright      :  AQUA (Leaf)  since 2001 - 
    //                                 Programd by.  Miyakusa Masakazu
     //------------------------------------------------------------------------
      // ★パックファイル関連

#include <string.h>
#include <stdlib.h>
#include <piece.h>
#include <ctype.h>

#define DWORD_CHAR(ch0, ch1, ch2, ch3)                              \
				(((DWORD)(BYTE)(ch0) << 24) | ((DWORD)(BYTE)(ch1) << 16) |   \
				 ((DWORD)(BYTE)(ch2) <<  8) | ((DWORD)(BYTE)(ch3)      ))

typedef struct {
	unsigned long	head;												//	パックヘッダ
	long			famount;											//	ファイル数
}FILE_PAC_INFO;

typedef struct {
	char			fname[16];											//	ファイル名（拡張子、'\0'含みで16文字まで）
	unsigned long	offset;												//	オフセットアドレス
	unsigned long	size;												//	ファイルサイズ
}FILE_INFO;


//-------------------------------------------------------
//	半角大文字を、小文字に変換
//-------------------------------------------------------
static void strlower( char *str )
{
	while( *str ){
		*str = (char)tolower( *str );
		str++;
	}

}

//-------------------------------------------------------
//	文字列比較
//-------------------------------------------------------
static int strcmpUL( char *str1, char *str2 )
{
	char	buf1[256], buf2[256];
	strcpy( buf1, str1 );
	strcpy( buf2, str2 );
	strlower( buf1 );
	strlower( buf2 );
	return strcmp( buf1, buf2 );
}

//	FPK_FindPackData
//		パックされたデータ領域から、指定のファイル名のデータが置かれている領域を
//		検出しポインタを返す
unsigned char *FPK_FindPackData( unsigned char *pack_data, char *fname )
{
	unsigned char	*ret=NULL;
	unsigned char	*root_ptr = pack_data;
	FILE_PAC_INFO	*fpi = (FILE_PAC_INFO *)pack_data;
	FILE_INFO		*fi;
	int				count;

	if( pack_data==NULL ) return NULL;
	if( fpi->head!=DWORD_CHAR('F','P','A','K') ) return NULL;	//ファイルパック形式か？
	
	pack_data += sizeof(FILE_PAC_INFO);

	count = 0;
	while( count<fpi->famount ){
		fi = (FILE_INFO *)pack_data;
		if( !strcmpUL( fi->fname, fname ) ){
			ret = root_ptr+fi->offset;
			break;
		}
		pack_data += sizeof(FILE_INFO);
		count++;
	}
	return ret;
}