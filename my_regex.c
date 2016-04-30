#include <stdbool.h>
#include <ctype.h>

bool chrcasecmp(char const a, char const b)
{
    int diff = tolower(a) - tolower(b);
    return diff == 0;
}

bool match_regex_at_place(char* regex, char* text, bool case_insensitive)
{
    if (regex[0] == '\0')
            return true;
    if (text[0] != '\0'){
       if(regex[0] == '.' || text[0] == regex[0] ||
               (case_insensitive && chrcasecmp(text[0], regex[0])))
        return match_regex_at_place(++regex, ++text, case_insensitive);
    }
    return false;
}

bool match_regex(char* regex, char* text, bool case_insensitive)
{
   do {
      if (match_regex_at_place(regex, text, case_insensitive)) {
         return true;
      }
   } while (*text++ != '\0');
   return false;
}
