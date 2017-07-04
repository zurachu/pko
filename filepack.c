//------------------------------------------------------------------------
 //  Project Name   :  ����i���j
  //  File Name      :  FilePack.c
   //  Copyright      :  AQUA (Leaf)  since 2001 - 
    //                                 Programd by.  Miyakusa Masakazu
     //------------------------------------------------------------------------
      // ���p�b�N�t�@�C���֘A

#include <string.h>
#include <stdlib.h>
#include <piece.h>
#include <ctype.h>

#define DWORD_CHAR(ch0, ch1, ch2, ch3)                              \
				(((DWORD)(BYTE)(ch0) << 24) | ((DWORD)(BYTE)(ch1) << 16) |   \
				 ((DWORD)(BYTE)(ch2) <<  8) | ((DWORD)(BYTE)(ch3)      ))

typedef struct {
	unsigned long	head;												//	�p�b�N�w�b�_
	long			famount;											//	�t�@�C����
}FILE_PAC_INFO;

typedef struct {
	char			fname[16];											//	�t�@�C�����i�g���q�A'\0'�܂݂�16�����܂Łj
	unsigned long	offset;												//	�I�t�Z�b�g�A�h���X
	unsigned long	size;												//	�t�@�C���T�C�Y
}FILE_INFO;


//-------------------------------------------------------
//	���p�啶�����A�������ɕϊ�
//-------------------------------------------------------
static void strlower( char *str )
{
	while( *str ){
		*str = (char)tolower( *str );
		str++;
	}

}

//-------------------------------------------------------
//	�������r
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
//		�p�b�N���ꂽ�f�[�^�̈悩��A�w��̃t�@�C�����̃f�[�^���u����Ă���̈��
//		���o���|�C���^��Ԃ�
unsigned char *FPK_FindPackData( unsigned char *pack_data, char *fname )
{
	unsigned char	*ret=NULL;
	unsigned char	*root_ptr = pack_data;
	FILE_PAC_INFO	*fpi = (FILE_PAC_INFO *)pack_data;
	FILE_INFO		*fi;
	int				count;

	if( pack_data==NULL ) return NULL;
	if( fpi->head!=DWORD_CHAR('F','P','A','K') ) return NULL;	//�t�@�C���p�b�N�`�����H
	
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