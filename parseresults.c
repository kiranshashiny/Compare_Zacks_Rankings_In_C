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
    char symbol_name  [50];
    char *token;
    const char s[2] = "|";
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
		
	    symbol_found_flag=0;
	    while( fgets (resultsstr, 200, fresults)!=NULL ) {
		if ( debug_flag ) printf("%s\n",resultsstr);
		resultsstr[ strlen ( resultsstr) -1 ] = '\0';
		/* get the date  first token */
		token = strtok(resultsstr, s);
		if ( debug_flag ) {printf( " Date from token>>>%s\n", token ); }
		strcpy ( datestr, token );

		// get the symbol
		token = strtok(NULL, s);
		result = replaceWord(token, "\"", "");
		if ( debug_flag) {printf( "Symbol being processed in results file >>>%s\n", result ); }

		// result contains SYMBOL
		if ( strcmp ( result, targetstr ) == 0) {
			symbol_found_flag++;
			if ( debug_flag ) {printf ( ">>>>>>>>>>>>> Found [%d] [%s], count[%d] in results\n", results_linecount, targetstr, symbol_found_flag ); }
  			results_linecount++;


			// get the symbol name
			token = strtok(NULL, s);
			strcpy ( symbol_name, token );
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
					printf ("[%s] [%s] Old rank and New rank are different \n", targetstr,symbol_name);
					printf ("[%s] Old date [%s], Old rank [%s]\n[%s] New date [%s], New rank [%s] \n\n", targetstr,olddate, oldrank, targetstr, datestr, token );

				} else {
					//printf ("[%s], No change \n", targetstr);
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
