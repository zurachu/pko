# Piece 用 makefile

# 拡張子の定義

.SUFFIXES:
.SUFFIXES:  .o .s .c

# 生成コマンド・オプションのマクロ

CC = c33-pcc
CFLAGS = -c -gp=0x0 -near -O2 -Wall
AS = c33-pcc
ASFLAGS = -c -gp=0x0 -near
LD = c33-pcc
LDFLAGS = -ls -lm

# 生成規則

.c.o:
	$(CC) $(CFLAGS) $<

.s.o:
	$(AS) $(ASFLAGS) $<

# 構成ファイル・生成ファイルのマクロ

PRGNAME = pkotrial
FILENAME = pkotrial
CAPTION = Ｐ・Ｋ・Ｏ体験版
ICON = kamyu.pid
OBJS =	pko.o\
		main.o\
		stage1.o\
		gamelib.o\
		triangle.o\
		htomei2.o\
		myunpack.o\
		filepack.o\
		inflate.o\
		instdef2.o

$(PRGNAME).srf : $(OBJS)
	$(LD) $(LDFLAGS) -e$(PRGNAME).srf $(OBJS)

# 依存関係

# フラッシュ書き込みイメージ生成
pex : $(PRGNAME).srf
	ppack -e $(PRGNAME).srf -o$(FILENAME).pex -n$(CAPTION) -i$(ICON)

# クリーンアップ
clean:
	del $(PRGNAME).srf
	del $(FILENAME).pex
	del $(PRGNAME).sym
	del $(PRGNAME).map
	del *.o

