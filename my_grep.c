#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// TDD -n line number -i case insensitive -E
int main(int argc, char *argv[])
{
	FILE *input_file;
	char *str_to_find=NULL, *line=NULL;
	size_t line_cap =0;
	int line_length=0, current_line_num=0,i=1;
	bool print_line_num=false, case_insensitive=false, regex=false;
	int print_multi_lines = 0;

	while (*(argv[i])=='-') {
		if (*(argv[i]+1)=='n')
			print_line_num = true;
		else if (*(argv[i]+1)=='i')
			case_insensitive = true;
		else if (*(argv[i]+1)=='E')
			regex = true;
		else if (*(argv[i]+1)=='A')
		{
			print_multi_lines = atoi((argv[++i]));
		}
		i++;
	}
	if (regex) {
		char current_char=0,*new_char=NULL;
		str_to_find = malloc(sizeof(argv[i]));
		new_char = str_to_find;
		while((current_char = *argv[i]++) != '\0')
		{
			if(current_char != '\\')
				*new_char++ = current_char;
		}
	} else {
		str_to_find = argv[i];
	}
	if (i<(argc-1)) {
		input_file = fopen(argv[++i],"r");
	} else
		input_file =stdin;
	int multi_lines_to_print = 0;
	while((line_length = getline(&line, &line_cap, input_file)) > 0){
		current_line_num++;
		if((case_insensitive && strcasestr(line,str_to_find)) || strstr(line,str_to_find)){
			multi_lines_to_print = print_multi_lines;
			if (print_line_num) {
				printf("%d:", current_line_num);
			}
			printf("%s",line);
		}else if(multi_lines_to_print > 0)
		{
			multi_lines_to_print--;
			if (print_line_num)
				printf("%d-", current_line_num);
			printf("%s",line);
		}

	}
	free(line);
	fclose(input_file);
	return 0;
}