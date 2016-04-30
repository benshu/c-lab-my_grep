#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "my_grep.h"

//
// TODO - fix -b for stdin
//

int parse_arguments(char *argv[])
{
    int next_argument=1;
    while (*(argv[next_argument])=='-') {
        if (*(argv[next_argument]+1)=='n')
            params.print_line_num = true;
        else if (*(argv[next_argument]+1)=='i')
            params.case_insensitive = true;
        else if (*(argv[next_argument]+1)=='E')
            params.regex = true;
        else if (*(argv[next_argument]+1)=='A')
            params.print_multi_lines = atoi((argv[++next_argument]));
        else if (*(argv[next_argument]+1)=='b')
            params.print_bytes_offset = true;
        else if (*(argv[next_argument]+1)=='c')
            params.print_line_count = true;
        else if (*(argv[next_argument]+1)=='v')
            params.print_not_matching = true;
        else if (*(argv[next_argument]+1)=='x')
            params.strict_match_only = true;
        next_argument++;
    }
    return next_argument;
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
            printf("%d:", current_line->bytes_offset - current_line->line_length);
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
        if (params.print_bytes_offset) {
            printf("%d-", current_line->bytes_offset - current_line->line_length);
        }
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

char* parse_str_to_find(char *argv[], int next_argument_idx)
{
    char *str_to_find = NULL;

    char current_char=0, *new_char=NULL;
    str_to_find = malloc(sizeof(argv[next_argument_idx]));
    new_char = str_to_find;
    while((current_char = *argv[next_argument_idx]++) != '\0')
    {
        if(current_char != '\\')
            *new_char++ = current_char;
        else if(*argv[next_argument_idx] == '\\')
            *new_char++ = current_char;
    }
    *new_char = '\0';
    return str_to_find;
}

FILE *parse_input_file(char argc, char *argv[], int next_argument_idx)
{
    if (next_argument_idx<(argc-1)) {
        return fopen(argv[++next_argument_idx],"r");
    } else {
        return stdin;
    }
}

int read_line(FILE* input_file, Line *current_line)
{
    current_line->line_length = getline(&(current_line->line_content),
                                        &(current_line->line_buffer_capacity),
                                        input_file);
    current_line->bytes_offset += current_line->line_length;
    current_line->line_num++;
    return current_line->line_length;
}

int main(int argc, char *argv[])
{
    FILE *input_file;
    char *str_to_find=NULL;
    int next_argument_idx=0;
    int matching_lines_count = 0;

    next_argument_idx = parse_arguments(argv);

    str_to_find = parse_str_to_find(argv, next_argument_idx);

    input_file = parse_input_file(argc, argv, next_argument_idx);

    Line curr_line = {
        input_file,   // *input_stream;
        NULL,         // *line_content;
        0,            // line_buffer_capacity;
        0,            // bytes_offset;
        0,            // line_num;
        0,            // line_length;
        false };      // is_match_in_line;

    while(read_line(input_file, &curr_line) > 0){
        replace_newline_with_nullbyte(curr_line.line_content);
        curr_line.is_match_in_line = is_match_in_line(curr_line.line_content,
                                                      str_to_find);
        matching_lines_count += report_line_match(&curr_line);
    }

    if (params.print_line_count) {
        printf("%d\n",matching_lines_count);
    }

    free(str_to_find);
    free(curr_line.line_content);
    fclose(input_file);
    return 0;
}