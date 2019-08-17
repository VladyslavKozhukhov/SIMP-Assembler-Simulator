CC = gcc
                       

EXEC_ASSEMBLER = sim
CC_COMP_FLAG = -std=c99

all:$(EXEC_SIM) $(EXEC_ASSEMBLER)

$(EXEC_ASSEMBLER): simulator.c
	$(CC) $(CC_COMP_FLAG) simulator.c -o $@

clean:
	rm -f sim
