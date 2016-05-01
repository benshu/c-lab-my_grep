#ifndef MYREGEX_H
#define MYREGEX_H

#include <stdbool.h>

enum regex_type {
    simple_char,
    dot,
    brackets,
    parenthesis
};

typedef struct regex_struct{
    enum regex_type type;
    char single_char;
    char from;
    char to;
    struct regex_struct** optionA;
    struct regex_struct** optionB;
}Regex;


/*
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
 */


void free_regex(Regex** regex);
bool match_regex_at_place(Regex** regex, char* text, bool case_insensitive);
bool match_regex(Regex** regex, char* text, bool case_insensitive);
void parse_regex(Regex** regex_result, char* regex_str, bool is_escaped);

#endif /* ifndef MYREGEX_H */
