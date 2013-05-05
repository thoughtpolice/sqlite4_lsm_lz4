CC=clang
CCOPTS=-std=gnu99 -Wno-incompatible-pointer-types -Wno-implicit-function-declaration -Wno-comment
LDOPTS = -lm -lpthread

## Flags used for debugging and/or optimization
#EXTRAOPTS=-O0 -g -fsanitize=address
EXTRAOPTS=-DNDEBUG -O3

CCOPTS+= $(EXTRAOPTS)

LSM_OPTS = -Ilsm lsm/sqlite4.c
LZ4_OPTS = -DUSE_LSM_LZ4_COMPRESSOR -Ilz4 lz4/lz4.c lsm-lz4.c

## Controlling verbosity
ifeq ($(V),1)
Q=
else
Q=@
endif

## Rules
all: lsm-test lsm-lz4-test
lsm-test: lsm-test.c
	$(Q)echo CC lsm-test
	$(Q)$(CC) $(CCOPTS) $(LSM_OPTS) lsm-test.c -o lsm-test $(LDOPTS)
lsm-lz4-test: lsm-test.c
	$(Q)echo CC lsm-lz4-test
	$(Q)$(CC) $(CCOPTS) $(LSM_OPTS) $(LZ4_OPTS) lsm-test.c -o lsm-lz4-test $(LDOPTS)
clean:
	$(Q)rm -f lsm-test lsm-lz4-test *.o *~ *.db*
