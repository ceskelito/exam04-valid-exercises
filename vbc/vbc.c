#include <stdio.h>
#include <malloc.h> // change this to <stdlib.h>
#include <ctype.h>

enum {
	ADD,
	MULTI,
	VAL
};

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

#include <limits.h>
#include <stdbool.h>

enum { PAR_WEIGHT = 10 };

#define GET_PRIOR(weight, depth) ( weight + (depth * PAR_WEIGHT) )

int get_type(char c)
{	
	if ( c == '+' )
		return ADD;
	else if ( c == '*' )
		return MULTI;
	else if ( isdigit(c) )
		return VAL;
	return -1;
}

int n(char *s) {

	int	res;
	struct s_elem {
		int		val;		// Value of the element. Only considered if the type is VAL.
		int		depth;		// Paranthesis depth of the element.
		int		weight;		// Element type (val, sum, multi). Also encodes the operator precedence (excluding parentheses).
		int		priority;	// Effective precedence, derived from the type and the current parentheses depth.
		char	*ptr;		// Pointer to the element in the original expression's string.
	} curr, min_prior;

	min_prior.priority = INT_MAX;
	min_prior.weight = VAL + 1;
	min_prior.val = 0;
	min_prior.ptr = NULL;

	curr.depth = 0;
	curr.priority = 0;

	// Find the less prioritized element (including parenthesis)
	for ( int i = 0; s[i]; i++ )
	{
		if ( s[i] == '(' )
		{
			curr.depth++;
			continue;
		}
		else if ( s[i] == ')' )
		{
			curr.depth--;
			continue;
		}

		curr.ptr = &s[i];
		curr.weight = get_type(s[i]);
		if ( curr.weight == VAL )
			curr.val = s[i] - '0';
		curr.priority = GET_PRIOR(curr.weight, curr.depth);
		if ( curr.priority < min_prior.priority )
			min_prior = curr;
	}
	if ( min_prior.weight == VAL )
		return min_prior.val;
	if (min_prior.ptr)
		*min_prior.ptr = '\0';
	if ( min_prior.weight == ADD )
		return ( n(s) + n(min_prior.ptr + 1) );
	else if ( min_prior.weight == MULTI )
		return ( n(s) * n(min_prior.ptr + 1) );
	return 0;
}

bool check_set(char c, char *set)
{
	for (int i = 0; set[i]; i++)
		if (c == set[i])
			return true;
	return false;
}

int validate_input(char *s)
{
	enum { ERR = -1, NICE = 0 };

	if ( !s )
		return ERR;

	int d = 0;

	for ( int i = 0; s[i]; i++ ) {
		if ( s[i] == '(' )
			d++;
		else if ( s[i] == ')' )
			d--;
		if ( d < 0 ) {
			unexpected(s[i]);
			return ERR;
		}

		if ( s[i] == '(' )
		{
			if ( !s[i+1] || !check_set(s[i+1], "()0123456789") ) {
				unexpected(s[i+1]);
				return ERR;		
			}
		}
		else if ( s[i] == ')' )
		{
			if ( s[i+1] && !check_set(s[i+1], ")*+") ) {
				unexpected(s[i+1]);
				return ERR;		
			}
		}
		else if ( check_set(s[i], "+*"))
		{
			if ( !s[i+1] || !check_set(s[i+1], "(0123456789") ) {
				unexpected(s[i+1]);
				return ERR;
			}
		}
		else if ( isdigit(s[i]) )
		{
			if ( s[i+1] && !check_set(s[i+1], "*+)") ) {
				unexpected(s[i+1]);
				return ERR;
			}
		}
		else {
			unexpected(s[i]);
			return ERR;
		}
	}
	if ( d != 0 ) {
		unexpected('\0');
		return ERR;
	}
	return NICE;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
	if ( validate_input(argv[1]) == -1 )
		return(1);
	printf("%d\n", n(argv[1]));
}
