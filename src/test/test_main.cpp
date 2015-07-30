#include <cstdio>

#include "libev.h"
// #include "t_ncurses.h"
#include "local.h"
#include "libcurl.h"
#include "t_leveldb.h"
#include "t_common.h"
#include "t_filehandler.h"

using namespace std;

int main (void)
{
    // test_ncurses();
    // show_locale();
    // test_libev();
    // test_curl();
    // t_leveldb();
    // test_common();
    test_filehandler();

    return 0;
}
