/**
 * Project 2
 * LC-2K Linker
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSIZE 300
#define MAXLINELENGTH 1000
#define MAXFILES 6
#define MASK 0xFFFF

typedef struct FileData FileData;
typedef struct SymbolTableEntry SymbolTableEntry;
typedef struct RelocationTableEntry RelocationTableEntry;
typedef struct CombinedFiles CombinedFiles;

struct SymbolTableEntry {
	char label[7];
	char location;
	int offset;
};

struct RelocationTableEntry {
	int offset;
	char inst[7];
	char label[7];
	int file;
};

struct FileData {
	int textSize;
	int dataSize;
	int symbolTableSize;
	int relocationTableSize;
	int textStartingLine; // in final executable
	int dataStartingLine; // in final executable
	int text[MAXSIZE];
	int data[MAXSIZE];
	SymbolTableEntry symbolTable[MAXSIZE];
	RelocationTableEntry relocTable[MAXSIZE];
};

struct CombinedFiles {
	int text[MAXSIZE];
	int data[MAXSIZE];
	SymbolTableEntry     symTable[MAXSIZE];
	RelocationTableEntry relocTable[MAXSIZE];
	int textSize;
	int dataSize;
	int symTableSize;
	int relocTableSize;
};
void global(CombinedFiles* comb, FileData* current_file, int* offset, char* current_label, int i, int j, int comb_data, int comb_text);
void local(FileData* current_file, int* current_offset, int i, int comb_text);
void combine(int fileNum, FileData* current_file, CombinedFiles* comb);

int define(FileData* file, char* label_in){
    for (int i = 0; i < file->symbolTableSize; i++){
        if (file->symbolTable[i].location == 'U'){
            continue;
        }
        if (!strcmp(file->symbolTable[i].label, label_in)){
            return 1;
        }
    }
    return 0;
}

int upperFirst(char* word){
    return (word[0] >= 'A' && word[0] <= 'Z');
}

int main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr; 
	int i, j;

	if (argc <= 2) {
		printf("error: usage: %s <obj file> ... <output-exe-file>\n",
				argv[0]);
		exit(1);
	}

	outFileString = argv[argc - 1];

	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	FileData files[MAXFILES];

  // read in all files and combine into a "master" file
	for (i = 0; i < argc - 2; i++) {
		inFileString = argv[i+1];

		inFilePtr = fopen(inFileString, "r");
		printf("opening %s\n", inFileString);

		if (inFilePtr == NULL) {
			printf("error in opening %s\n", inFileString);
			exit(1);
		}

		char line[MAXLINELENGTH];
		int sizeText, sizeData, sizeSymbol, sizeReloc;

		// parse first line of file
		fgets(line, MAXSIZE, inFilePtr);
		sscanf(line, "%d %d %d %d",
				&sizeText, &sizeData, &sizeSymbol, &sizeReloc);

		files[i].textSize = sizeText;
		files[i].dataSize = sizeData;
		files[i].symbolTableSize = sizeSymbol;
		files[i].relocationTableSize = sizeReloc;

		// read in text section
		int instr;
		for (j = 0; j < sizeText; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			instr = atoi(line);
			files[i].text[j] = instr;
		}

		// read in data section
		int data;
		for (j = 0; j < sizeData; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			data = atoi(line);
			files[i].data[j] = data;
		}

		// read in the symbol table
		char label[7];
		char type;
		int addr;
		for (j = 0; j < sizeSymbol; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%s %c %d",
					label, &type, &addr);
			files[i].symbolTable[j].offset = addr;
			strcpy(files[i].symbolTable[j].label, label);
			files[i].symbolTable[j].location = type;
		}

		// read in relocation table
		char opcode[7];
		for (j = 0; j < sizeReloc; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%d %s %s",
					&addr, opcode, label);
			files[i].relocTable[j].offset = addr;
			strcpy(files[i].relocTable[j].inst, opcode);
			strcpy(files[i].relocTable[j].label, label);
			files[i].relocTable[j].file	= i;
		}
		fclose(inFilePtr);
	} // end reading files

	// *** INSERT YOUR CODE BELOW ***
	//    Begin the linking process
	//    Happy coding!!!
    
    int fileNum = argc - 2;
    CombinedFiles comb; //store all linked values
    
    // error checking - inefficeint
    //loops through all obj files
    for (int i = 0; i < fileNum; i++){
        //loops through all the labels in symbol table of the file its in
        //check for stack label definition
        for (int j = 0; j < files[i].symbolTableSize; ++j){
            if (define(&files[i], "Stack")){
                printf("Error: Stack label defined by an object file\n");
                exit(1);
            }
            
            //loops through all object files, compares current file labels and all other ones
            //check for duplicate global definitions
            for (int k = 0; k < fileNum; ++k){
                if (i != k){
                    if (define(&files[i], files[i].symbolTable[j].label) && define(&files[k], files[i].symbolTable[j].label)){
                        printf("Error: duplicate defined global labels\n");
                        exit(1);
                    }
                }
            }
        }
    }
    
    //section sizes
    // get the total textSize of all files
    int comb_text = 0;
    int comb_data = 0;
    int comb_sym = 0;
    int comb_reloc = 0;
    for (int i = 0; i < fileNum; ++i){
        comb_text += files[i].textSize;
        comb_data += files[i].dataSize;
        comb_sym += files[i].symbolTableSize;
        comb_reloc += files[i].relocationTableSize;
    }

    //put everything but reloc in files into comb obj, updated addresses
    for (int i = 0; i < fileNum; ++i){
        //text to combine
        for (int j = 0; j < files[i].textSize; ++j){
            comb.text[comb.textSize + j] = files[i].text[j];
        }
        comb.textSize += files[i].textSize;
        
        //data to combine
        for (int j = 0; j < files[i].dataSize; ++j){
            comb.data[comb.dataSize + j] = files[i].data[j];
        }
        comb.dataSize += files[i].dataSize;
        
        //symbol table to combine
        for(int j = 0; j < files[i].symbolTableSize; ++j) {
            if (files[i].symbolTable[j].location != 'U'){
                int temp_offset = 0;
                if (files[i].symbolTable[j].location == 'T'){
                    temp_offset = files[i].symbolTable[j].offset + (comb.textSize - files[i].textSize);
                }
                else {
                    temp_offset = files[i].symbolTable[j].offset + (comb.dataSize - files[i].dataSize) + comb_text;
                }
                comb.symTable[comb.symTableSize] = files[i].symbolTable[j];
                comb.symTable[comb.symTableSize].offset = temp_offset;
                ++comb.symTableSize;
            }
        }
        files[i].textStartingLine = comb.textSize - files[i].textSize;
        files[i].dataStartingLine = comb.dataSize - files[i].dataSize;
        
    }
    
    //reloc table to combine
    for (int i = 0; i < fileNum; ++i){
        for(int j = 0; j < files[i].relocationTableSize; ++j) {
            char* current_label = files[i].relocTable[j].label;
            int* current_offset = &files[i].text[files[i].relocTable[j].offset];
            
            // i-type (not beq) so lw or sw
            if (!strcmp(files[i].relocTable[j].inst, "lw") || !strcmp(files[i].relocTable[j].inst, "sw")){
                //global TK
                if (upperFirst(current_label)){
                    global(&comb, &files[i], &files[i].text[files[i].relocTable[j].offset], current_label, i, j, comb_data, comb_text);
                }
                
                //local TK
                else {
                    local(&files[i], current_offset, i, comb_text);
                }
            }
        
            else if (!strcmp(files[i].relocTable[j].inst, ".fill")){
                current_offset = &files[i].data[files[i].relocTable[j].offset];
                //global
                if (upperFirst(current_label)){
                    global(&comb, &files[i], &files[i].data[files[i].relocTable[j].offset], current_label, i, j, comb_data, comb_text);
                }
                    
                //local
                else {
                    local(&files[i], current_offset, i, comb_text);
                }
            }
            
            else {
                printf("error with reloc table\n");
                exit(1);
            }
//            int temp = *current_offset;
//            temp = temp - 1;
        }
    }
    
    //TK
    int curr_text = 0;
    for (int k = 0; k < fileNum; k++){
        for (int i = 0; i < files[k].textSize; i++){
//            int temp = comb.text[curr_text + i];
//            int temp2 = files[k].text[i];
            comb.text[curr_text + i] = files[k].text[i];
        }
        curr_text += files[k].textSize;
    }
    
    int curr_data = 0;
    for (int k = 0; k < fileNum; k++){
        for (int i = 0; i < files[k].textSize; i++){
            comb.data[curr_data + i] = files[k].data[i];
        }
        curr_data += files[k].dataSize;
    }
        
    
    

    //print text
    for (int i = 0; i < comb.textSize; i++) {
        fprintf(outFilePtr, "%i\n", comb.text[i]);
    }

    // print data
    for (int i = 0; i < comb.dataSize; i++) {
        fprintf(outFilePtr, "%i\n", comb.data[i]);
    }
} // main

void global(CombinedFiles* comb, FileData* current_file, int* offset, char* current_label, int i, int j, int comb_data, int comb_text){
    int global_count = 0;
    int local_count = 0;
    //go until find same label
    for (; strcmp(current_label, comb->symTable[global_count].label); ++global_count){
        if (global_count >= comb->symTableSize){
            break;
        }
    }

    if (global_count == comb->symTableSize){
        if (!strcmp(current_label, "Stack")){
            *offset += comb_text + comb_data;
            //int temp = *offset;
        }
        else {
            printf("Error: undefined global label\n");
            exit(1);
        }
    }
    else {
        for (; strcmp(current_label, current_file->symbolTable[local_count].label); ++local_count){
            if (local_count >= current_file->symbolTableSize){
                break;
            }
        }
        if (current_file->symbolTable[local_count].location == 'D'){
//            int temp = *offset;
//            int temp2 = comb->symTable[global_count].offset;
//            int temp3 = current_file->textSize;
//            int temp4 = current_file->symbolTable[local_count].offset;
//            int temp5 = current_file->symbolTable[local_count].offset & MASK;
            *offset += comb->symTable[global_count].offset - (current_file->textSize + (current_file->symbolTable[local_count].offset & MASK)) ;
        }
        else{
            *offset += comb->symTable[global_count].offset - (current_file->symbolTable[local_count].offset & MASK);
        }
    }
}

void local(FileData* current_file, int* current_offset, int i, int comb_text){
        if ((*current_offset & MASK) < current_file->textSize)
        {
            *current_offset += current_file->textStartingLine;
        }
        else {
            *current_offset += (comb_text - current_file->textSize) + current_file->dataStartingLine;
        }
}

//void combine(int k, int size, int* index,  ){
//    for (int i = 0; i < size; i++)
//    {
//        comb->text[curr_text + i] = files[k].text[i];
//    }
//    for (int i = 0; i < files[k].dataSize; i++)
//    {
//        comb->data[curr_data + i] = files[k].data[i];
//    }
//    index += files[k].textSize;
//    curr_data += files[k].dataSize;
//}
