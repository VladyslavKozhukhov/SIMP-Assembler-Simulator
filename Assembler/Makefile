CC = gcc
                       

EXEC_ASSEMBLER = asm
CC_COMP_FLAG = -std=c99

all:$(EXEC_SIM) $(EXEC_ASSEMBLER)

$(EXEC_ASSEMBLER): assembler.c
	$(CC) $(CC_COMP_FLAG) assembler.c -o $@

clean:
	rm -f asm
