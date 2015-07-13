#include <clocale>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#include "../common/common_defs.h"
void show_locale() {
    char* tmp = setlocale(LC_ALL, "");
    cout << strlen(common::CURR_P) << endl;
    printf("%s\n", tmp);
}
