#include <piece.h>
#include <string.h>
#include <musdef.h>

#define SECTOR_SIZE 4096		// �P�Z�N�^�̃T�C�Y

#define RAW_FILE_NAME "drum.bin"	// ���F�f�[�^�t�@�C��
#define RAW_FILE_SIZE 39758		// ���F�f�[�^�t�@�C���̃T�C�Y
#define ARCHIVE_FILE_NAME "drum.arc"	// ���k���F�f�[�^�t�@�C��
#define ARCHIVE_FILE_SIZE 29433		// ���k���F�f�[�^�t�@�C���̃T�C�Y

// �e���F�f�[�^�̃T�C�Y
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

// �h�����ȊO�̉��F�͂��̂܂܎g�p
extern INST i_square0;
extern INST i_saw0;
extern INST i_triangle0;
extern INST i_square;
extern INST i_saw;
extern INST i_triangle;

// �h�������F
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

// ���F�e�[�u��
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

// ���F�f�[�^�p�o�b�t�@
static signed char instBuffer[RAW_FILE_SIZE];

////////////////////////////////////////////////////////////////////////////////

// �t�@�C���̓ǂݍ���
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
  int count;	// �q�m�[�h�̏o���p�x�̘a
  int parent;	// �e�m�[�h��
  int left;	// �����̎q�m�[�h��
  int right;	// �E���̎q�m�[�h��
} NODE;

// �f�[�^�̓W�J
static BOOL decode(unsigned char *inputBuffer, int inputSize,
                   unsigned char *outputBuffer, int outputSize){
  NODE *node;
  int i, k, c, len, min1, min2, freeNode, root, parent, previous;
  static unsigned char bit1[8] = {128, 64, 32, 16, 8, 4, 2, 1};

  if(inputSize <= 2 * CHAR_SIZE){
    return FALSE;
  }

  // �n�t�}���ؗp�̗̈���m��
  node = (NODE *)pceHeapAlloc(NODE_SIZE * sizeof(NODE));

  // �e�����̏o���p�x���Z�b�g
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

  // �ԕ�
  node[NODE_SIZE - 1].count = 0x10000;

  // �n�t�}���؂�����
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

  // ������
  i = 0;
  c = 2 * CHAR_SIZE;
  previous = 0;
  for(len = 0; len < outputSize + 1; len++){

    // ������ɑΉ����镶���m�[�h�𓾂�
    k = root;
    do{
      parent = k;
      k = (*inputBuffer & bit1[i]) ? node[k].right : node[k].left;

      // ���蓾�Ȃ�
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

    // �e�o�C�g�̍��������̃f�[�^�ɖ߂�
    previous = (k + previous) % 0x100;
    *outputBuffer = previous;
    outputBuffer++;
  }

  // �n�t�}���ؗp�̗̈�����
  pceHeapFree(node);

  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////

int loadInst(void){
  int result;
  signed char *archiveBuffer, *p;

  // ���k���F�f�[�^�p�̗̈���m��
  archiveBuffer = (signed char *)pceHeapAlloc(ARCHIVE_FILE_SIZE);

  result = 1;

  // ���k���F�f�[�^�t�@�C��������Γǂݍ���
  if(readFile(ARCHIVE_FILE_NAME, archiveBuffer)){
    if(decode(archiveBuffer, ARCHIVE_FILE_SIZE, instBuffer, RAW_FILE_SIZE)){
      result = 0;
    }
  }

  // ���F�f�[�^�t�@�C��������Γǂݍ���
  if((result == 1) && readFile(RAW_FILE_NAME, instBuffer)){
    result = 0;
  }

  // ���F�f�[�^�t�@�C�����Ȃ���Ή��F�f�[�^���O�Ŗ��߂�
  if(result == 1){
    memset(instBuffer, 0, RAW_FILE_SIZE);
  }

  // ���k���F�f�[�^�p�̗̈�����
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