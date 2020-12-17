#include "day7.h"

#define BUFF_SIZE 200

hashMap* bagMap;
arrayList* bagList;

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
    bool found = false;
    for (int i = 0; i < getSize(currentBagList); i++) {
        bag* bag = getItemAt(currentBagList, i);
        if (strcmp("shiny gold", bag->color) == 0) {
            printf("Found shiny gold in: %s\n", bagName);
            return true;
        } else {
            if (canContainGoldBag(bag->color)) {
                found = true;
                break;
            };
        }
    }
    return found;
}

int getShinyGoldCarrierCount() {
    int count = 0;

    for (int i = 0; i < getSize(bagList); i++) {
        char* bagName = getItemAt(bagList, i);
        printf("Searching bag: %s\n", bagName);
        if (canContainGoldBag(bagName)) {
            count++;
        }
    }
    return count;
}

int main() {

    FILE* in = fopen("day7.txt", "r");

    bagMap = createHashMap(strHash, strComp);
    bagList = createArrayList();

    for (char buff[BUFF_SIZE], *p = fgets(buff, BUFF_SIZE, in); p; p = fgets(buff, BUFF_SIZE, in)) {
        parseLine(buff);
    }
    iterateMapPairs(bagMap, bagKvpPrinter);
    puts("Rules:");
    iterateListItems(bagList, strPrinter);

    int shinyGoldCarrierCount = getShinyGoldCarrierCount();
    printf("Part 1: Can contain at least one gold bag: %i", shinyGoldCarrierCount);
    freeBagMap();
    freeAl(bagList, false);
    fclose(in);
}