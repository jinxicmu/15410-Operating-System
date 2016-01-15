/** @file traceback.c
 *  @brief The traceback function
 *
 *  This file contains the traceback function for the traceback library
 *
 *  @author Harry Q. Bovik (hqbovik)
 *  @bug Unimplemented
 */

#include "traceback_internal.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int* read_ebp();
int get_func_in_table_given_addr(void* addr);
void print_func_to_file(int i, void* addr, FILE* fp);
void print_string(char* str, FILE* fp);
void print_string_array(argsym_t* argu, char** arr, FILE* fp);
void print_argu_to_file(argsym_t* argu, void* base, FILE* fp);
bool is_print_string(char* str);
void traceback(FILE *fp);

int get_func_in_table_given_addr(void* addr) {
	int i;	
	for (i = 0; i < FUNCTS_MAX_NUM; i++) {
		if (functions[i].name[0] == '\0') {
			break;
		}
		if (functions[i].addr == addr) {
			return i;
		}
	}
	return -1;	
}

void print_func_to_file(int i, void* addr, FILE* fp) {
	if (i == -1) {
		fprintf(fp, "Function %p(...), in\n", addr);
		return;	
	}
	functsym_t func = functions[i];	
	fprintf(fp, "Function %s", func.name);
	fprintf(fp, "(");
	int j;
	for (j = 0; func.args[j].name != '\0'; j++) {
		if (j != 0) {
			fprintf(fp, ", ");
		}
		print_argu_to_file(&(func.args[j]), addr, fp);
	}
	fprintf(fp, "), in\n");
}

void print_string(char* str, FILE* fp) {
	if (is_print_string(str)) {
		int i;
		fprintf(fp, "\"");
		for (i = 0; i < 25; i++) {
			if (str[i] == '\0') {
				fprintf(fp, "\"");
				return;
			}
			fprintf(fp, "%c", str[i]);
		}
		if (str[25] != '\0') {
			fprintf(fp, "...");
		}
		fprintf(fp, "\"");
	}						
	else {
		fprintf(fp, "%p", str);
	}
}

void print_string_array(argsym_t* argu, char** arr, FILE* fp) {
	int i = 0;
	fprintf(fp, "{");
	while (i < 4) {
		if (i == 3) {
			fprintf(fp, "...");
			break;
		}
		print_string(arr[i], fp);
		i++;
		if (arr[i] == NULL) {
			break;
		}
		fprintf(fp, ", ");
	}
	fprintf(fp, "}");
}

void print_argu_to_file(argsym_t* argu, void* base, FILE* fp) {
	void* addr = base + argu->offset;
	switch(argu->type) {
		case 0: fprintf(fp, "char %s=\'%c\'", argu->name, *(char*)addr);
		case 1: fprintf(fp, "int %s=%d", argu->name, *(int*)addr);
		case 2: fprintf(fp, "float %s=%f", argu->name, *(float*)addr);
		case 3: fprintf(fp, "double %s=%lf", argu->name, *(double*)addr);
		case 4: {
					fprintf(fp, "char* %s=", argu->name);
					print_string((char*)addr, fp);
				}
		case 5: print_string_array(argu, (char**)addr, fp);
		case 6: fprintf(fp, "void* %s=0v%x", argu->name, (unsigned int)addr);
		default: fprintf(fp, "unkown %s=0x%x", argu->name, (unsigned int)addr);
	}
}

bool is_print_string(char* str) {
	int i;
	for (i = 0; str[i] != '\0';i++) {
		if (isprint(str[i]) == 0) {
			return false;
		}
	}	
	return true;
}

void traceback(FILE *fp)
{
	/* the following just makes a sample access to "functions" array. 
	 * note if "functions" is not referenced somewhere outside the 
	 * file that it's declared in, symtabgen won't be able to find
	 * the symbol. So be sure to always do something with functions */

	/* remove this line once you've got real code here */
	void* p = (void*)read_ebp();
	int index = get_func_in_table_given_addr(p);
	print_func_to_file(index, p, fp);
}

