#include <string.h>

#define MAX_X 4			    // size of the grid
#define MAX_Y 4
#define MIN_WORD 3		    // smallest word
#define MAX_WORD 8		    // largest
#define MAX_WORDN MAX_WORD + 1	// largest + null terminator
#define MAX_WL 1024 * 8		// wordlist size (each word length has its own)
#define MAX_MATCHES 210		// 203 was the most i've seen


// copy four strings to initialise a new grid
int makeGrid(char grid[][MAX_X], char *r1, char *r2, char *r3, char *r4)
{
    if (strlen(r1) != 4) { return 0; }
    if (strlen(r2) != 4) { return 0; }
    if (strlen(r3) != 4) { return 0; }
    if (strlen(r4) != 4) { return 0; }

    strncpy(grid[0], r1, 4);
    strncpy(grid[1], r2, 4);
    strncpy(grid[2], r3, 4);
    strncpy(grid[3], r4, 4);

    return 1;
}


int makeGridString(char grid[][MAX_X], char *gridAsString)
{
    for (int k = 0; k < 4; k++) {
        strncpy(gridAsString, grid[k], 4);
        gridAsString += 4;
    }
    strncpy(++gridAsString, "\0", 1);
    return 1;
}


// checks if word is already in the matches array
// to avoid populating it with dupes
int findMatch(char word[], int mu, char matches[][MAX_WORDN])
{
	for (int c = 0; c < mu; c++) {
		if (word[0] != matches[c][0]) { continue; }
		if (word[1] != matches[c][1]) { continue; }
		if (strncmp(word, matches[c], strlen(word)) == 0) {
			return 1;
		}
	}
	return 0;
}


// from a given start coordinate, build gridString (string made up from a path
// taken on the grid), and compare to the wordlists for matches.
// multiple matches are possible in a gridString (at various lengths)
void recurseScramble(int depth, int x, int y, char gridString[], char grid[][MAX_X], int valid[][MAX_X], char words[][MAX_WL][MAX_WORDN], int *mi, char matches[][MAX_WORDN])
{
	// have we been here before?
	if (valid[y][x] != 1) { return; }
	// is the coordinate legal?
	if ((x < 0) || (x >= MAX_X)) { return; }
	if ((y < 0) || (y >= MAX_Y)) { return; }
	// too deep?
	if (depth >= MAX_WORD) { return; }
	// found enough matches?
	if (*mi > MAX_MATCHES) { return; }

	// mark this position no longer valid
	valid[y][x] = 0;

	// append this character to our query word
	gridString[depth] = grid[y][x];

	depth++;

	// compare words of this length (indicated by "depth")
	if (depth >= MIN_WORD) {
		// do we match any words at this length?
		for (int i = 0; i < MAX_WL; i++) {
			// is this the end of the list?
			if (words[depth][i][0] == '\0') { break; }

			// executing strncmp() on each word adds about one
			// second to the overall time,
			// so check the first few chars to decide if we
			// want to check the whole string
			if (gridString[0] != words[depth][i][0]) { continue; }
			if (gridString[1] != words[depth][i][1]) { continue; }
			if (gridString[2] != words[depth][i][2]) { continue; }

			if (strncmp(gridString, words[depth][i], depth) == 0) {

				// do we already know this match?
				if (findMatch(gridString, *mi, matches)) {
					continue;
				}

				// we don't know it!
				memset(matches[*mi], '\0', MAX_WORDN);
				strncpy(matches[*mi], gridString, depth);
				*mi = *mi + 1;

				// this will get iffy with MAX_WORD > 8
				printf("%9s", gridString);
				if ((*mi % 8) == 0) {
					printf("\n");
					fflush(stdout);
				}

				// since we've found a match at this length
				// we can stop searching
				break;
			}

			if (*mi > MAX_MATCHES) { break; }
		}


		// decide if we want to recurse further with current string
		if (depth != MAX_WORD) {
			/*
			compare the first few chars with words from wordlists
			with longer words than the current depth
			if we match enough, we assume there may be a real
			match if we travel deeper
			we can thus ignore paths leading to nothing
			this is a noticable speed boost and finally got the
			time under one second on a length of 8
			*/

			// found a hint of a real match flag
			int foundHint = 0;
			int d;		// depth loop index
			// check all lists *longer* than current depth
			for (d = depth+1; d < MAX_WORD; d++) {

				for (int i = 0; i < MAX_WL; i++) {

                    // are we at the end of the word list?
                    if (words[d][i][0] == '\0') { break; }

                    // just compare a few initial chars
                    // as we only want a hint of a true match
                    if (gridString[0] != words[d][i][0]) { continue; }
                    if (gridString[1] != words[d][i][1]) { continue; }
                    if (gridString[2] != words[d][i][2]) { continue; }

                    foundHint = 1;

				}

				// abandon the search, we have a hope!
				if (foundHint) { break; }

			}

			// found a hint? if not we can safely abandon this path
			if (!foundHint) { return; }

		} else {
			// we have reached the end of a grid string
			if (depth == MAX_WORD) { return; }
		}

	}

	// travel
	// from the current position we can travel in 8 directions
	// we need copies of the gridString and the valid lookup table
	//  for each path we travel, other than that the same vars are passed
	int xx, yy;
	for (xx = x-1; xx < (x+2); xx++) {
		for (yy = y-1; yy < (y+2); yy++) {
			// ignore our current position
			if ((xx == x) && (yy == y)) { continue; }
			if ((xx < 0) || (xx > MAX_X)) { continue; }
			if ((yy < 0) || (yy > MAX_Y)) { continue; }

			// create clones of the gridString and valid lookup
			char gridStringX[MAX_WORDN];
			memset(gridStringX, '\0', MAX_WORDN);
			strncpy(gridStringX, gridString, MAX_WORD);
			int validX[4][4];
			memcpy(validX, valid, sizeof(validX));

			recurseScramble(
				depth, xx, yy,
				gridStringX,
				grid, validX,
				words,
				mi, matches);
		}
	}
}


// load one wordlist containing words of a given length
// filters out words on "gridAsString" to ignore including words that contain
// characters that aren't found in the grid
int loadWordList(int length, char words[][MAX_WL][MAX_WORDN], char *gridAsString)
{
	char filename[9];
	sprintf(filename, "words.%d", length);

	FILE *WL = fopen(filename, "r");
	if (WL == 0) { return 0; }

	char line[15];
	memset(line, '\0', sizeof(line));
	int lidx = 0;	// word list index
	int linelen;
	int i;
	int isValidWord;

	while (fscanf(WL, "%s", line) != EOF) {
		linelen = strlen(line);
		// enforce length
		if (linelen > MAX_WORD) { continue; }
		if (linelen < MIN_WORD) { continue; }

		// ignore words who's chars aren't anywhere in the grid
		// this gives us one of the largest noticable speed boosts
		isValidWord = 1;
		for (i = 0; i < linelen; i++) {
			if (strchr(gridAsString, line[i]) == NULL) {
				isValidWord = 0;
			}
		}
		if (!isValidWord) { continue; }

		memset(words[length][lidx], '\0', MAX_WORDN);
		strncpy(words[length][lidx], line, MAX_WORDN);

		lidx++;
		if (lidx > MAX_WL) { break; }

		// prepare for next line
		memset(line, '\0', linelen);
	}
	fclose(WL);

	// mark the end of the list
	if (lidx < MAX_WL) {
		words[length][lidx][0] = '\0';
	}

	//printf("Loaded %d/%d from %s\n", lidx, MAX_WL, filename);

    return lidx;
}


// loads multiple word lists (how many is based on MAX_WORD)
void loadWordLists(char words[][MAX_WL][MAX_WORDN], char *gridAsString)
{
	for (int i = MIN_WORD; i <= MAX_WORD; i++) {
		loadWordList(i, words, gridAsString);
	}
}
