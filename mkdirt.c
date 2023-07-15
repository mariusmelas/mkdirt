#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>


/*
 * Replaces characters form given position on string
 * by another given string. 
 *
 */
void str_replace_end(char *str, char *new_end, int from_pos, int new_end_len) {
	int i;
	for(i = 0; i < new_end_len; i++) {
		*(str+from_pos+i) = *(new_end+i);
	}
	*(str+from_pos+i) = '\0';
}

/*
 * Makes new directories by a given pathstring
 */
void mkdirpath(char *treestring, char *pathstring, int last_slash_pos) {
	 
	char c;
	char dirname[256] = "";
	while(1) {

		/* Get next character */
		c = *(treestring++);
		
		/* Check if character has special meaning */
		if( c != '/' && c != '(' && c != ',' && c != '\0') {
			/* Add character to dirname */
			strncat(dirname, &c, 1);
		} else {
			
			/* Got the next directory, add it to the pathstring,  
			 * and create it.
			 */
			str_replace_end(pathstring, dirname, last_slash_pos + 1, strlen(dirname));
			mkdir(pathstring, 0777);
			
			/*
			 * Handle character with special meaning
			 */
			switch(c) {
				case '/':
					strncat(pathstring, &c, 1);
					last_slash_pos += strlen(dirname) + 1;
					/* reset dirname */
					dirname[0] = '\0';
					break;
				case ',':
					dirname[0] = '\0';
					break;
				case '(':

					/* Cr* Create a new substring *//

					char subtreestring[2048] = "";
					while((c = *(treestring++)) != ')') {
						strncat(subtreestring, &c, 1);
					}

					mkdirpath(subtreestring, pathstring, last_slash_pos);
					break;
				default:
					break;
			}
		}
		
		if(c == '\0') {
			return;
		}
		
	}

}


int main(int argc, char *argv[]) {
	
	if(argc != 2) {
		printf("Usage: mktree \"diretory tree\"\n");
		return -1;
	}

	char pathstring[4000] = "";
	mkdirpath(argv[1], pathstring, -1);
	
	return 0;
}
