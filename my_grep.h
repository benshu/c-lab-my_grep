#ifndef MYGREP_H
#define MYGREP_H

#include <stdio.h>
#include <stdlib.h>
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

#endif /* ifndef MYGREP_H */
