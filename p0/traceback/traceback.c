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
int get_func_in_table_given_addr(int* addr);
void print_func_to_file(int i, int* addr, FILE* fp);
void print_string(char* str, FILE* fp);
void print_string_array(char** arr, FILE* fp);
void print_argu_to_file(argsym_t argu, int* base, FILE* fp);
bool is_print_string(char* str);
void traceback(FILE *fp);
int* get_return_addr(int* ebp);
int* get_func_addr(int* ret_addr);
int* get_func_exit_addr(int* ret_addr);

int get_func_in_table_given_addr(int* addr) {
	int i;	
	for (i = 0; i < FUNCTS_MAX_NUM; i++) {
		if (functions[i].name[0] == '\0') {
			break;
		}
		if ((unsigned int)functions[i].addr > (unsigned int)addr) {
			return i - 1;
		}
	}
	return -1;	
}

void print_func_to_file(int i, int* addr, FILE* fp) {
	if (i == -1) {
		fprintf(fp, "Function %p(...), in\n", addr);
		return;	
	} else {
	functsym_t func = functions[i];	
	fprintf(fp, "Function %s", func.name);
	fprintf(fp, "(");
	int j;
	for (j = 0; j < ARGS_MAX_NUM && func.args[j].name[0] != '\0'; j++) {
		if (j != 0) {
			fprintf(fp, ", ");
		}
		print_argu_to_file(func.args[j], addr, fp);
	}
	if (func.args[0].name[0] == '\0') {
		fprintf(fp, "void");
	}
	fprintf(fp, "), in\n");
	}
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

void print_string_array(char** arr, FILE* fp) {
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

void print_argu_to_file(argsym_t argu, int* ebp, FILE* fp) {
	int* addr = (int*)((char*)ebp + argu.offset);
	switch(argu.type) {
		case 0: fprintf(fp, "char %s=\'%c\'", argu.name, *((char*)addr));
				break;
		case 1: fprintf(fp, "int %s=%d", argu.name, *((int*)addr));
				break;
		case 2: fprintf(fp, "float %s=%f", argu.name, *(float*)addr);
				break;
		case 3: fprintf(fp, "double %s=%lf", argu.name, *(double*)addr);
				break;
		case 4: fprintf(fp, "char *%s=", argu.name);
				print_string((char*)(*(unsigned long *)addr), fp);
				break;
		case 5: fprintf(fp, "char **%s=",argu.name);
				print_string_array((char**)(*(unsigned long *)addr), fp);
				break;
		case 6: fprintf(fp, "void *%s=0v%x", argu.name, (unsigned int)addr);
				break;
		default: fprintf(fp, "unkown %s=0x%p", argu.name, addr);
				break;
	}
}

bool is_print_string(char* str) {
	int i;
	for (i = 0; str[i] != '\0';i++) {
		if (isprint((int)str[i]) == 0) {
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
	int* ebp0 = read_ebp();
	int* ebp = (int*)(*ebp0);

	while(1) {
		int* ret_addr = get_return_addr(ebp);
		int index = get_func_in_table_given_addr(ret_addr);
		
		ebp = (int*)(*ebp);		
		print_func_to_file(index, ebp, fp);
		if (strcmp(functions[index].name, "main") == 0) {
			break;
		}	
		
	}	
}

int* get_return_addr(int* ebp) {
	// NOTE: Since ebp is int*, ebp + 1 actually leads to 4 byte increment.
	int ret_addr = *(ebp + 1);
	return (int*)ret_addr;
}

int* get_func_addr(int* ret_addr) {
	char* offset_addr = (char*)ret_addr - 4;
	return (int*)((char*)ret_addr + *(offset_addr));
}
