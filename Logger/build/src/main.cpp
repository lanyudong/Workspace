#include <iostream>
#include "include/logger.h"

#define DEVICE = test

int main () {
    Logger("INFO") << "Data\t" << "Data\t" << "Data\t";
    Logger("WARN") << "Data\t" << "Data\t" << "Data\t";
    Logger("ERROR") << "Data\t" << "Data\t" << "Data\t";
    return 0;
}
