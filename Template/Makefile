# Nome do binário. Tem de ser "proj".
PROG=proj

# Indicar o caminho para todos os ficheiros a compilar.
.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/controllers/keyboard
.PATH: ${.CURDIR}/controllers/mouse
.PATH: ${.CURDIR}/controllers/timer
.PATH: ${.CURDIR}/controllers/video
.PATH: ${.CURDIR}/states/
.PATH: ${.CURDIR}/drawing/

# Ficheiros ".c" a compilar. Têm de estar contidos nas pastas indicadas no passo anterior.
SRCS = 	main.c timer.c utils.c graphics.c KBC.c keyboard.c mouse.c states.c menu.c

# Flags de compilação
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# Dependências
DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>
