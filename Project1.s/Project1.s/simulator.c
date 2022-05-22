/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure *not* to modify printState or any of the associated functions
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Machine Definitions
#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */

// File
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

enum op {Add = 0, Nor, Lw, Sw, Beq, Jalr, Halt, Noop};

void printState(stateType *);

int convertNum(int);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
    state.pc = 0;
    int instruct_count = 0;
    //set all registers to zero
    for (int i = 0; i < NUMREGS; i++){
        state.reg[i] = 0;
    }
    
    while (1) {
        //set up the vars
        int op = (state.mem[state.pc] >> 22) & 7;
        int regA = (state.mem[state.pc] >> 19) & 7;
        int regB = (state.mem[state.pc] >> 16) & 7;
        
        //third field - either offset or destR
        int arg3 = state.mem[state.pc] & 65535;

        printState(&state);
        
        //incorrect opcode, exit
        if (op < 0 || op > Noop){
            printf("error: unrecognized opcode\n");
            exit(1);
        }
        
        if (op == Halt){
            //increment pc and halt machine
            state.pc++;
            instruct_count++;
            break;
        }
        switch (op) {
            case Add:
                //regA + regB -> destR (arg3)
                state.reg[arg3] = state.reg[regA] + state.reg[regB];
                break;

            case Nor:
                //bitwise nor reg A | regB -> destR (arg3)
                state.reg[arg3] = ~(state.reg[regA] | state.reg[regB]);
                break;

            case Lw:
                //load regB with data stored at regA data + offset (arg3)
                state.reg[regB] = state.mem[convertNum(state.reg[regA] + arg3)]; //convert num <- 2s comp
                break;

            case Sw:
                //store regB's data at regA data + offset (arg3)
                state.mem[convertNum(state.reg[regA] + arg3)] = state.reg[regB]; //convert num <- 2s comp
                break;

            case Beq:
                //if regA contains same as regB, branch to PC + 1 + offset (arg3)
                if (state.reg[regA] == state.reg[regB]) {
                    state.pc += convertNum(arg3);
                }
                break;

            case Jalr:
                //pc + 1 -> regB then branch to address in reg A
                state.reg[regB] = state.pc + 1;
                state.pc = state.reg[regA] - 1;
                break;
            //Noop does nothing, let loop run and increment pc and instruction count
            }
        instruct_count++;
        state.pc++;

    }
    printf("machine halted\ntotal of %d instructions executed", instruct_count);
    printf("\nfinal state of machine:\n");
    printState(&state);
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num);
}

