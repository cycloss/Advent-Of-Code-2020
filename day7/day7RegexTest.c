#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void match(char* line, char* regex) {

    size_t matchCount = 10;

    regex_t reg;
    regmatch_t matches[matchCount];

    if (regcomp(&reg, regex, REG_EXTENDED) || regexec(&reg, line, matchCount, matches, 0)) {
        puts("Failed to execute regex");
        return;
    }

    for (int i = 1;; i++) {
        regmatch_t match = matches[i];
        int adjusted = i - 1;
        if (match.rm_so == -1) {
            break;
        } else if (adjusted % 3 == 0) {
            continue;
        }

        int len = match.rm_eo - match.rm_so;
        char* matchStr = malloc(sizeof(char) * len + 1);
        matchStr[len] = '\0';
        memcpy(matchStr, line + match.rm_so, len);

        puts(matchStr);
    }
}

int main() {
    char* line = "vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.";
    char* regex = "^([a-z]+ [a-z]+)";
    match(line, regex);
    char* regex2 = "(([1-9]+) ([a-z]+ [a-z]+) bags?[ .,]?[ ]?)(([1-9]+) ([a-z]+ [a-z]+) bags?[ .,]?[ ]?)?";
    match(line, regex2);
}