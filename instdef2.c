#include <piece.h>
#include <string.h>
#include <musdef.h>

#define SECTOR_SIZE 4096		// １セクタのサイズ

#define RAW_FILE_NAME "drum.bin"	// 音色データファイル
#define RAW_FILE_SIZE 39758		// 音色データファイルのサイズ
#define ARCHIVE_FILE_NAME "drum.arc"	// 圧縮音色データファイル
#define ARCHIVE_FILE_SIZE 29433		// 圧縮音色データファイルのサイズ

// 各音色データのサイズ
#define SIZE_BD909    2641
#define SIZE_CYMBD    9795
#define SIZE_HANDCLAP 2153
#define SIZE_HC909    1427
#define SIZE_HO909    5665
#define SIZE_SD909    2254
#define SIZE_SDGATE   5823
#define SIZE_TOMH1    2360
#define SIZE_TOML1    4524
#define SIZE_TOMM1    3116

////////////////////////////////////////////////////////////////////////////////

// ドラム以外の音色はそのまま使用
extern INST i_square0;
extern INST i_saw0;
extern INST i_triangle0;
extern INST i_square;
extern INST i_saw;
extern INST i_triangle;

// ドラム音色
INST i_BD909    = {0, 0, 0, 0, PITCH16K, PITCH_C4, 0, 0, SIZE_BD909    << 14};
INST i_CYMBD    = {0, 0, 0, 0, PITCH16K, PITCH_C4, 0, 0, SIZE_CYMBD    << 14};
INST i_HANDCLAP = {0, 0, 0, 0, PITCH16K, PITCH_C4, 0, 0, SIZE_HANDCLAP << 14};
INST i_HC909    = {0, 0, 0, 0, PITCH16K, PITCH_C4, 0, 0, SIZE_HC909    << 14};
INST i_HO909    = {0, 0, 0, 0, PITCH16K, PITCH_C4, 0, 0, SIZE_HO909    << 14};
INST i_SD909    = {0, 0, 0, 0, PITCH16K, PITCH_C4, 0, 0, SIZE_SD909    << 14};
INST i_SDGATE   = {0, 0, 0, 0, PITCH16K, PITCH_C4, 0, 0, SIZE_SDGATE   << 14};
INST i_TOMH1    = {0, 0, 0, 0, PITCH16K, PITCH_C4, 0, 0, SIZE_TOMH1    << 14};
INST i_TOML1    = {0, 0, 0, 0, PITCH16K, PITCH_C4, 0, 0, SIZE_TOML1    << 14};
INST i_TOMM1    = {0, 0, 0, 0, PITCH16K, PITCH_C4, 0, 0, SIZE_TOMM1    << 14};

// 音色テーブル
INST *inst[] = {
  &i_square0,
  &i_saw0,
  &i_triangle0,
  &i_square,
  &i_saw,
  &i_triangle,
  &i_BD909,
  &i_SDGATE,
  &i_SD909,
  &i_HO909,
  &i_HC909,
  &i_CYMBD,
  &i_CYMBD,
  &i_TOMH1,
  &i_TOMM1,
  &i_TOML1,
  &i_HANDCLAP
};

// 音色データ用バッファ
static signed char instBuffer[RAW_FILE_SIZE];

////////////////////////////////////////////////////////////////////////////////

// ファイルの読み込み
static BOOL readFile(char *fileName, void *buffer){
  FILEACC facc;
  int i, sectors;

  if(pceFileOpen(&facc, fileName, FOMD_RD) == 0){

    sectors = (facc.fsize + SECTOR_SIZE - 1) / SECTOR_SIZE;

    for(i = 0; i < sectors; i++){
      pceFileReadSct(&facc, buffer + i * SECTOR_SIZE, i, SECTOR_SIZE);
    }

    pceFileClose(&facc);

    return TRUE;
  }

  else{
    return FALSE;
  }
}

////////////////////////////////////////////////////////////////////////////////

#define CHAR_SIZE 0x100
#define NODE_SIZE (2 * CHAR_SIZE + 2)

typedef struct {
  int count;	// 子ノードの出現頻度の和
  int parent;	// 親ノードへ
  int left;	// 左側の子ノードへ
  int right;	// 右側の子ノードへ
} NODE;

// データの展開
static BOOL decode(unsigned char *inputBuffer, int inputSize,
                   unsigned char *outputBuffer, int outputSize){
  NODE *node;
  int i, k, c, len, min1, min2, freeNode, root, parent, previous;
  static unsigned char bit1[8] = {128, 64, 32, 16, 8, 4, 2, 1};

  if(inputSize <= 2 * CHAR_SIZE){
    return FALSE;
  }

  // ハフマン木用の領域を確保
  node = (NODE *)pceHeapAlloc(NODE_SIZE * sizeof(NODE));

  // 各文字の出現頻度をセット
  for(i = 0; i < NODE_SIZE; i++){
    node[i].count = 0;
  }
  for(i = 0; i < CHAR_SIZE; i++){
    node[i].count = 0x100 * *inputBuffer;
    inputBuffer++;
    node[i].count += *inputBuffer;
    inputBuffer++;
  }

  // EOF
  node[CHAR_SIZE].count = 1;

  // 番兵
  node[NODE_SIZE - 1].count = 0x10000;

  // ハフマン木をつくる
  for(freeNode = CHAR_SIZE + 1; freeNode < NODE_SIZE - 1; freeNode++){
    min1 = NODE_SIZE - 1;
    min2 = NODE_SIZE - 1;

    for(i = 0; i < NODE_SIZE - 1; i++){
      if(node[i].count > 0){
        if(node[i].count < node[min1].count){
          min2 = min1;
          min1 = i;
        }
        else if(node[i].count < node[min2].count){
          min2 = i;
        }
      }
    }

    if(min2 == NODE_SIZE - 1){
      break;
    }

    node[freeNode].left = min1;
    node[freeNode].right = min2;
    node[freeNode].count = node[min1].count + node[min2].count;
    node[min1].parent = freeNode;
    node[min2].parent = freeNode;
    node[min1].count = 0;
    node[min2].count = 0;
  }
  root = min1;

  // 復号化
  i = 0;
  c = 2 * CHAR_SIZE;
  previous = 0;
  for(len = 0; len < outputSize + 1; len++){

    // 符号語に対応する文字ノードを得る
    k = root;
    do{
      parent = k;
      k = (*inputBuffer & bit1[i]) ? node[k].right : node[k].left;

      // あり得ない
      if(k >= parent){
        return FALSE;
      }

      i++;
      if(i == 8){
        i = 0;
        inputBuffer++;
        c++;

        if(c > inputSize){
          return FALSE;
        }
      }

    }while (k > CHAR_SIZE);

    if(k == CHAR_SIZE){
      break;
    }

    // 各バイトの差分を元のデータに戻す
    previous = (k + previous) % 0x100;
    *outputBuffer = previous;
    outputBuffer++;
  }

  // ハフマン木用の領域を解放
  pceHeapFree(node);

  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////

int loadInst(void){
  int result;
  signed char *archiveBuffer, *p;

  // 圧縮音色データ用の領域を確保
  archiveBuffer = (signed char *)pceHeapAlloc(ARCHIVE_FILE_SIZE);

  result = 1;

  // 圧縮音色データファイルがあれば読み込み
  if(readFile(ARCHIVE_FILE_NAME, archiveBuffer)){
    if(decode(archiveBuffer, ARCHIVE_FILE_SIZE, instBuffer, RAW_FILE_SIZE)){
      result = 0;
    }
  }

  // 音色データファイルがあれば読み込み
  if((result == 1) && readFile(RAW_FILE_NAME, instBuffer)){
    result = 0;
  }

  // 音色データファイルがなければ音色データを０で埋める
  if(result == 1){
    memset(instBuffer, 0, RAW_FILE_SIZE);
  }

  // 圧縮音色データ用の領域を解放
  pceHeapFree(archiveBuffer);

  p = instBuffer;
  i_BD909.pData = p;

  p += SIZE_BD909;
  i_CYMBD.pData = p;

  p += SIZE_CYMBD;
  i_HANDCLAP.pData = p;

  p += SIZE_HANDCLAP;
  i_HC909.pData = p;

  p += SIZE_HC909;
  i_HO909.pData = p;

  p += SIZE_HO909;
  i_SD909.pData = p;

  p += SIZE_SD909;
  i_SDGATE.pData = p;

  p += SIZE_SDGATE;
  i_TOMH1.pData = p;

  p += SIZE_TOMH1;
  i_TOML1.pData = p;

  p += SIZE_TOML1;
  i_TOMM1.pData = p;

  return result;
}
