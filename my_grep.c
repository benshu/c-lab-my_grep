#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//
// TODO - fix -b for stdin
//
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

int multi_lines_to_print; // TODO - refactor this global to someplace better

typedef struct line{
	FILE *input_stream;
	char *line_content;
	size_t line_buffer_capacity;
	int line_num;
	int line_length;
	bool is_match_in_line;

}Line;

Params_struct params = {false,false,false,false,false,false,false,0};

int parse_arguments(char *argv[])
{
	int i=1;
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
	return i;
}

bool is_match_in_line(char *line, char *str_to_find)
{
	if (params.case_insensitive)
		if (params.strict_match_only)
			return !strcasecmp(line,str_to_find);
		else
			return strcasestr(line,str_to_find);
	else
		if (params.strict_match_only)
			return !strcmp(line,str_to_find);
		else
			return strstr(line,str_to_find);

}

int report_line_match(Line *current_line)
{


	if((current_line->is_match_in_line && !params.print_not_matching) ||
			(!current_line->is_match_in_line && params.print_not_matching)){
		multi_lines_to_print = params.print_multi_lines;
		if (params.print_line_num) {
			printf("%d:", current_line->line_num);
		}
		if (params.print_bytes_offset) {
			printf("%lu:", ftell(current_line->input_stream) - current_line->line_length);
		}
		if (params.print_line_count) {
			return 1;
		} else
			printf("%s\n",current_line->line_content);
	}else if(multi_lines_to_print > 0)
	{
		multi_lines_to_print--;
		if (params.print_line_num)
			printf("%d-", current_line->line_num);
		printf("%s\n",current_line->line_content);
	}
	return 0;
}

void replace_newline_with_nullbyte(char *line){
	while (*line != '\0') {
		if (*line=='\n') {
			*line='\0';
			break;
		}
		line++;
	}
}

int main(int argc, char *argv[])
{
	FILE *input_file;
	char *str_to_find=NULL;
	int current_line_num=0,i=1;
	i=parse_arguments(argv);

	if (params.regex) { //TODO - refactor this
		char current_char=0,*new_char=NULL;
		str_to_find = malloc(sizeof(argv[i]));
		new_char = str_to_find;
		while((current_char = *argv[i]++) != '\0')
		{
			if(current_char != '\\')
				*new_char++ = current_char;
		}
		*new_char = '\0';
	} else {
		str_to_find = argv[i];
	}

	if (i<(argc-1)) {
		input_file = fopen(argv[++i],"r");
	} else
		input_file =stdin;

	int matching_lines_count=0;

	Line current_line = {
		input_file,
		NULL,
		0,
		0,
		0,
		false };

	while((current_line.line_length = getline(&(current_line.line_content), &(current_line.line_buffer_capacity), input_file)) > 0){
		current_line.line_num = ++current_line_num;
		replace_newline_with_nullbyte(current_line.line_content);
		current_line.is_match_in_line= is_match_in_line(current_line.line_content, str_to_find);
		matching_lines_count += report_line_match(&current_line);
	}
	if (params.print_line_count) {
		printf("%d\n",matching_lines_count);
	}
	if(params.regex)
		free(str_to_find);
	free(current_line.line_content);
	fclose(input_file);
	return 0;
}