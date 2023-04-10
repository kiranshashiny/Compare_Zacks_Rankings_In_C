#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define STRSIZE      50
#define STRSTOCKNAME 50

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
    char symbol_name  [50];
    char *token;
    const char s[2] = "|";
    const char *result = NULL;
    int target_linecount=1;
    int results_linecount=1;
    int symbol_found_flag=0;
    int debug_flag =0;
    int rank_changed =0;

    struct MyStructure {   // Structure declaration
	   char Date[STRSIZE];  
	   char StockName[STRSIZE];
	   char StockSymbol [STRSTOCKNAME];
	   float StockPrice; 
	   char StockRank[STRSIZE]; 
    };

    struct MyStructure arrayOfStocks[5];

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

    //if ( debug_flag ) { printf ( "snp.txt opened successfully\n"); }
    while( fgets (targetstr, 200, fsnp)!=NULL ) {
	    targetstr[strlen (targetstr)-1 ] = '\0';
	    if ( targetstr[0] == '#' ){
		continue;
	    }
	    if ( debug_flag) {printf ("TARGET snp.txt >>>>> [%d],  [%s]\n", target_linecount, targetstr); }
	    target_linecount++;
	
	    // Open the results file and start reading one line at a time.

	    fresults = fopen("results.txt", "r");
	    if(fresults == NULL) {
	      printf ("Error file results.txt not found ! Exiting - Fix the file name first.\n");
	      return(-1);
	    }
            if ( debug_flag ) { printf ( "results.txt opened successfully"); }
		
	    symbol_found_flag=1;
	    rank_changed =0;
	    while( fgets (resultsstr, 200, fresults)!=NULL ) {
		//if ( debug_flag ) printf("%s\n",resultsstr);
		resultsstr[ strlen ( resultsstr) -1 ] = '\0';

		/* get the date  first token */
		token = strtok(resultsstr, s);
		//if ( debug_flag ) {printf( " Date from token>>>%s\n", token ); }
		strcpy ( datestr, token );

		// get the symbol
		token = strtok(NULL, s);
		result = replaceWord(token, "\"", "");
		//if ( debug_flag) {printf( "Symbol being processed in results file >>>%s\n", result ); }

		// result contains SYMBOL
		if ( strcmp ( result, targetstr ) == 0) {
			if ( debug_flag ) {printf ( ">>>>>>>>>>>>> Found [%d] [%s], count[%d] in results.txt\n", results_linecount, targetstr, symbol_found_flag ); }
  			results_linecount++;

			strcpy( arrayOfStocks[symbol_found_flag].Date,        datestr );
			strcpy( arrayOfStocks[symbol_found_flag].StockSymbol, result );

			// get the symbol name
			token = strtok(NULL, s);
			strcpy ( symbol_name, token );

			if ( debug_flag) {printf( " date >>>%s\n", datestr ); }
			if ( debug_flag) {printf( " Symbolname >>>%s\n", token ); }

			// get the price
			token = strtok(NULL, s);
			if ( debug_flag) {printf( " price >>>%s\n", token );}
			arrayOfStocks[symbol_found_flag].StockPrice = atof ( token );

			// get the rank
			token = strtok(NULL, s);
			if ( debug_flag) {printf( " rank >>>%s\n", token );}
			strcpy( arrayOfStocks[symbol_found_flag].StockRank, token );

			// first time- so just dont do anything - just save data.
			if( symbol_found_flag == 1) {
				strcpy ( olddate, datestr );
				strcpy ( oldrank, token );
			} else {
				if ( strcmp ( oldrank, token ) != 0 ){ // they are different
				        rank_changed = 1;
					printf ("[%s] [%s] Old rank and New rank are different \n", targetstr,symbol_name);
					printf ("[%s] Old date [%s], Old rank [%s]\n[%s] New date [%s], New rank [%s] \n\n", targetstr,olddate, oldrank, targetstr, datestr, token );
					strcpy ( olddate, datestr);
					strcpy ( oldrank, token );

				} else {
					//printf ("[%s], No change \n", targetstr);
					strcpy ( olddate, datestr);
					strcpy ( oldrank, token );
				}
			}
			symbol_found_flag++;
		}

	     } // inner while results loop.

	     //printf ( "Number of times the symbol [%s] was found [%d]\n", targetstr, symbol_found_flag-1 );
	     if (rank_changed ) {
		     for ( int i=1; i< symbol_found_flag; i++ ) {
			printf ("%s %s %.2f %s\n", arrayOfStocks[i].Date, arrayOfStocks[i].StockSymbol , arrayOfStocks[i].StockPrice, arrayOfStocks[i].StockRank);
		     }
		     printf ("\n");
	     } // end of the rank_changed.

	     fclose (fresults ); 
	} // big while loop.

    fclose ( fresults);
    fclose ( fsnp);
    return 1;
}
