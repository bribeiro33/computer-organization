/**
 * Project 1 
 * Assembler code fragment for LC-2K 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

typedef struct labelsStruct{
    char lName[MAXLINELENGTH];
    int lAdd;
} labels;

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int rtype(int opcode, char* arg0,char* arg1,char* arg2);
int lsw(struct labelsStruct* labelVec, int label_count, int opcode, char* arg0, char* arg1, char* arg2);
int beq(struct labelsStruct* labelVec, int label_count, int pc, char* arg0, char* arg1, char* arg2);
int jtype(char* arg0, char* arg1);
int otype(int opcode);
long fill(struct labelsStruct* labelVec, int label_count, char* arg0);

int main(int argc, char *argv[])
{
    // Given - file opening and reading
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }
    
    //1st pass
    labels labelVec[1000];
    int line = 0;
    int count = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if (strcmp(label, "")){
            for (int i = 0; i < count; i++){
                if (!strcmp(labelVec[i].lName, label)){
                    printf("error: duplicate label");
                    exit(1);
                }
            }
            if (strlen(label) > 6){
                printf("error: label too long");
                exit(1);
            }
            //copy label name and address into label vec
            strcpy(labelVec[count].lName, label);
            labelVec[count].lAdd = line;
            count++;
        }
        line++;
    }
        
    rewind(inFilePtr);
    line = 0;
    //2nd pass
    int pc = 0;
    int machine_code = 0;
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if (!strcmp(opcode, "add")){
            machine_code = rtype(0, arg0, arg1, arg2);
            fprintf(outFilePtr,"%d\n", machine_code);
        }
        else if (!strcmp(opcode, "nor")){
            machine_code = rtype(1, arg0, arg1, arg2);
            fprintf(outFilePtr,"%d\n", machine_code);
        }
        else if (!strcmp(opcode, "lw")){
            machine_code = lsw(labelVec, count, 2, arg0, arg1, arg2);
            fprintf(outFilePtr,"%d\n", machine_code);
        }
        else if (!strcmp(opcode, "sw")){
            machine_code = lsw(labelVec, count, 3, arg0,  arg1, arg2);
            fprintf(outFilePtr,"%d\n", machine_code);
        }
        else if (!strcmp(opcode, "beq")){
            machine_code = beq(labelVec, count, pc, arg0, arg1, arg2);
            fprintf(outFilePtr,"%d\n", machine_code);
        }
        else if (!strcmp(opcode, "jalr")){
            machine_code = jtype(arg0, arg1);
            fprintf(outFilePtr,"%d\n", machine_code);
        }
        else if (!strcmp(opcode, "halt")){
            machine_code = otype(6);
            fprintf(outFilePtr,"%d\n", machine_code);
        }
        else if (!strcmp(opcode, "noop")){
            machine_code = otype(7);
            fprintf(outFilePtr,"%d\n", machine_code);
        }
        else if (!strcmp(opcode, ".fill")){
            long mc = 0;
            mc = fill(labelVec, count, arg0);
            fprintf(outFilePtr,"%ld\n", mc);
        }
        else {
            printf("error: unrecognized opcode");
            exit(1);
        }
        pc++;
    }
    return(0);
}

int rtype(int opcode, char* arg0,char* arg1,char* arg2){
    //31-25 unused, 24-22 opcode, 21-19 regA, 18-16 regB, 15-1 unused, 2-0 destR
    //opcode: 0b001
    int machine_code = 0;
    int op = (opcode << 22);
    int regA = (atoi(arg0) << 19);
    int regB = (atoi(arg1) << 16);
    int destR = atoi(arg2);
    
    machine_code = op | regA | regB | destR;

    return machine_code;
}

int lsw(struct labelsStruct* labelVec, int label_count, int opcode, char* arg0, char* arg1, char* arg2){
    //31-25 unused, 24-22 opcode, 21-19 regA, 18-16 regB, 15-0 offset
    //opcodes: lw:0b010 sw:0b011
    int machine_code = 0;
    int offset = -1;
    int op = (opcode << 22);
    int regA = (atoi(arg0) << 19);
    int regB = (atoi(arg1) << 16);
    
    //numeric address
    if (isNumber(arg2)){
        offset = atoi(arg2);
        if (offset <= -32768 || offset >= 32767){
            printf("error: offset too big \n");
            exit(1);
        }
        offset &= 0xFFFF; //two's complement
    }
    
    //symbolic address
    else {
        for (int i = 0; i < label_count; i++){ //find line address of label
            if (!strcmp(labelVec[i].lName, arg2)){
                offset = labelVec[i].lAdd;
                break;
            }
        }
        if (offset == -1){
            printf("error: undefined label");
            exit(1);
        }
        if (offset <= -32768 || offset >= 32767){
            printf("error: offset too large");
            exit(1);
        }
        offset = offset & 65535; //memory - 1
    }
    
    machine_code = op | regA | regB | offset;
    
    return machine_code;
}

int beq(struct labelsStruct* labelVec, int label_count, int pc, char* arg0, char* arg1, char* arg2){
    //31-25 unused, 24-22 opcode, 21-19 regA, 18-16 regB, 15-0 offset
    //opcode: 0b100
    int machine_code = 0;
    int offset = -1;
    int op = (4 << 22);
    int regA = (atoi(arg0) << 19);
    int regB = (atoi(arg1) << 16);
    
    //numeric address
    if (isNumber(arg2)){
        offset = atoi(arg2);
        if (offset <= -32768 || offset >= 32767){
            printf("error: offset too big \n");
            exit(1);
        }
        offset = offset & 0xFFFF; //two's complement
    }
    
    //symbolic address
    else {
        int address = -1;
        for (int i = 0; i < label_count; i++){ //find line address of label
            if (!strcmp(labelVec[i].lName, arg2)){
                address = labelVec[i].lAdd;
                break;
            }
        }
        if (address == -1){
            printf("error: undefined label");
            exit(1);
        }
        offset = address - pc - 1;
        if (offset <= -32768 || offset >= 32767){
            printf("error: offset too large");
            exit(1);
        }
        
        offset = offset & 65535;
    }
    
    machine_code = op | regA | regB | offset;
    return machine_code;
}

int jtype(char* arg0, char* arg1){
    //31-25 unused, 24-22 opcode, 21-19 regA, 18-16 regB, 15-0 unused
    //opcode: 0b101
    int machine_code = 0;
    int op = 5 << 22;
    int regA = atoi(arg0) << 19;
    int regB = atoi(arg1) << 16;
    machine_code = op | regA | regB;
    return machine_code;
}

int otype(int opcode){
    //31-25 unused, 24-22 opcode, 21-0 unused
    //opcodes- halt: 6 noop: 7
    int machine_code = 0;
    machine_code += (opcode << 22);
    return machine_code;
}

long fill(struct labelsStruct* labelVec, int label_count, char* arg0){
    long machine_code = 0;
    if (isNumber(arg0)){
        machine_code = atol(arg0);
        long long mc = 0;
        if (mc <= -2147483648 || mc >= 2147483647){
            printf("error: out of bounds\n");
            exit(1);
        }
    }
    else {
        for (int i = 0; i < label_count; i++){
            if (!strcmp(labelVec[i].lName, arg0)){
                machine_code = labelVec[i].lAdd;
                break;
            }
        }
    }

    return machine_code;
}
/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}


