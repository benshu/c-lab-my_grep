#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
	FILE *input_file;
	char *str_to_find=NULL, *line=NULL;
	size_t line_cap =0;
	int line_length=0, current_line_num=0,i=1;
	bool print_line_num=false, case_insensitive=false, regex=false, print_bytes_offset =false;
	bool print_line_count=false, print_not_matching=false, strict_match_only = false ;
	int print_multi_lines = 0;

	while (*(argv[i])=='-') {
		if (*(argv[i]+1)=='n')
			print_line_num = true;
		else if (*(argv[i]+1)=='i')
			case_insensitive = true;
		else if (*(argv[i]+1)=='E')
			regex = true;
		else if (*(argv[i]+1)=='A')
			print_multi_lines = atoi((argv[++i]));
		else if (*(argv[i]+1)=='b')
			print_bytes_offset = true;
		else if (*(argv[i]+1)=='c')
			print_line_count = true;
		else if (*(argv[i]+1)=='v')
			print_not_matching = true;
		else if (*(argv[i]+1)=='x')
			strict_match_only = true;
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
	int matching_lines_count=0;
	bool is_match_in_line=false;
	char *temp_char=NULL;

	while((line_length = getline(&line, &line_cap, input_file)) > 0){
		current_line_num++;
		i=0;
		while (*(temp_char = &line[i++]) != '\0') {
			if (*temp_char=='\n') {
				*temp_char = '\0';
			}
		}
		if (case_insensitive)
			if (strict_match_only)
				is_match_in_line = !strcasecmp(line,str_to_find);
			else
				is_match_in_line = strcasestr(line,str_to_find);
		else
			if (strict_match_only)
				is_match_in_line = !strcmp(line,str_to_find);
			else
				is_match_in_line = strstr(line,str_to_find);

		if((is_match_in_line && !print_not_matching) || (!is_match_in_line && print_not_matching)){
			multi_lines_to_print = print_multi_lines;
			if (print_line_num) {
				printf("%d:", current_line_num);
			}
			if (print_bytes_offset) {
				printf("%lu:", ftell(input_file) - line_length);
			}
			if (print_line_count) {
				matching_lines_count++;
			} else
				printf("%s\n",line);
		}else if(multi_lines_to_print > 0)
		{
			multi_lines_to_print--;
			if (print_line_num)
				printf("%d-", current_line_num);
			printf("%s\n",line);
		}
	}
	if (print_line_count) {
		printf("%d\n",matching_lines_count );
	}
	if(regex)
		free(str_to_find);
	free(line);
	fclose(input_file);
	return 0;
}