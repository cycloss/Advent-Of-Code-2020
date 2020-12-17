
#include <arrayList.h>
#include <hashMap.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 200
#define WORD [a - z] +

typedef struct {
    char* color;
    int count;
} bag;

hashMap* bagMap;
arrayList* bagList;

void bagKvpPrinter(keyValPair* kvpp) {
    printf("[%s : ", (char*)kvpp->key);
    for (int i = 0; i < getSize(kvpp->val); i++) {
        bag* bagp = getItemAt(kvpp->val, i);
        printf("(%i %s), ", bagp->count, bagp->color);
    }
    printf("]\n");
}

void strPrinter(void* str) {
    puts((char*)str);
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
            regmatch_t fullMatch = matches[0];
            int fullLen = fullMatch.rm_eo - fullMatch.rm_so;
            (*line) += fullLen;
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

    appendToAl(bagList, bagKey);
    addToMap(bagMap, bagKey, enclosedBags, true);
}

bool canContainGoldBag(char* bagName) {
    arrayList* currentBagList = (arrayList*)getValueForKey(bagMap, bagName);

    if (!currentBagList) {
        //no match found in map
        printf("No match found for %s\n", bagName);
        return false;
    }

    //keeps going recursing until finds bag with no more bags
    for (int i = 0; getSize(currentBagList); i++) {
        bag* bag = getItemAt(currentBagList, i);
        if (strcmp("shiny gold", bag->color) == 0) {
            printf("Found shiny gold in: %s\n", bagName);
            return true;
        } else {
            return canContainGoldBag(bag->color);
        }
    }
    return false;
}

int getShinyGoldCarrierCount() {
    int count = 0;

    for (int i = 0; i < getSize(bagList); i++) {
        char* bagName = getItemAt(bagList, i);
        if (canContainGoldBag(bagName)) {
            count++;
        }
    }
    return count;
}

int main() {

    FILE* in = fopen("day7sample2.txt", "r");

    bagMap = createHashMap(strHash, strComp);
    bagList = createArrayList();

    for (char buff[BUFF_SIZE], *p = fgets(buff, BUFF_SIZE, in); p; p = fgets(buff, BUFF_SIZE, in)) {
        parseLine(buff);
    }
    printMapPairs(bagMap, bagKvpPrinter);
    puts("Rules:");
    iterateListItems(bagList, strPrinter);

    int shinyGoldCarrierCount = getShinyGoldCarrierCount();
    printf("Can contain gold bag: %i", shinyGoldCarrierCount);
    //TODO severe memory leak as map not being properly free as is only freeing array list and not also the values in the array list
    freeMap(bagMap, true);
    freeAl(bagList, false);
    fclose(in);
}