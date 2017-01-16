#include <stdio.h>
#include "descramble.h"


void usage(char *name)
{
	printf("Usage: %s abcd efgh ijkl mnop\n", name);
}


int main(int argc, char *argv[])
{
	if (argc != 5) {
        usage(argv[0]);
        return 1;
    }

	char grid[MAX_Y][MAX_X];
	char gridAsString[17];

    if (!makeGrid(grid, argv[1], argv[2], argv[3], argv[4])) {
        usage(argv[0]);
    }

    makeGridString(grid, gridAsString);

	// wordlists are seperated by word length
	char words[MAX_WORD + 1][MAX_WL][MAX_WORDN];
	loadWordLists(words, gridAsString);

	char matches[MAX_MATCHES][MAX_WORDN];
	int mi = 0;	// matches array index

	// start the crawl
	int x,y;
	for (y = 0; y < MAX_Y; y++) {
		for (x = 0; x < MAX_Y; x++) {
			// this is our valid lookup table
			// a 0 will indicate somewhere we have been and thus
			// we can not visit again
			int valid[4][4] = {
				{1, 1, 1, 1},
				{1, 1, 1, 1},
				{1, 1, 1, 1},
				{1, 1, 1, 1}
			};
			char gridString[MAX_WORDN];
			memset(gridString, '\0', MAX_WORDN);
			recurseScramble(0, x, y, gridString, grid, valid, words, &mi, matches);
		}
	}

	printf("\n");
	printf("Found %d words\n", mi);

	return 0;
}
