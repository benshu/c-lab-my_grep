#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct params_struct{
	bool print_line_num;
	bool case_insensitive;
	bool regex;
	bool print_bytes_offset ;
	bool print_line_count;
	bool print_not_matching;
	bool strict_match_only;
	int print_multi_lines;
}Params_struct;

	Params_struct params = {};

void parse_arguments(char *argv[])
{
	int i=0;
	printf("%s\n", argv[0]);
	while (*(argv[i])=='-') {
		if (*(argv[i]+1)=='n')
			params.print_line_num = true;
		else if (*(argv[i]+1)=='i')
			params.case_insensitive = true;
		else if (*(argv[i]+1)=='E')
			params.regex = true;
		else if (*(argv[i]+1)=='A')
			params.print_multi_lines = atoi((argv[++i]));
		else if (*(argv[i]+1)=='b')
			params.print_bytes_offset = true;
		else if (*(argv[i]+1)=='c')
			params.print_line_count = true;
		else if (*(argv[i]+1)=='v')
			params.print_not_matching = true;
		else if (*(argv[i]+1)=='x')
			params.strict_match_only = true;
		i++;
	}
}
int main(int argc, char *argv[])
{
	FILE *input_file;
	char *str_to_find=NULL, *line=NULL;
	size_t line_cap =0;
	int line_length=0, current_line_num=0,i=1;
	parse_arguments(argv);

	if (params.regex) {
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
		if (params.case_insensitive)
			if (params.strict_match_only)
				is_match_in_line = !strcasecmp(line,str_to_find);
			else
				is_match_in_line = strcasestr(line,str_to_find);
		else
			if (params.strict_match_only)
				is_match_in_line = !strcmp(line,str_to_find);
			else
				is_match_in_line = strstr(line,str_to_find);

		if((is_match_in_line && !params.print_not_matching) || (!is_match_in_line && params.print_not_matching)){
			multi_lines_to_print = params.print_multi_lines;
			if (params.print_line_num) {
				printf("%d:", current_line_num);
			}
			if (params.print_bytes_offset) {
				printf("%lu:", ftell(input_file) - line_length);
			}
			if (params.print_line_count) {
				matching_lines_count++;
			} else
				printf("%s\n",line);
		}else if(multi_lines_to_print > 0)
		{
			multi_lines_to_print--;
			if (params.print_line_num)
				printf("%d-", current_line_num);
			printf("%s\n",line);
		}
	}
	if (params.print_line_count) {
		printf("%d\n",matching_lines_count );
	}
	if(params.regex)
		free(str_to_find);
	free(line);
	fclose(input_file);
	return 0;
}