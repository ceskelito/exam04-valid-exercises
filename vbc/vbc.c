/**
 * Assignment name  : vbc
 * Expected files   : *.c *.h
 * Allowed functions: malloc, calloc, realloc, free, printf, isdigit, write
 * --------------------------------------------------------------------------------
 * 
 * Write a program that will print the result of a math expression given as
 * argument.
 * You must handle the operations + * and the parenthesis.
 * You don't have to handle whitespaces in the expression.
 * All the values in the expression will be between 0 and 9 included.
 * In case of unexpected symbol or inappropriate parenthesis, you will
 * print "Unexpected token '%c'
 * " and exit with the code 1 (if the
 * symbol is the end of input you will print: "Unexpected end of input
 * ").
 * In case of a syscall failure you will just exit with the code 1.
 * 
 * You will find in this directory the beginning of the code you need to write.
 */

#include <stdio.h>
#include <malloc.h> // change this to <stdlib.h>
#include <ctype.h>
#include <limits.h>

enum { ERR = 1, NICE = 0};

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node *n(char **s);

int	validate_input(char *s) {

	if ( !isdigit(s[0]) || s[0] != '(' )
		return ERR;
	for ( int i = 0; s[i]; i++ ) {

		if ( s[i] == '(' || s[i] == '+' || s[i] == '*' ) {
			if ( !s[i + 1] || 
					!( isdigit(s[i + 1]) || s[i + 1] == '(' )) {
				return ERR;	
			}
		} else if ( s[i] == ')' || isdigit(s[i]) ) {
			if ( s[i + 1] && 
					!( s[i + 1] == '(' || s[i + 1] == '+' || s[i + 1] == '*' ) ) {
				return ERR;
			}
		} else
			return ERR;
	}
	return NICE;
}

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

//...

// node    *parse_expr(char *s)
// {
//     //...
//
//     if (*s) 
//     {
//         destroy_tree(ret);
//         return (NULL);
//     }
//     return (ret);
// }

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
}

int get_prior( char c ) {
	if ( c == '+' )
		return ADD;
	if ( c == '*' )
		return MULTI;
	if ( isdigit(c) )
		return VAL;
	return ( -1 );
}

node *n(char **s) {

	// The variables names in this function have to be improved.

	enum { PAR_WEIGHT = 10 };

	if ( !s || !*s )
		return NULL;

	struct s_min_prior {
		int type; // Net weight of the element -> aka the node's type in the tree.
		int value; // Composed weight (priority) of the element. It means: net weight + parenthesis depth * parenthesis weight.
		char *ptr; // Pointer to the less prioritized element in the string.
	} min_prior;
	min_prior.value = INT_MAX;
	min_prior.type = VAL;
	min_prior.ptr = 0;
	
	node new;

	int p = 0; // Current element's net weight
	int pp = 0; // Parenthesis depth

	// Find the less prioritized node (including parenthesis)
	for ( int i = 0; (*s)[i]; i++ ) {

		if ( (*s)[i] == '(' ){
			pp++;
			continue;
		}
		else if ( (*s)[i] == ')' ) {
			pp--;
			continue;
		}

		// `p' contains the net weight of the curr element.
		// `p' contains -1 in case of unexpected element.
		p = get_prior((*s)[i]);

		// To obtain the real priority of the element simply sum the Net Weight with the Parenthesis Full Weight: `p' + `pp' * `PAR_WEIGHT'.
		if ( p != -1 && (p + pp * PAR_WEIGHT) < min_prior.value ) {
			min_prior.value = (p + pp * PAR_WEIGHT);
			min_prior.type = p;
			min_prior.ptr = (*s) + i;
		}
	}

	new.type = min_prior.type;
	if ( new.type == VAL ) {
		new.val = min_prior.ptr[0] - '0';
		return (new_node(new));
	}
	*min_prior.ptr = 0;
	char *right_s = min_prior.ptr + 1;
	if ( right_s )
		new.r = n(&right_s);
	new.l = n(s);
	
	return new_node(new);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
	// if ( validate_input(argv[1]) == ERR )
	// 	return ERR;
    node *tree = n(&argv[1]);//parse_expr(argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
