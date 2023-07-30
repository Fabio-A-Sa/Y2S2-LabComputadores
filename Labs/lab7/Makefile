# Nome do binário. Tem de ser "proj".
PROG=proj

# Ficheiros ".c" a compilar
SRCS = 	lab7.c sp.c queue.c utils.c

# Flags de compilação
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

# Dependências
DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>
