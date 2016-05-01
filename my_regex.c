#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h> // DEBUG - remove later
#include "my_regex.h"

#define SPECIAL_CHARS  ".][}{|)(\\"

bool chrcasecmp(char const a, char const b)
{
    int diff = tolower(a) - tolower(b);
    return diff == 0;
}

bool match_regex_at_place(Regex** regex, char* text, bool case_insensitive)
{
    if (*regex == NULL)
            return true;
    if (text[0] != '\0'){
        if ((*regex)->type == brackets) {
           if(text[0] >=(*regex)->from && text[0] <= (*regex)->to )
               return true;
        }
        if ((*regex)->type == parenthesis) {
            //printf("IT's PARNETSSS TIME");
            if (match_regex_at_place((*regex)->optionA, text, case_insensitive))
               return true;
           else if(match_regex_at_place((*regex)->optionB, text, case_insensitive))
               return true;
           else return false;
        }
       else if((*regex)->type == dot || text[0] == (*regex)->single_char ||
               (case_insensitive && chrcasecmp(text[0], (*regex)->single_char)))
        return match_regex_at_place(++regex, ++text, case_insensitive);
    }
    return false;
}

void free_regex(Regex** regex)
{
    Regex** temp = regex;
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


/*
 *void generate_regex_struct(Regex* new_regex, char *regex_str, enum regex_type type)
 *{
 *
 *    new_regex = malloc(sizeof(Regex));
 *    (new_regex)->type = type;
 *    (new_regex)->single_char = *regex_str;
 *    (new_regex)->content = NULL;
 *
 *   }
 *}
 */
void parse_regex(Regex** regex_result, char* regex_str, bool is_escaped)
{
    //printf("PARSING %c \n", *regex_str);
    if (*regex_str == '\0' )
    {
        *regex_result = NULL;
        return;
    }
    if (is_escaped || !strchr(SPECIAL_CHARS, *regex_str))
    {
        *regex_result = malloc(sizeof(Regex));
        (*regex_result)->type = simple_char;
        (*regex_result)->single_char = *regex_str;
        parse_regex(++regex_result, regex_str + 1, false);
    } else
    {
        switch (*regex_str) {
            case '\\':
                parse_regex(regex_result, regex_str + 1, true);
                break;
            case '.':
                *regex_result = malloc(sizeof(Regex));
                (*regex_result)->type = dot;
                (*regex_result)->single_char = *regex_str;
                parse_regex(++regex_result, regex_str+1, false);
                break;
            case '[':
                //generate_regex_struct(*regex_result, brackets);
                *regex_result = malloc(sizeof(Regex));
                (*regex_result)->type = brackets;
                (*regex_result)->from = *(regex_str+1);
                (*regex_result)->to = *(regex_str+3);
                parse_regex(++regex_result, regex_str+4, false);
                break;
            case '(':
                //printf("PArtns : %s", regex_str);
                *regex_result = malloc(sizeof(Regex));
                (*regex_result)->type = parenthesis;
                (*regex_result)->optionA = malloc(sizeof(Regex*) * strlen(regex_str));
                (*regex_result)->optionB = malloc(sizeof(Regex*) * strlen(regex_str));
                parse_regex((*regex_result)->optionA, regex_str+1, false);
                char *after_pipe = strchr(regex_str, '|') + 1;
                //printf("AFTER PIPE : %s", after_pipe);
                parse_regex((*regex_result)->optionB, after_pipe, false);
                char *after_parents = strchr(regex_str, ')') + 1;
                //printf("AFTER PIPE : %s", after_pipe);
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


bool match_regex(Regex** regex, char* text, bool case_insensitive)
{
   do {
      if (match_regex_at_place(regex, text, case_insensitive)) {
         return true;
      }
   } while (*text++ != '\0');
   return false;
}

/*
 *int main()
 *{
 *    char text[] = "bla";
 *    char re_str[] = "b.a";
 *    Regex** re = malloc(sizeof(Regex*) * strlen(text));
 *
 *    parse_regex(re, re_str, false);
 *    printf("FINISHED PARSING");
 *    if (match_regex(re, text, false))
 *       printf("FOUND MATCH");
 *    else
 *      printf("NO MATCH");
 *   return 0;
 *
 *}
 *
 */
