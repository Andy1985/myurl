%{
#include "stdio.h"
#include "stdlib.h"
%}

%%
" porn "	{ return 100; }
" viagra " 	{ return 101; }
" cialis " 	{ return 102; }
" vigra " 	{ return 103; }
" pill "    { return 105; }
"Õ≈π∫"      { return 106; }
%%

int yywrap(yyscan_t lexer)
{
    return 1;
}

int my_keycheck(const char* data)
{
	int ret = 0;
	yyscan_t lexer;

	yylex_init(&lexer);
	yyset_extra(NULL,lexer);
	yy_scan_string(data,lexer);
	ret = yylex(lexer);
	yylex_destroy(lexer);

	return ret;
}
