#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 0

void removeLeading(char *str, char *str1)
{
    int idx = 0, j, k = 0;
 
    // Iterate String until last
    // leading space character
    while (str[idx] == ' ' || str[idx] == '\t' || str[idx] == '\n')
    {
        idx++;
    }
 
    // Run a for loop from index until the original
    // string ends and copy the content of str to str1
    for (j = idx; str[j] != '\0'; j++)
    {
        str1[k] = str[j];
        k++;
    }
 
    // Insert a string terminating character
    // at the end of new string
    str1[k] = '\0';
 
    // Print the string with no whitespaces
    //printf("%s", str1);
}

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
int main(int argc, char  *argv[])
{
    FILE* ptr;
    char str[200];
    char rank_view_search[] = "rank_view";
    char span_class_str[]   = "sr-only";
    char to_date_str[]      = "to_date";

    char rank_search_strong_sell[] = "5-Strong Sell";
    char rank_search_sell[]        = "4-Sell";
    char rank_search_hold[]        = "3-Hold";
    char rank_search_buy[]         = "2-Buy";
    char rank_search_strong_buy[]  = "1-Strong Buy";
    char rank_rect_na_str[]        = "rankrect_NA";

    char title_search[]      = "<title>";
    char symbol_search[]     = "var _ticker_string";
    char last_price_search[] = "p class=\"last_price\"";

    char *charptr;
    char *spanclassptr;
    char *rankptr;
    char *titleptr;
    char *symbolptr;
    char *lastpriceptr;
    char *todateptr;
    char c[] = " - Stock Price Today - Zacks";
    char d[] = "";
    char *result= NULL;
    char newresult[100];
    int rank_flag =0;

    char result_todate_str[30];
    float result_price;
    char result_rank_str[30];
    char result_title_str[50];
    char result_symbol_str[30]; 

    if ( argc == 1 ) {
	printf ( "Please enter the name of the stock symbol data file to process\n");
        exit (0);
    }


    ptr = fopen(argv[1], "r");
 
    if (NULL == ptr) {
        printf("File can't be opened \n");
	exit(0);
    }

    //if ( DEBUG ) { printf ( "1"); }
 
    //printf("content of this file are \n");
    while (fgets(str, 200, ptr) != NULL) {
	str[strlen(str) - 1] = '\0';

        if ( DEBUG ) { printf ( "%s",str); }

	//////////  Search for the Symbol  //////////////////
        symbolptr =  strstr ( str, symbol_search );
        if (symbolptr != NULL) /* Substring symbol found */
	{
		result = replaceWord(str, "var _ticker_string   =", "");
		result = replaceWord(result, ";", "");
		result = replaceWord(result, "'", "");
		result = replaceWord(result, " ", "");
		//printf("\"%s\",", result);
		sprintf(result_symbol_str,"\"%s\"", result);

	}
	//////////  Search for the title  //////////////////
        todateptr =  strstr ( str, to_date_str );
        if (todateptr != NULL) /* Substring title found */
	{
		if ( DEBUG ) {printf ( "date is [%s]\n", str ); }
		result = replaceWord(str, "        <input type=\"hidden\" class=\"to_date\" value=", "");
		result = replaceWord(result, " /> ", "");

		//printf ( "date after -> [%s]\n", result );
		removeLeading(result, newresult);
		
		sprintf (result_todate_str, "%s", newresult );
                
	}


	//////////  Search for the title  //////////////////
        titleptr =  strstr ( str, title_search );
        if (titleptr != NULL) /* Substring title found */
	{
		//printf ("Am I crashing here \n");
		result = replaceWord(str, c, d);
		result = replaceWord(result, "    <title>", "");
		result = replaceWord(result, "</title>", "");

		//printf("\"%s\",", result);
		sprintf(result_title_str, "\"%s\"", result);
	}


	//////////  Search for the last price  //////////////////
        lastpriceptr =  strstr ( str, last_price_search );
        if (lastpriceptr != NULL) /* Substring title found */
	{
	       
		result = replaceWord(str, "<p class=\"last_price\">", "");
		result = replaceWord(result, "<span>", "");
	        
		result = replaceWord(result, "</span>", "");
		result = replaceWord(result, "</p>", "");
		result = replaceWord(result, " USD", "");
		result = replaceWord(result, "$", "");
		removeLeading(result, newresult);
		//printf("%.2f,", atof(newresult));
		result_price = atof(newresult);

	}



	//////////// Search for the Ranking ///////////////
        charptr =  strstr ( str, rank_view_search );
	//printf("Found = %s", str);

        if (charptr != NULL) /* Substring rank_view found */
	{
		//printf("Found = %s", str);

                rank_flag=0;
                if (fgets(str, 200, ptr) != NULL) {
		     //printf(">>>>>>>>>>>> next line = %s\n", str);
                     spanclassptr =  strstr( str, span_class_str );
		     //printf(">>>>>>>>>>>> spanclassptr = %s\n", spanclassptr);
                     if (spanclassptr != NULL) /* Substring found */
	             {

			 // Sell
                         rankptr =  strstr( str, rank_search_sell );
                         if (rankptr != NULL) /* Substring found */
			 {
		             //printf("\"SELL\"" );
		             sprintf(result_rank_str, "%s", "\"SELL\"" );
				
			     rank_flag=1;
			 }

			 // strong Sell
                         rankptr =  strstr( str, rank_search_strong_sell );
                         if (rankptr != NULL) /* Substring found */
			 {
		             //printf("\"Strong SELL\"" );
		             sprintf(result_rank_str, "%s", "\"Strong SELL\""  );
		             rank_flag=1;
			 }

			 // Hold
                         rankptr =  strstr( str, rank_search_hold );
                         if (rankptr != NULL) /* Substring found */
			 {
		             //printf("\"HOLD\"" );
		             sprintf(result_rank_str, "%s", "\"HOLD\"" );
			     rank_flag=1;
			 }

			 // Buy
                         rankptr =  strstr( str, rank_search_buy );
                         if (rankptr != NULL) /* Substring found */
			 {
		             //printf("\"BUY\"" );
		             sprintf(result_rank_str, "%s", "\"BUY\"" );
			     rank_flag=1;
			 }

			 // Strong Buy
                         rankptr =  strstr( str, rank_search_strong_buy );
                         if (rankptr != NULL) /* Substring found */
			 {
		             //printf("\"Strong BUY\"" );
		             sprintf(result_rank_str, "%s", "\"Strong BUY\"" );
			     rank_flag=1;
			 }
		         //printf("\n" );
                     } else  {
			
			 rankptr =  strstr( str, rank_rect_na_str );
                         if (rankptr != NULL) /* Substring found rankrect_NA */
                         {
                              //printf ( "\"Rank NA\"");
                              sprintf (result_rank_str, "%s", "\"Rank NA\"");
                         }
		     } 
                }
	}
    }

    printf  ("%s,%s,%s,%.2f,%s\n", result_todate_str, result_symbol_str, result_title_str, result_price, result_rank_str  );
 
    fclose(ptr);
    return 0;
}
