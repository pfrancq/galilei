			/* ==== :INFO/STRIP.C ==== */

/*
 * AUTHOR	: Stuart J. Barr 
 * DATE:	: c. September 1986, give or take a few months...
 */

/*
 * to save confusion...
 * USAGE	: define KEYWORDSIZE (6 is a good value, in practise)
 *		  define FALSE, TRUE
 *		  if you want to strip prefixes, define PREFIXES
 *		  Write a wee C function to call
 *			strip_affixes(string)
 *		  where string is a char *.
 */

#include <stdio.h>



/*#include "defs.h"*/
#define TRUE   1
#define FALSE  0
#define KEYWORDSIZE 25
#define PREFIXES 1







#define BIG_KEYWORDSIZE (KEYWORDSIZE+20)

void	to_lower_case(),
	clean(),
#ifdef PREFIXES
	strip_prefixes(),
#endif PREFIXES
	strip_suffixes();

void strip_affixes ( string )
	char *string;
{
	to_lower_case(string);
	clean(string);
#ifdef PREFIXES
	strip_prefixes(string);
#endif PREFIXES
	strip_suffixes(string);
	string[KEYWORDSIZE] = '\0';
}

static void to_lower_case ( kwd )
	char *kwd;
{
	int i;
	for ( i=0 ; i < strlen(kwd) ; i++ )
		if ( (kwd[i] >= 'A') && (kwd[i] <= 'Z') )
			kwd[i] += 'a' - 'A';
}

static void clean ( kwd )
	char *kwd;
{
	int i,j,last=strlen(kwd);
	for ( i=0 ; i < last ; i++ ) {
		if ( isvalid(kwd[i]) != 0 ) {
			for ( j = i ; j < last-1 ; j++ )
				kwd[j] = kwd[j+1];
			kwd[last-1] = '\0';
			last--;
			i--;
		}
	}
}

static isvalid(l)
	char l;
{
	if ( (l >= 'a') && (l <= 'z') )
		return(0);
	if ( (l >= 'A') && (l <= 'Z') )
		return(0);
	if ( (l >= '0') && (l <= '9') )
		return(0);
	return(1);
}

#ifdef PREFIXES
static void strip_prefixes ( string )
	char *string;
{
	static char *prefixes[] = { "kilo", "micro", "milli", "intra", "ultra",
					"mega", "nano", "pico", "pseudo", 0 };
	int i;
	for ( i=0 ; prefixes[i] != 0 ; i++ ) {
		if ( strncmp(prefixes[i],string,strlen(prefixes[i])) == 0 ) {
			int j;
			for ( j=0 ; j<strlen(string)-strlen(prefixes[i]) ; j++ )
				string[j] = string[j+strlen(prefixes[i])];
			string[j] = '\0';
			return;
		}
	}
}
#endif PREFIXES

static void strip_suffixes ( string )
	char *string;
{
	void step_1(), step_2(), step_3(), step_4(), step_5();
	step_1 ( string );
	step_2 ( string );
	step_3 ( string );
	step_4 ( string );
	step_5 ( string );
}

static void step_1 ( string )
	char *string;
{
	char stem[BIG_KEYWORDSIZE];
	if ( string[strlen(string)-1] == 's' ) {
		if ( (has_suffix(string,"sses",stem) == TRUE) ||
				(has_suffix(string,"ies",stem) == TRUE ) )
			string[strlen(string)-2] = '\0';
		else
			if ( string[strlen(string)-2] != 's' )
				string[strlen(string)-1] = '\0';
		}
	if ( has_suffix(string,"eed",stem) == TRUE ) {
		if ( measure(stem) > 0 )
			string[strlen(string)-1] = '\0';
	}
	else {	if ( ( (has_suffix(string,"ed",stem) == TRUE )
				|| (has_suffix(string,"ing",stem) == TRUE ) )
				&& (contains_vowel(stem) == TRUE ) ) {
			string[strlen(stem)] = '\0';
			if ( ( has_suffix(string,"at",stem) == TRUE )
				|| ( has_suffix(string,"bl",stem) == TRUE )
				|| ( has_suffix(string,"iz",stem) == TRUE ) ) {
				string[strlen(string)+1] = '\0';
				string[strlen(string)] = 'e';
			}
			else {	int length = strlen(string);
				if ( (string[length-1] == string[length-2])
						&& (string[length-1] != 'l')
						&& (string[length-1] != 's')
						&& (string[length-1] != 'z') )
					string[length-1] = '\0';
				else
					if ( measure(string) == 1 ) {
					     if ( cvc(string) == TRUE ) {
						string[strlen(string)+1] = '\0';
						string[strlen(string)] = 'e';
						}
					}
			}
		}
	}
	if ( (has_suffix(string,"y",stem) == TRUE) &&
					(contains_vowel(stem) == TRUE) )
		string[strlen(string)-1] = 'i';
}

static void step_2 ( string )
	char *string;
{
	static char *suffixes[][2] =  { { "ational", "ate" },
				        { "tional",  "tion" },
				        { "enci",    "ence" },
					{ "anci",    "ance" },
					{ "izer",    "ize" },
					{ "iser",    "ize" },
					{ "abli",    "able" },
					{ "alli",    "al" },
					{ "entli",   "ent" },
					{ "eli",     "e" },
					{ "ousli",   "ous" },
					{ "ization", "ize" },
					{ "isation", "ize" },
					{ "ation",   "ate" },
					{ "ator",    "ate" },
					{ "alism",   "al" },
					{ "iveness", "ive" },
					{ "fulness", "ful" },
					{ "ousness", "ous" },
					{ "aliti",   "al" },
					{ "iviti",   "ive" },
					{ "biliti",  "ble" },
					{  0,        0     } };
	char stem[BIG_KEYWORDSIZE];
	register int index;
	for ( index = 0 ; suffixes[index][0] != 0 ; index++ ) {
		if ( has_suffix ( string, suffixes[index][0], stem ) == TRUE ) {
			if ( measure ( stem ) > 0 ) {
				sprintf ( string, "%s%s", stem, suffixes[index][1] );
				return;
			}
		}
	}
	return;
}

static void step_3 ( string )
	char *string;
{
	static char *suffixes[][2] = { { "icate", "ic" },
				       { "ative", "" },
				       { "alize", "al" },
				       { "alise", "al" },
				       { "iciti", "ic" },
				       { "ical",  "ic" },
				       { "ful",   "" },
				       { "ness",  "" },
				       { 0,       0 } };
	char stem[BIG_KEYWORDSIZE];
	register int index;
	for ( index = 0 ; suffixes[index][0] != 0 ; index++ ) {
		if ( has_suffix ( string, suffixes[index][0], stem ) == TRUE )
			if ( measure ( stem ) > 0 ) {
				sprintf ( string, "%s%s", stem, suffixes[index][1] );
				return;
			}
	}
	return;
}

static void step_4 ( string )
	char *string;
{
	static char *suffixes[] = { "al", "ance", "ence", "er", "ic", "able",
		"ible", "ant", "ement", "ment", "ent", "sion", "tion",
		"ou", "ism", "ate", "iti", "ous", "ive", "ize", "ise", 0 };
	char stem[BIG_KEYWORDSIZE];
	register int index;
	for ( index = 0 ; suffixes[index] != 0 ; index++ ) {
		if ( has_suffix ( string, suffixes[index], stem ) == TRUE )
			if ( measure ( stem ) > 1 ) {
				strcpy( string, stem );
				return;
			}
	}
	return;
}

static void step_5 ( string )
	char *string;
{
	if ( string[strlen(string)-1] == 'e' ) {
		if ( measure(string) > 1 )
			/* measure(string)==measure(stem) if ends in vowel */
			string[strlen(string)-1] = '\0';
		else
			if ( measure(string) == 1 ) {
				char stem[BIG_KEYWORDSIZE];
				strcpy(stem,"");
				strncat( stem, string, strlen(string)-1 );
				if ( cvc(stem) == FALSE )
					string[strlen(string)-1] = '\0';
			}
	}
	if ( (string[strlen(string)-1] == 'l')
				&& (string[strlen(string)-2] == 'l')
				&& (measure(string) > 1) )
		string[strlen(string)-1] = '\0';
}

static has_suffix ( word, suffix, stem )
	char *word, *suffix, *stem;
{
	char tmp[BIG_KEYWORDSIZE];
	if ( strlen(word) <= strlen(suffix) )
		return(FALSE);
	if ( (strlen(suffix) > 1) &&
			( word[strlen(word)-2] != suffix[strlen(suffix)-2] ) )
		return(FALSE);
	strcpy ( stem, "" );
	strncat ( stem, word, strlen(word) - strlen(suffix) );
	strcpy ( tmp, stem );
	strcat ( tmp, suffix );
	if ( strcmp (  tmp, word ) == 0 )
		return(TRUE);
	else
		return(FALSE);
}

static int cvc ( string )
	char *string;
{
	int length=strlen(string);
	if ( length < 3 )
		return(FALSE);
	if ( (vowel(string[length-1],string[length-2]) == FALSE)
			&& (string[length-1] != 'w')
			&& (string[length-1] != 'x')
			&& (string[length-1] != 'y')
			&& (vowel(string[length-2],string[length-3]) == TRUE)
			&& ( ( (length==3) && (vowel(string[0],'a') == FALSE) )
			|| (vowel(string[length-3],string[length-4])==FALSE) ) )
		return(TRUE);
	else
		return(FALSE);
}

static int vowel ( ch, prev )
	char ch, prev;
{
	switch ( ch ) {
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u': return(TRUE);
			  break;
		case 'y': return( vowel(prev,'?') == TRUE );
			  break;
		default : return(FALSE);
			  break;
	}
}

static measure ( stem )
	char *stem;
{
	register int i=0, count = 0;
	int length=strlen(stem);
	while ( i < length ) {
		for ( ; i < length ; i++ ) {
			if ( i > 0 ) {
				if ( vowel(stem[i],stem[i-1]) == TRUE )
					break;
			}
			else {	if ( vowel(stem[i],'a') == TRUE )
					break;
			}
		}
		for ( i++ ; i < length ; i++ ) {
			if ( i > 0 ) {
				if ( vowel(stem[i],stem[i-1]) == FALSE )
					break;
			}
			else {	if ( vowel(stem[i],'?') == FALSE )
					break;
			}
		}
		if ( i < length ) {
			count++;
			i++;
		}
	}
	return(count);
}

static int contains_vowel ( word )
	char *word;
{
	register int i;
	for ( i=0 ; i < strlen(word) ; i++ )
		if ( i > 0 ) {
			if ( vowel(word[i],word[i-1]) == TRUE )
				return(TRUE);
		}
		else {	if ( vowel(word[0],'a') == TRUE )
				return(TRUE);
		}
	return(FALSE);
}

