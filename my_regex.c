#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "my_regex.h"


bool chrcasecmp(char const a, char const b)
{
    int diff = tolower(a) - tolower(b);
    return diff == 0;
}

bool match_regex_at_place(Regex regex, char* text, bool case_insensitive)
{
    if (*regex == NULL)
        return true;
    if (text[0] != '\0'){
        if (regex[0]->type == brackets) {
            if(text[0] >= regex[0]->from && text[0] <= regex[0]->to )
                return true;
        }
        if (regex[0]->type == parenthesis) {
            if (match_regex_at_place(regex[0]->optionA, text, case_insensitive))
                return true;
            else if(match_regex_at_place(regex[0]->optionB, text, case_insensitive))
                return true;
            else return false;
        }
        else if(regex[0]->type == dot || text[0] == regex[0]->single_char ||
                (case_insensitive && chrcasecmp(text[0], regex[0]->single_char)))
            return match_regex_at_place(&regex[1], ++text, case_insensitive);
    }
    return false;
}



void generate_regex_struct(Regex new_regex, char *regex_str, enum regex_type type)
{
    *new_regex = malloc(sizeof(Regex_struct));
    if (type == simple_char) {
        new_regex[0]->type = simple_char;
        new_regex[0]->single_char = *regex_str;
    }
    if (type == parenthesis) {
        new_regex[0]->type = type;
        new_regex[0]->optionA = malloc(sizeof(Regex_struct*) * strlen(regex_str));
        new_regex[0]->optionB = malloc(sizeof(Regex_struct*) * strlen(regex_str));
    } if (type == brackets) {
        new_regex[0]->type = brackets;
        new_regex[0]->from = *(regex_str+1);
        new_regex[0]->to = *(regex_str+3);
    } if (type == dot) {
        new_regex[0]->type = dot;
    }
}

void parse_regex(Regex regex_result, char* regex_str, bool is_escaped)
{
    if (*regex_str == '\0' )
    {
        regex_result[0] = NULL;
        return;
    }
    if (is_escaped || !strchr(SPECIAL_CHARS, *regex_str))
    {
        generate_regex_struct(regex_result, regex_str, simple_char);
        parse_regex(++regex_result, regex_str + 1, false);
    }
    else if (*regex_str == '\\')
        parse_regex(regex_result, regex_str + 1, true);
    else
    {
        switch (*regex_str) {
            case '.':
                generate_regex_struct(regex_result, regex_str, dot);
                parse_regex(++regex_result, regex_str+1, false);
                break;
            case '[':
                generate_regex_struct(regex_result, regex_str, brackets);
                parse_regex(++regex_result, regex_str+4, false);
                break;
            case '(':
                generate_regex_struct(regex_result, regex_str, parenthesis);
                parse_regex(regex_result[0]->optionA, regex_str+1, false);
                char *after_pipe = strchr(regex_str, '|') + 1;
                parse_regex(regex_result[0]->optionB, after_pipe, false);
                char *after_parents = strchr(regex_str, ')') + 1;
                parse_regex(++regex_result, after_parents, false);
                break;
            case '|':
            case '{':
            case ']':
            case '}':
            case ')':
                *regex_result = NULL;
                break;
        }
    }

}

bool match_regex(Regex regex, char* text, bool case_insensitive)
{
    do {
        if (match_regex_at_place(regex, text, case_insensitive)) {
            return true;
        }
    } while (*text++ != '\0');
    return false;
}

void free_regex(Regex regex)
{
    Regex temp = regex;
    while (*temp != NULL) {
        if((*temp)->type == parenthesis)
        {
            free_regex((*temp)->optionA);
            free_regex((*temp)->optionB);
        }
        free(*temp);
        temp++;
    }
    free(regex);
}
