
#include <arrayList.h>
#include <hashMap.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 150
#define WORD [a - z] +

typedef struct {
    char* color;
    int count;
} bag;

hashMap* bagMap;

void bagKvpPrinter(keyValPair* kvpp) {
    printf("[%s : ", (char*)kvpp->key);
    for (int i = 0; i < getSize(kvpp->val); i++) {
        bag* bagp = getItemAt(kvpp->val, i);
        printf("(%i %s), ", bagp->count, bagp->color);
    }
    printf("]\n");
}

char* getBagName(char** line) {
    char* bagNameReg = "^([a-z]+ [a-z]+) bags contain? ";
    size_t matchCount = 3;

    regex_t reg;
    regmatch_t matches[matchCount];

    if (regcomp(&reg, bagNameReg, REG_EXTENDED) || regexec(&reg, *line, matchCount, matches, 0)) {
        puts("Failed to execute bag name regex");
        exit(1);
    }

    regmatch_t fullMatch = matches[0];
    int fullLen = fullMatch.rm_eo - fullMatch.rm_so;

    regmatch_t nameMatch = matches[1];
    int len = nameMatch.rm_eo - nameMatch.rm_so;
    char* matchStr = malloc(sizeof(char) * len + 1);
    matchStr[len] = '\0';
    memcpy(matchStr, *line + nameMatch.rm_so, len);
    (*line) += fullLen;
    return matchStr;
}

//double pointer so address can be modified and incremented for next bag
bag* getNextBag(char** line) {
    char* enclosedBagReg = "([1-9]+) ([a-z]+ [a-z]+) bags?[ .,]?[ ]?";
    int matchCount = 4;
    regex_t reg;
    regmatch_t matches[matchCount];

    if (regcomp(&reg, enclosedBagReg, REG_EXTENDED) || regexec(&reg, *line, matchCount, matches, 0)) {
        puts("Failed to execute enclosing bag regex");
        return NULL;
    }

    bag* currentBag = malloc(sizeof(bag));

    for (int i = 1;; i++) {
        regmatch_t match = matches[i];
        int len = match.rm_eo - match.rm_so;
        char* matchStr = malloc(sizeof(char) * len + 1);
        matchStr[len] = '\0';
        memcpy(matchStr, *line + match.rm_so, len);

        if (match.rm_so == -1) {
            break;
        }

        if (i == 1) {
            currentBag->count = atoi(matchStr);
        } else if (i == 2) {
            currentBag->color = matchStr;
            (*line) += len;
        } else {
            puts("Too many matches in bag match");
            puts(matchStr);
            exit(1);
        }
    }

    return currentBag;
}

arrayList* getEnclosedBags(char* line) {
    arrayList* bagL = createArrayList();
    bag* currentBag;
    while ((currentBag = getNextBag(&line))) {
        appendToAl(bagL, currentBag);
    }
    return bagL;
}

void parseLine(char* line) {

    char* bagKey = getBagName(&line);
    arrayList* enclosedBags = getEnclosedBags(line);
    char* regex = "^([a-z]+ [a-z]+) [a-z]+ [a-z]+ ([1-9]+ )?([a-z]+ [a-z]+)? bags?[,. ]?";

    addToMap(bagMap, bagKey, enclosedBags, true);
}

int main() {

    FILE* in = fopen("day7sample.txt", "r");

    bagMap = createHashMap(strHash, strComp);

    for (char buff[BUFF_SIZE], *p = fgets(buff, BUFF_SIZE, in);; p = fgets(buff, BUFF_SIZE, in)) {
        puts(buff);
        parseLine(buff);
        if (!p) {
            break;
        }
    }
    printMapPairs(bagMap, bagKvpPrinter);
}