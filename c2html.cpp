#include "c-lex.hpp"

#include <cassert>

void printe(const char *s)
{
    while (*s)
    {
        switch (*s)
        {
        case '<': printf("&lt;"); break;
        case '>': printf("&gt;"); break;
        case '&': printf("&amp;"); break;
        default: putchar(*s); break;
        }
        ++s;
    }
}

void prints(const char *c, const char *t)
{
    printf("<span class=\"%s\">", c);
    printe(t);
    printf("</span>");
}

// Single line comment
/* multiline comment
 * **
 */
int main(int argc, char **argv)
{
    float ignored = 0. + .0;
    (void)ignored;

    if (argc != 2 || argv[1][0] == '-')
    {
        fprintf(stderr, "Usage: %s file.c > file.html\n", argv[0]);
        return 42;
    }

    FILE *infile = fopen(argv[1], "r");
    if (!infile)
    {
        fprintf(stderr, "Error: failed to read %s\n", argv[1]);
        return 43;
    }
    yyscan_t scanner;
    cclex_init(&scanner);
    ccset_in(infile, scanner);

    printf(
        "<!doctype html>\n"
        "<html>\n"
        "<head>\n"
        "<title>%s</title>\n"
        "<style type=\"text/css\">\n"
        "code { white-space: pre ; display: block; unicode-bidi: embed }\n"
        ".directive { color: darkmagenta }\n"
        ".comment { color: blue }\n"
        "/* .line { color: gray } */\n"
        ".operator { color: black; font-weight: bold }\n"
        ".keyword { color: green; font-weight: bold }\n"
        ".constant { color: red; font-weight: bold }\n"
        ".identifier { color: black }\n"
        ".number { color: red }\n"
        ".string { color: red }\n"
        ".character { color: red }\n"
        ".unknown { background-color: red }\n"
        "</style>\n"
        "</head>\n"
        "<body>\n"
        "<ol>\n",
        argv[1]
    );
    bool newline = true;
    // yylineno doesn't actually work
    //int line = 1;
    while (int tok = cclex(scanner))
    {
        const char *text = ccget_text(scanner);
        if (newline)
        {
            printf("<li> <code>");
            newline = false;
        }
        switch (tok)
        {
        case NEWLINE:
            printf("</code> </li>\n");
            newline = true;
            //++line;
            break;
        case WHITESPACE:
            // prints("whitespace", text);
            printe(text);
            break;
        case COMMENT:
            prints("comment", text);
            break;
        case DIRECTIVE:
            prints("directive", text);
            break;
        case KEYWORD:
            prints("keyword", text);
            break;
        case CONSTANT:
            prints("constant", text);
            break;
        case IDENTIFIER:
            prints("identifier", text);
            break;
        case NUMBER:
            prints("literal", text);
            break;
        case STRING:
            prints("string", text);
            break;
        case CHARACTER:
            prints("character", text);
            break;
        case OPERATOR:
            prints("operator", text);
            break;

        default:
            assert (tok < 256);
            /* fallthrough */
        case UNKNOWN:
            prints("unknown", text);
            break;
        }
    }
    printf(
        "</ol>\n"
        "</body>\n"
        "</html>\n"
    );

    cclex_destroy(scanner);
}
