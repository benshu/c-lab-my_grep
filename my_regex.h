#ifndef MYREGEX_H
#define MYREGEX_H

#include <stdbool.h>

#define SPECIAL_CHARS  ".][}{|)(\\"

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
}Regex_struct;

typedef Regex_struct** Regex;

void free_regex(Regex regex);
bool match_regex(Regex regex, char* text, bool case_insensitive);
void parse_regex(Regex regex_result, char* regex_str, bool is_escaped);

#endif /* ifndef MYREGEX_H */
