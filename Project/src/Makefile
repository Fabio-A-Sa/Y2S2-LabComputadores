# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/controllers/keyboardMouse
.PATH: ${.CURDIR}/controllers/serialPort
.PATH: ${.CURDIR}/controllers/timer
.PATH: ${.CURDIR}/controllers/video
.PATH: ${.CURDIR}/controllers/serialPort
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/game/modes
.PATH: ${.CURDIR}/sprites

# source code files to be compiled  serial_pPOrt.Port.c
SRCS = 	main.c timer.c utils.c drawing.c menu.c graphics.c sprites.c keyboard.c mouse.c reader.c game.c guessing.c serialPort.c queue.c inst.c

# ./controllers/keyboardMouse/KBC.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
