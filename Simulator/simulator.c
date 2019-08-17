#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MEM_SIZE 65536
#define NUM_OF_GEGISTERS 16
#define PC_START_LOCATION 0
#define $RA 15
#define TRACE_BUFFER_LEN (NUM_OF_GEGISTERS*8)+34 //+36 for spaces pc and command


//define all opcodes
#define ADD 0
#define SUB 1
#define AND 2
#define OR 3
#define SLL 4
#define SRA 5
#define LIMM 6
#define BEQ 7
#define BGT 8
#define BLE 9
#define BNE 10
#define JAL 11
#define LW 12
#define SW 13
#define JR 14


static int lastIndInMem = 0; //saves last index to know when to stop writing to memout

/*
 * gets txt that is a hex number
 * return: integer that represents hex number in 2's complement (imm range -32678<->32677 / rd,rs,rt range 0<->15)
 */
int toInt(char* txt,int numOfBits)
{
    if(numOfBits == 16)
    {
        return (int16_t)strtol(txt,NULL,16);
    }
    else
    {
        return (int)strtoul(txt,NULL,16);
    }
}

/**
 * @param hex char array
 * @param int number
 * number will be used to insert the the hex number that it represents in  hex
 */
void ToHex(char* hex, int num) //ok with 2 complement
{
    sprintf(hex,"%08X",num);
}

/*
 * gets momory pointer, byte pointer and index
 * inserts the byte in memory at correct index
 */
void addByteToArr (char* arr,char* byte, int index)
{
    arr[index] = byte[0];
    arr[index+1] = byte[1];
    arr[index+2] = byte[2];
    arr[index+3] = byte[3];
    arr[index+4] = byte[4];
    arr[index+5] = byte[5];
    arr[index+6] = byte[6];
    arr[index+7] = byte[7];
}
void addCommandToMem (char* arr, char* com, int index)
{
    addByteToArr(arr,com,index*8);
}

//gets a byte from array and puts it in byte string
void getComFromMem (char* mem,char* com, int index)
{
    com[0] = mem[index*8];
    com[1] = mem[index*8+1];
    com[2] = mem[index*8+2];
    com[3] = mem[index*8+3];
    com[4] = mem[index*8+4];
    com[5] = mem[index*8+5];
    com[6] = mem[index*8+6];
    com[7] = mem[index*8+7];
}

//returns pcLoc or -1 to halt simulator
int performCommand(char* command,int OpCode,int* registers,int pcLoc,char* memory)
{
    //split command and convert to int
    char temp[5] = {0};
    temp[0] = command[1];
    int rd = toInt(temp,16); //initiate rd
    temp[0] = command[2];
    int rs  = toInt(temp,16);//initiate rs
    temp[0] = command[3];
    int rt = toInt(temp,16);//initiate rs
    temp[0] = command[4];
    temp[1] = command[5];
    temp[2] = command[6];
    temp[3] = command[7];
    int imm = toInt(temp,16);

    char memCom[9] = {0}; //temp char* that contains command for sw operation
    int index;
    unsigned int lwInd;
    switch (OpCode)
    {
        case ADD:
            registers[rd] = registers[rs] + registers[rt];
            pcLoc+=1;
            break;
        case SUB:
            registers[rd] = registers[rs] - registers[rt];
            pcLoc+=1;
            break;
        case AND:
            registers[rd] = registers[rs] & registers[rt];
            pcLoc+=1;
            break;
        case OR:
            registers[rd] = registers[rs] | registers[rt];
            pcLoc+=1;
            break;
        case SLL:
            registers[rd] = registers[rs] << registers[rt];
            pcLoc+=1;
            break;
        case SRA:
            registers[rd] = registers[rs] >> registers[rt];
            pcLoc+=1;
            break;
        case LIMM:
            registers[rd] = imm;
            pcLoc+=1;
            break;
        case BEQ:
            if (registers[rs] == registers[rt])
            {
                pcLoc = imm;
            }
            else
            {
                pcLoc+=1;
            }
            break;
        case BGT:
            if (registers[rs] > registers[rt])
            {
                pcLoc = imm;
            }
            else
            {
                pcLoc+=1;
            }
            break;
        case BLE:
            if (registers[rs] <= registers[rt])
            {
                pcLoc = imm;
            }
            else
            {
                pcLoc+=1;
            }
            break;
        case BNE:
            if (registers[rs] != registers[rt])
            {
                pcLoc = imm;
            }
            else
            {
                pcLoc+=1;
            }
            break;
        case JAL:
            registers[$RA] = pcLoc +1;
            pcLoc = imm;
            break;
        case LW:
            getComFromMem(memory,memCom,(registers[rs]+imm)&0xffff);
            registers[rd] = toInt(memCom,32);
            pcLoc+=1;
            break;
        case SW:
            ToHex(memCom,registers[rd]); //put in comToMem rd in hex
            index = registers[rs]+imm;
            if (index>lastIndInMem)
            {
                lastIndInMem = index;
            }
            addCommandToMem(memory,memCom,index);
            pcLoc+=1;
            break;
        case JR:
            pcLoc = registers[rd];
            break;
        default:
            perror("not a legal command");
            exit(1);
    }
    registers[0] = 0; //change in case of change
    return pcLoc;
}



void createMem(FILE* memin, char* mem)
{
    char buffer[10] = {0}; //10 because 2 last chars are \n and \000
    int index = 0;
    while(fgets(buffer,10,memin)!=NULL && index<MEM_SIZE)
    {

        addCommandToMem(mem,buffer,index);
        lastIndInMem = index;
        index++;
    }
    strcpy(buffer,"00000000");
    while (index<MEM_SIZE) //add zeros to memory
    {
         addCommandToMem(mem,buffer,index);
         index++;
    }
    fclose(memin);
}



int getOpcode (char* byte)
{
    char opcode[2] = {0};
    opcode[0] = byte[0];
    return toInt(opcode,16);
}

void addTrace (FILE* trace, int* regArr, int pcLoc,char* command)
{
    char tempReg[9] = {0};
    char buffer[TRACE_BUFFER_LEN] = { 0 };
    buffer[TRACE_BUFFER_LEN - 1] = '\n';
    //add pc and command to buffer
    ToHex(tempReg,pcLoc);
    addByteToArr(buffer,tempReg,0);
    addByteToArr(buffer,command,9);
    buffer[8] = ' ';
    buffer[17] = ' ';

    for (int i = 0; i < NUM_OF_GEGISTERS; i++)
    {
        ToHex(tempReg, regArr[i]);
        addByteToArr(buffer,tempReg,i*8+i+18);
        if (i!=NUM_OF_GEGISTERS-1)
        {
            buffer[i*8+i+26] = ' ';
        }
    }
    fwrite(buffer,1,TRACE_BUFFER_LEN,trace);
}

void writeMemToOutFile(char* memory, FILE* memout)
{
    char writeBuffer[9] = {0};
    writeBuffer[8] = '\n';
    for (int i=0; i<=lastIndInMem;i++)
    {
        if (memory[i*8]=='\0')
        {
            fwrite("00000000\n",1,9,memout);
        }
        else
        {
            writeBuffer[0] = memory[i*8];
            writeBuffer[1] = memory[i*8+1];
            writeBuffer[2] = memory[i*8+2];
            writeBuffer[3] = memory[i*8+3];
            writeBuffer[4] = memory[i*8+4];
            writeBuffer[5] = memory[i*8+5];
            writeBuffer[6] = memory[i*8+6];
            writeBuffer[7] = memory[i*8+7];
            fwrite(writeBuffer,1,9,memout);
        }

    }
}
/**
 *
 * @param comCount
 * @param count
 * writes command count to count file
 */
void writeComCount(int comCount,FILE* count)
{
    char countSt[6] = {0};
    sprintf(countSt,"%d",comCount);
    fwrite(countSt,1,strlen(countSt),count);
}

void  writeRegout(int* registers,FILE* regout)
{
    char buffer[9] = {0};
    for (int i=0; i<NUM_OF_GEGISTERS; i++)
    {
        ToHex(buffer,registers[i]);
        buffer[8] = '\n';
        fwrite(buffer,1,9,regout);
    }

}
/*
 * return 0 on success 1 otherwise
 */
int main(int argc, char** argv)
{
    int* registers = (int*)calloc(NUM_OF_GEGISTERS, sizeof(int)); //each reg is 32 bit - 4 byte
    if (registers==NULL)
    {
        perror("memory allocation error");
        return 1;
    }
    char* memory = (char*)calloc(MEM_SIZE*8, sizeof(char));
    if (memory==NULL)
    {
        free(registers);
        printf("ERROR: unable to allocate memory\n");
        return 1;
    }
    FILE* memin = fopen(argv[1],"r");
    if (memin==NULL)
    {
        free(memory);
        free(registers);
        perror("memin open fail");
        return 1;
    }
    FILE* memout = fopen(argv[2],"w");
    if (memout==NULL)
    {
        free(memory);
        free(registers);
        fclose(memin);
        perror("memout open fail");
        return 1;
    }
    FILE* regout = fopen(argv[3],"w");
    if (regout==NULL)
    {
        free(memory);
        free(registers);
        fclose(memin);
        fclose(memout);
        perror("regout open fail");
        return 1;
    }
    FILE* trace = fopen(argv[4],"w");
    if (trace==NULL)
    {
        free(memory);
        free(registers);
        fclose(memin);
        fclose(memout);
        fclose(regout);
        perror("trace open fail");
        return 1;
    }
    FILE* count = fopen(argv[5],"w");
    if (count==NULL)
    {
        free(memory);
        free(registers);
        fclose(memin);
        fclose(memout);
        fclose(regout);
        fclose(trace);
        perror("trace open fail");
        return 1;
    }

    createMem(memin,memory);
    char command[9] = { 0 }; //temporary string that contains byte
    int pcLoc = PC_START_LOCATION;

    getComFromMem(memory,command,pcLoc);
    int comCode = getOpcode(command);
    int comCount = 0;
    while (comCode!=15)
    {
        addTrace(trace,registers,pcLoc,command);
        pcLoc = performCommand(command,comCode,registers,pcLoc,memory);
        comCount++;
        getComFromMem(memory,command,pcLoc);
        comCode = getOpcode(command);

    }
    addTrace(trace,registers,pcLoc,command); //add trace after halting
    comCount++; //count halt command
    writeMemToOutFile(memory,memout);
    writeComCount(comCount,count);
    writeRegout(registers,regout);

    free(memory);
    free(registers);
    fclose(memout);
    fclose(regout);
    fclose(trace);
    fclose(count);
    return 0;
}