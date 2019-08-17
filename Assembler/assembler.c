#include <stdio.h>
#include <stdlib.h>
#include "string.h"

//maximal line/label/memory space
#define LINE_LEN 500
#define LABEL_LEN 50
#define MEM_SPACE 65536


//struct command that contains a single command
//HEX - contains the hex representation of command fields, TXT- represents the text of a command field as given in asm file.
typedef struct Command
{
    char opcodeHEX;
    char rdHEX;
    char rsHEX;
    char rtHEX;
    char immHEX[4];
    char opcodeTXT[LINE_LEN+1];
    char rdTXT[LINE_LEN+1];
    char rsTXT[LINE_LEN+1];
    char rtTXT[LINE_LEN+1];
    char immTXT[LINE_LEN+1];;
} Command ;

//a struct which represents a label contains its name and it's location
typedef  struct Labels
{
    char lab_name[LABEL_LEN+1];
    int address;
} Labels;

/**
 * @param com - a Command*
 * empty all the variables of com
 */
void initCommand(Command* com)
{
    com->opcodeTXT[0] = '\0';
    com->rdTXT[0]= '\0';
    com->rtTXT[0]= '\0';
    com->rsTXT[0]= '\0';
    com->immTXT[0]= '\0';
}

/**
 * @param txt - char*
 * @return a char that represents text or W if text isn't known to assembler
 */
char TextToHex (char* txt)
{
    if (strcmp(txt,"$zero")==0 || strcmp(txt,"add")==0 || txt[0]=='\0') {
        return '0';
    } else if (strcmp(txt,"$at")==0 || strcmp(txt,"sub")==0) {
        return '1';
    } else if (strcmp(txt,"$v0")==0 || strcmp(txt,"and")==0) {
        return '2';
    } else if (strcmp(txt,"$a0")==0 || strcmp(txt,"or")==0) {
        return '3';
    } else if (strcmp(txt,"$a1")==0 || strcmp(txt,"sll")==0) {
        return '4';
    } else if (strcmp(txt,"$t0")==0 || strcmp(txt,"sra")==0) {
        return '5';
    } else if (strcmp(txt,"$t1")==0 || strcmp(txt,"limm")==0) {
        return '6';
    } else if (strcmp(txt,"$t2")==0 || strcmp(txt,"beq")==0) {
        return '7';
    } else if (strcmp(txt,"$t3")==0 || strcmp(txt,"bgt")==0) {
        return '8';
    } else if (strcmp(txt,"$s0")==0 || strcmp(txt,"ble")==0) {
        return '9';
    } else if (strcmp(txt,"$s1")==0 || strcmp(txt,"bne")==0) {
        return 'A';
    } else if (strcmp(txt,"$s2")==0 || strcmp(txt,"jal")==0) {
        return 'B';
    } else if (strcmp(txt,"$gp")==0 || strcmp(txt,"lw")==0) {
        return 'C';
    } else if (strcmp(txt,"$sp")==0 || strcmp(txt,"sw")==0) {
        return 'D';
    } else if (strcmp(txt,"$fp")==0 || strcmp(txt,"jr")==0) {
        return 'E';
    } else if (strcmp(txt,"$ra")==0 || strcmp(txt,"halt")==0) {
        return 'F';
    }
    else
    {
        return 'W';
    }
}

/**
 * @param text - char*
 * removes a comment from given text
 */
void removeComment(char* text)
{
    if (text[0]=='#')
    {
        text[0] ='\0';
    }
    else
    {
        strcpy(text,strtok(text,"#")); //remove # from buffer if yes skip iteration (if # is not at beginning)
        ///-> remove space at beginning
        if(text[0]==32)//deals with case where line contains spaces or just comment that doesnt start at beginning
        {
            int i=0;
            while (i<LINE_LEN+1)
            {
                if (text[i]==' ') //continue searching
                {
                    i++;
                    continue;
                }
                else
                {
                    if (text[i]=='\0') //line is empty
                    {
                        text[0]='\0';
                        break;
                    }
                    else //there is a command in line
                    {
                        break;
                    }
                }
            }
        }

    }

}

//check if label is in label array return label address if yes -1 otherwise
int labelInArray(char* txt, int labelNumber,Labels* labels)
{
    for (int i=0; i<labelNumber; i++)
    {
        if(!strcmp(labels[i].lab_name,txt))
        {
            return labels[i].address;
        }
    }
    return -1;
}

//turns labels/hex/decimal numbers to hex with len 4 or 8 (Txt is in len 50)
void ToHex(char* txt,int labelNum,Labels* labels,int len)
{
    int labelAddress;
    if (txt[0]=='0' && txt[1]=='x') //imm/.word content is in hex - remove first 2 chars witout changingt txt pointer
    {                              // text len needs to be <= len
        char temp[8] = { 0 };
        size_t txtLen = strlen(txt);
        for (int i=0; i<txtLen-2; i++)
        {
            temp[i] = txt[i+2];
        }
        int numOfZerosToAdd = len-((int)txtLen-2);
        for (int j=0; j<len;j++)
        {
            if (j<numOfZerosToAdd)
            {
                txt[j]='0';
            }
            else
            {
                txt[j] = temp[j-numOfZerosToAdd];
            }
        }
        txt[len] = '\0';
    }
    else
    {
        labelAddress = labelInArray(txt,labelNum,labels);
        if (labelAddress!=-1)  //txt is a imm and is a label
        {
            sprintf(txt,"%04X",labelAddress);
        }
        else //txt is a number for imm or content of .word
        {
            switch (len)
            {
                case 4://for imm
                    sprintf(txt,"%08X",atoi(txt));
                    txt[0]=txt[4];
                    txt[1]=txt[5];
                    txt[2]=txt[6];
                    txt[3]=txt[7];
                    txt[4]='\0';
                    break;
                case 8://for word
                    sprintf(txt,"%08X",atoi(txt));
                    break;
                default:
                    break;
            }
        }
    }
}

/**
 * @param txt - a string
 * @return check if string contains label. return ":" position if yes. return -1 otherwise
 */
int checkLabel(char* txt)
{
    for (int i=0; i<strlen(txt); i++)
    {
        if (txt[i]==':')
        {
            return i;
        }
    }
    return -1;
}


/**
 * @param strings
 * removes commas from a string
 */
void removeComma (char* str)
{
    for (int i=0; i<strlen(str); i++)
    {
        if (str[i] == ',')
        {
            str[i] = ' ';
        }
    }
}

/**
 *
 * @param labeles - empty array of labels*
 * @param program - the file which we read asm program from
 * @return number of labels in array
 */
int CreateLabelList (Labels* labels,FILE* program)
{
    char buffer[LINE_LEN+1] = { 0 };
    buffer[LINE_LEN] = '\0';
    int lineCounter = 0;
    int skipCounter = 0;
    int labelCounter = 0;
    int LabelEndLoc;
    int containsCom;
    int i;
    char word1[LABEL_LEN+1] = { 0 };
    char word2[LABEL_LEN+1] = { 0 };
    word1[LABEL_LEN] = '\0';
    word2[LABEL_LEN] = '\0';
    while(fgets(buffer,LINE_LEN,program)!=NULL)
    {

        removeComment(buffer);
        removeComma(buffer);
        i=0;
        containsCom = 0;
        word1[0] = '\0';
        word2[0] = '\0';
        sscanf(buffer,"%s %s",word1,word2);
        if ((TextToHex(word1)!='W'&&word1[0]!='\0') || ((TextToHex(word2)!='W') &&word2[0]!='\0')) //the command will be first or second word
        {
            containsCom =1;
        }
        LabelEndLoc = checkLabel(word1);
        if (LabelEndLoc!=-1 && labelCounter<MEM_SPACE) //preform if the line contains a label - add label to label array
        {
            word1[LabelEndLoc] = '\0';
            strcpy(labels[labelCounter].lab_name,word1);
            labels[labelCounter].address = lineCounter -  skipCounter;
            labelCounter++;
        }
        if (!containsCom)
        {
            skipCounter++;
        }
        lineCounter++;
    }
    rewind(program);
    return(labelCounter);
}

//changes txt to int. if txt is in hex so change to decimal
//txt will be positive number represented as char* in hex or in dec
//returns int
int ToInt(char* txt)
{
    if ((txt[0]=='0') && (txt[1]=='x'))
    {
        return  (int)strtol(txt+2,NULL,16);
    }
    else
    {
        return atoi(txt);
    }
}

//adds txt to memory array at i position
void addMem(char* memory,char* txt, int i)
{
    i = i*8;
    for(int j=0; j<8;j++)
    {
        memory[i+j]=txt[j];
    }
}

//writes the program to memory return memory size on success. -1 otherwise
int WriteToMem (FILE* program,char* memory, Labels* labels,int labelNumber)
{
    int maxMemIndex = 0;
    int wordAdd; //keeps the address where to insert word
    char* buffer = (char*)calloc(LINE_LEN+1, sizeof(char));
    if (buffer==NULL)
    {
        printf("ERROR: unable to allocate memory\n");
        return -1;
    }
    Command* com = (Command*)calloc(1, sizeof(Command));
    if (com==NULL)
    {
        printf("ERROR: unable to allocate memory\n");
        free(buffer);
        return -1;
    }
    char writeBuffer[9] = {0};
    int LabelEndLoc;
    int memoryInd = 0;
    char* bufferAdd = buffer;
    int lineNum = 0; //variable for error message
    while(fgets(buffer,LINE_LEN,program)!=NULL)
    {
        lineNum++;
        removeComment(buffer);
        if(strcmp(buffer,"\n")==0)
        {
            continue;
        }
        LabelEndLoc = checkLabel(buffer);
        if(LabelEndLoc!=-1) //remove label from line
        {
            buffer = buffer + LabelEndLoc + 1;
        }
        initCommand(com);
        removeComma(buffer);
        sscanf(buffer,"%s %s %s %s %s",com->opcodeTXT,com->rdTXT,com->rsTXT,com->rtTXT,com->immTXT);
        if (com->opcodeTXT[0] =='\0') //com->opcodeTXT will be \0 iff the line is used for label or empty
        {
            continue;
        }
        if(strcmp(com->opcodeTXT,".word")==0 )
        {
            ToHex(com->rsTXT,labelNumber,labels,8);
            wordAdd = ToInt(com->rdTXT); //contains address to insert word
            if (wordAdd>maxMemIndex)
            {
                maxMemIndex = wordAdd;
            }
            addMem(memory,com->rsTXT,wordAdd);
            continue;
        }
        else
        {
            //break down line to segments
            com->opcodeHEX = TextToHex(com->opcodeTXT);
            com->rdHEX = TextToHex(com->rdTXT);
            com->rsHEX = TextToHex(com->rsTXT);
            com->rtHEX = TextToHex(com->rtTXT);
            if (com->rtHEX=='W'||com->rdHEX=='W'||com->rsHEX=='W'||com->rtHEX=='W') //unknown text
            {
                printf("ERROR: Source file is in wrong format: %s\n       Line: %d", buffer,lineNum);
                free(buffer);
                free(com);
                return -1;
            }
            ToHex(com->immTXT,labelNumber,labels,4);
            strcpy(com->immHEX,com->immTXT);
            sprintf(writeBuffer,"%c%c%c%c%s",com->opcodeHEX,com->rdHEX,com->rsHEX,com->rtHEX,com->immHEX); //load hex line to write buffer
            if (memoryInd>maxMemIndex)
            {
                maxMemIndex = memoryInd;
            }
            addMem(memory,writeBuffer,memoryInd); //save line to memory
            memoryInd++;
        }
    }
    free(com);
    free(bufferAdd);
    return maxMemIndex;
}



int main(int argc, char** argv)
{
    char* memory = (char*)calloc(MEM_SPACE*8, sizeof(char));
    if (memory==NULL)
    {
        printf("ERROR: unable to allocate memory\n");
        return 1;
    }


    Labels* labels = (Labels*)calloc(MEM_SPACE, sizeof(Labels));
    if (labels==NULL)
    {
        printf("ERROR: unable to allocate memory\n");
        return 1;
    }


    FILE* memin = fopen(argv[2],"w");
    if (memin==NULL)
    {
        printf("ERROR: file open fail\n");
        free(memory);
        free(labels);
        return 1;

    }
    FILE* program =fopen(argv[1],"r");
    if (program==NULL)
    {
        printf("ERROR: file open fail\n");
        free(memory);
        free(labels);
        fclose(memin);
        return 1;
    }

    int labelNumber = CreateLabelList(labels,program);
    int lastIndInMem = WriteToMem(program,memory,labels,labelNumber);
    if(lastIndInMem==-1)
    {
        free(memory);
        free(labels);
        fclose(memin);
        fclose(program);
        return 1;

    }

    char writeBuffer[9] = {0};
    writeBuffer[8] = '\n';
    //WRITE MEM TO FILE
    //memSize+1
    for (int i=0; i<=lastIndInMem;i++)
    {
        if (memory[i*8]=='\0') //mem is empty
        {
            fwrite("00000000\n",1,9,memin);
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
            fwrite(writeBuffer,1,9,memin);
        }

    }
    free(memory);
    free(labels);
    fclose(memin);
    fclose(program);
    return 0;
}



