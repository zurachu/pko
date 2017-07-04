# Piece �p makefile

# �g���q�̒�`

.SUFFIXES:
.SUFFIXES:  .o .s .c

# �����R�}���h�E�I�v�V�����̃}�N��

CC = c33-pcc
CFLAGS = -c -gp=0x0 -near -O2 -Wall
AS = c33-pcc
ASFLAGS = -c -gp=0x0 -near
LD = c33-pcc
LDFLAGS = -ls -lm

# �����K��

.c.o:
	$(CC) $(CFLAGS) $<

.s.o:
	$(AS) $(ASFLAGS) $<

# �\���t�@�C���E�����t�@�C���̃}�N��

PRGNAME = pko
FILENAME = pko
CAPTION = �o�E�j�E�n
ICON = uruto.pid
OBJS =	pko.o\
		main.o\
		stage1.o\
		stage2.o\
		stage3.o\
		stage4.o\
		gamelib.o\
		triangle.o\
		htomei2.o\
		myunpack.o\
		filepack.o\
		inflate.o\
		instdef2.o

$(PRGNAME).srf : $(OBJS)
	$(LD) $(LDFLAGS) -e$(PRGNAME).srf $(OBJS)

# �ˑ��֌W

# �t���b�V���������݃C���[�W����
pex : $(PRGNAME).srf
	ppack -e $(PRGNAME).srf -o$(FILENAME).pex -n$(CAPTION) -i$(ICON)

# �N���[���A�b�v
clean:
	del $(PRGNAME).srf
	del $(FILENAME).pex
	del $(PRGNAME).sym
	del $(PRGNAME).map
	del *.o

