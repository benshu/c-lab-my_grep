#ifndef MYREGEX_H
#define MYREGEX_H

#include <stdbool.h>

/*
 *typedef struct params_struct{
 *    bool print_line_num;
 *    bool case_insensitive;
 *    bool regex;
 *    bool print_bytes_offset ;
 *    bool print_line_count;
 *    bool print_not_matching;
 *    bool strict_match_only;
 *    int print_multi_lines;
 *}Params_struct;
 *
 *typedef struct line{
 *    FILE *input_stream;
 *    char *line_content;
 *    size_t line_buffer_capacity;
 *    int bytes_offset;
 *    int line_num;
 *    int line_length;
 *    bool is_match_in_line;
 *
 *}Line;
 *
 */

bool match_regex_at_place(char* regex, char* text, bool case_insensitive);
bool match_regex(char* regex, char* text, bool case_insensitive);

#endif /* ifndef MYREGEX_H */
