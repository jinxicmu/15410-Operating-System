/** @file traceback.c
 *  @brief The traceback function
 *
 *  This file contains the traceback function for the traceback library
 *
 *  @author Harry Q. Bovik (hqbovik)
 *  @bug Unimplemented
 */

#include "traceback_internal.h"
#include "string.h"
#include "ctype.h"

#define min(a, b) (((a) > (b)) ? (b) : (a))

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

void print_func_to_file(int i) {
	
}

bool is_print_string(char* str) {
	
}

void traceback(FILE *fp)
{
	/* the following just makes a sample access to "functions" array. 
	 * note if "functions" is not referenced somewhere outside the 
	 * file that it's declared in, symtabgen won't be able to find
	 * the symbol. So be sure to always do something with functions */

	/* remove this line once you've got real code here */
	printf("first function in table name: %s\n", functions[0].name);
	int i;
	for (i = 0; i < min(FUNCTS_MAX_NUM, 100); i++) {
		functsym_t cur = functions[i];
		if (strlen(cur.name) == 0) {
			break;
		}
		printf("\nThe %d function in table:\n",i);
		printf("\tName: %s\n", functions[i].name);
		printf("\tAddress: 0x%p\n", functions[i].addr);
		printf("\tArgument: ");
		int j;
		for (j = 0; j < sizeof(functions[i].args); j++) {
				if (strlen(functions[i].args[j].name) == 0) {
					break;
				}
			printf(" offset:%04x  %d %s\t", functions[i].args[j].offset, functions[i].args[j].type, functions[i].args[j].name);	
		}		
	}
}

