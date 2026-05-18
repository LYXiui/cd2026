#include <stdio.h>
#include <string.h>

/* ─────────────────────────────────────────────
   Grammar:
     S  -> E S'
     S' -> ε
     S' -> + S
     E  -> num
     E  -> ( S )
   ───────────────────────────────────────────── */

/* ── Token types ── */
#define TOK_NUM   1
#define TOK_PLUS  2
#define TOK_LPAREN 3
#define TOK_RPAREN 4
#define TOK_EOF   5
#define TOK_ERR   -1

/* ── Input buffer ── */
static char input[1024];
static int  pos = 0;

/* ── Current token ── */
static int  token;
static char token_val[64];   /* stores the number string when token == TOK_NUM */

/* ── Indentation depth for output ── */
static int depth = 0;

/* ── Forward declarations ── */
void parse_S(void);
void parse_Sp(void);   /* parse_S' */
void parse_E(void);

/* ════════════════════════════════════════════
   SCANNER  (HW#2 style)
   Returns the next token from input[].
   Skips whitespace; recognises: number, +, (, ), EOF.
   ════════════════════════════════════════════ */
int scanner(void)
{
    /* skip whitespace */
    while (input[pos] == ' ' || input[pos] == '\t' ||
           input[pos] == '\n' || input[pos] == '\r')
        pos++;

    if (input[pos] == '\0') {
        strcpy(token_val, "EOF");
        return TOK_EOF;
    }

    if (input[pos] == '+') {
        token_val[0] = '+'; token_val[1] = '\0';
        pos++;
        return TOK_PLUS;
    }
    if (input[pos] == '(') {
        token_val[0] = '('; token_val[1] = '\0';
        pos++;
        return TOK_LPAREN;
    }
    if (input[pos] == ')') {
        token_val[0] = ')'; token_val[1] = '\0';
        pos++;
        return TOK_RPAREN;
    }

    /* number: one or more digits */
    if (input[pos] >= '0' && input[pos] <= '9') {
        int i = 0;
        while (input[pos] >= '0' && input[pos] <= '9')
            token_val[i++] = input[pos++];
        token_val[i] = '\0';
        return TOK_NUM;
    }

    /* unknown character */
    token_val[0] = input[pos]; token_val[1] = '\0';
    pos++;
    return TOK_ERR;
}

/* ════════════════════════════════════════════
   PRINT helpers
   ════════════════════════════════════════════ */
static void print_indent(void)
{
    int i;
    for (i = 0; i < depth; i++)
        printf("  ");          /* 2 spaces per level */
}

static void print_rule(const char *rule)
{
    print_indent();
    printf("%s\n", rule);
}

static void print_token(const char *val)
{
    print_indent();
    printf("%s\n", val);
}

/* ════════════════════════════════════════════
   PARSE functions
   ════════════════════════════════════════════ */

/*
   S -> E S'
   FIRST(S) = { num, '(' }
*/
void parse_S(void)
{
    print_rule("S -> E S'");
    depth++;
    parse_E();
    parse_Sp();
    depth--;
}

/*
   S' -> ε        if token ∈ { ')', EOF }
   S' -> + S      if token == '+'
*/
void parse_Sp(void)
{
    if (token == TOK_PLUS) {
        print_rule("S' -> + S");
        depth++;
        print_token("+");           /* consume '+' */
        token = scanner();
        parse_S();
        depth--;
    } else if (token == TOK_RPAREN || token == TOK_EOF) {
        print_rule("S' -> ε");
    } else {
        printf("Parse error in S': unexpected token '%s'\n", token_val);
        /* hard stop */
        while (1) ;
    }
}

/*
   E -> num        if token == num   → consume, advance
   E -> ( S )      if token == '('  → consume '(', parse S, expect ')', advance
*/
void parse_E(void)
{
    if (token == TOK_NUM) {
        print_rule("E -> num");
        depth++;
        print_token(token_val);     /* print the number */
        depth--;
        token = scanner();          /* advance */
    } else if (token == TOK_LPAREN) {
        print_rule("E -> ( S )");
        depth++;
        print_token("(");           /* consume '(' */
        token = scanner();
        parse_S();
        if (token != TOK_RPAREN) {
            printf("Parse error in E: expected ')' but got '%s'\n", token_val);
            while (1) ;
        }
        print_token(")");           /* consume ')' */
        token = scanner();
        depth--;
    } else {
        printf("Parse error in E: unexpected token '%s'\n", token_val);
        while (1) ;
    }
}

/* ════════════════════════════════════════════
   MAIN
   ════════════════════════════════════════════ */
int main(void)
{
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("No input.\n");
        return 1;
    }

    /* remove trailing newline */
    int len = (int)strlen(input);
    if (len > 0 && input[len-1] == '\n') input[--len] = '\0';

    pos   = 0;
    token = scanner();   /* prime the lookahead */

    printf("\nParse tree:\n");
    parse_S();

    if (token == TOK_EOF)
        printf("\nParsing successful.\n");
    else
        printf("\nParse error: leftover input '%s'\n", token_val);

    return 0;
}
