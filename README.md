# Compare_Zacks_Rankings_In_C

This code compares the results for every stock and determines if there is a change in ranking and reports it.

The input files are snp.txt - containing all the ticker symbols that needs to be processed.

and the results.txt - that contains all the contents of the database for all stocks that has the date, symbol, name, price and ranking(s).

```

$ cat snp.txt
MMM
AOS
ABT
ABBV

```


The results.txt file has the listings from the database.

```
$ cat results.txt
04/06/23|A|Agilent Technologies - A|138.34|BUY
04/06/23|AAL|American Airlines - AAL|14.75|BUY
04/06/23|AAP|Advance Auto Parts - AAP|121.61|SELL
04/06/23|AAPL|Apple - AAPL|164.9|HOLD
04/06/23|ABBV|AbbVie - ABBV|159.37|HOLD
04/06/23|ABC|AmerisourceBergen - ABC|160.11|HOLD
04/06/23|ABT|Abbott Laboratories - ABT|101.26|HOLD
04/06/23|ACGL|Arch Capital Group - ACGL|67.87|BUY
04/06/23|ACN|Accenture - ACN|285.81|HOLD
04/06/23|ADBE|Adobe - ADBE|385.37|BUY

The results looks like this as shown below, Note the MMM and the ABBV reports the changes.

```


$  ./parseresults // Look at the code checked in - that is more accurate. This is just a sample.
TARGET >>>>> [1],  [MMM]
[MMM], No change 
[MMM] Old rank and New rank are different 
[MMM] Old date ["05/03/23"], Old rank ["BUY"]
[MMM] New date ["06/03/23"], New rank ["Strong BUY"] 
TARGET >>>>> [2],  [AOS]
TARGET >>>>> [3],  [ABT]
TARGET >>>>> [4],  [ABBV]
[ABBV], No change 
[ABBV] Old rank and New rank are different 
[ABBV] Old date ["05/03/23"], Old rank ["HOLD"]
[ABBV] New date ["05/03/23"], New rank ["Sell"] 

```

The source code is 
```

$ cat parseresults.c
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


char* replaceWord(const char* s, const char* oldW,
                const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
 
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
 
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
 
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
 
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
 
    result[i] = '\0';
    return result;
}


int main (int argc, char  * argv[] ) {

    FILE *fresults;
    FILE *fsnp;

    char str[200];
    char resultsstr[200];
    char targetstr[200];
    char olddate  [20];
    char datestr  [20];
    char oldrank  [20];
    char *token;
    const char s[2] = ",";
    const char *result = NULL;
    int target_linecount=1;
    int results_linecount=1;
    int symbol_found_flag=0;
    int debug_flag =0;


    if ( argc >1 ) {

	printf ("[%s]\n", argv[1]);
	debug_flag =1;
    }else {
	debug_flag =0;
}


    fsnp = fopen("snp.txt", "r");
    if(fsnp == NULL) {
      printf ("Error Opening the fsnp file\n");
      return(-1);
    }
    while( fgets (targetstr, 200, fsnp)!=NULL ) {
	    targetstr[strlen (targetstr)-1 ] = '\0';
	    printf ("TARGET >>>>> [%d],  [%s]\n", target_linecount, targetstr);
	    target_linecount++;
	
	    // Open the results file and start reading one line at a time.

	    fresults = fopen("results.txt", "r");
	    if(fresults == NULL) {
	      printf ("Error\n");
	      return(-1);
	    }
		
	    symbol_found_flag=0;
	    while( fgets (resultsstr, 200, fresults)!=NULL ) {
		resultsstr[ strlen ( resultsstr) -1 ] = '\0';
		//printf("%s\n",resultsstr);
		/* get the date  first token */
		token = strtok(resultsstr, s);
		strcpy ( datestr, token );
		//printf( " Date>>>%s\n", token );

		// get the symbol
		token = strtok(NULL, s);
		result = replaceWord(token, "\"", "");
		if ( debug_flag) {printf( "Symbol being processed in results file >>>%s\n", result ); }

		// result contains SYMBOL
		if ( strcmp ( result, targetstr ) == 0) {
			symbol_found_flag++;
			if ( debug_flag ) {printf ( ">>>>>>>>>>>>> Found [%d] [%s], count[%d] in results\n", results_linecount, targetstr, symbol_found_flag ); }
  			results_linecount++;
			// get the name
			token = strtok(NULL, s);
			if ( debug_flag) {printf( " Symbolname >>>%s\n", token ); }

			// get the price
			token = strtok(NULL, s);
			if ( debug_flag) {printf( " price >>>%s\n", token );}

			// get the rank
			token = strtok(NULL, s);
			if ( debug_flag) {printf( " rank >>>%s\n", token );}
			if( symbol_found_flag == 1) {
				strcpy ( olddate, datestr );
				strcpy ( oldrank, token );
			} else {
				if ( strcmp ( oldrank, token ) != 0 ){ // they are different
					printf ("[%s] Old rank and New rank are different \n", targetstr);
					printf ("[%s] Old date [%s], Old rank [%s]\n[%s] New date [%s], New rank [%s] \n", targetstr,olddate, oldrank, targetstr, datestr, token );

				} else {
					printf ("[%s], No change \n", targetstr);
					strcpy ( olddate, datestr);
					strcpy ( oldrank, token );
				}

			}
		} 

	     }
	     fclose (fresults ); 
    }

    fclose ( fresults);
    fclose ( fsnp);
    return 1;
}

```
