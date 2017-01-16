#include <stdio.h>
#include "minunit.h"
#include "../descramble.h"


int tests_run = 0;


static char * test_makeGrid()
{
	char g[4][4];

    mu_assert("error, makeGrid() #1 valid grid", makeGrid(g, "aaaa", "bbbb", "cccc", "dddd"));
    mu_assert_not("error, makeGrid() #2 invalid grid", makeGrid(g, "aaaa", "", "", ""));
    mu_assert_not("error, makeGrid() #3 invalid grid", makeGrid(g, "aaaa", "bbbb", "", ""));
    mu_assert_not("error, makeGrid() #4 invalid grid", makeGrid(g, "aaaa", "bbbb", "cccc", ""));
    mu_assert_not("error, makeGrid() #5 invalid grid", makeGrid(g, "a", "bbbb", "cccc", "dddd"));

    return 0;
}


static char * test_makeGridString()
{
	char g[4][4];
	char gs[17];

    makeGrid(g, "aaaa", "bbbb", "cccc", "dddd");

    mu_assert("error, makeGridString() #1 valid grid", makeGridString(g, gs));

    return 0;
}


static char * test_loadWordList()
{
	char words[MAX_WORD + 1][MAX_WL][MAX_WORDN];
    int t;

    t = loadWordList(4, words, "xyz");
    mu_assert("error, loadWordList() #1 should not load any words", t == 0);

    t = loadWordList(4, words, "thisfileae");
    mu_assert("error, loadWordList() #2 failed to read 5 words from file", t == 5);

    t = loadWordList(4, words, "thisfilee");
    mu_assert("error, loadWordList() #3 failed to read 4 words (filtered) from file", t == 4);

    return 0;
}


static char * all_tests()
{
    mu_run_test(test_makeGrid);
    mu_run_test(test_makeGridString);
    mu_run_test(test_loadWordList);
    return 0;
}


int main(int argc, char **argv)
{
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
