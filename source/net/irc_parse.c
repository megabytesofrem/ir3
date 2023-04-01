/*
    https://modern.ircdocs.horse/index.html
*/
#include "irc_parse.h"

#include <stdio.h>

static char parsePeek(struct ParserState *ps)
{
    if (ps->pos <= strlen(ps->raw))
        return ps->raw[ps->pos];
    else
        return '\0';
}

static void parseSource(struct Message *msg, struct ParserState *ps)
{
    int len, prev = 0;

    // calculate length
    while (parsePeek(ps) != ' ')
    {
        len++;
        ps->pos++;
    }

    char *buffer = malloc(sizeof(char) * len + 1);
    prev = ps->pos;
    ps->pos -= len;

    memcpy(buffer, ps->raw + ps->pos, len);
    buffer[len] = 0;

    printf("\nsource: %s\n", buffer);
    ps->pos = prev;
    msg->source = buffer;
}

static void parseCommand(struct Message *msg, struct ParserState *ps)
{
    int len, prev = 0;

    // parse the comamnd
    ps->pos++;
    while (parsePeek(ps) != ' ')
    {
        len++;
        ps->pos++;
    }

    char *buffer = malloc(sizeof(char) * len + 1);
    prev = ps->pos;
    ps->pos -= len;

    memcpy(buffer, ps->raw + ps->pos, len);
    buffer[len] = 0;

    printf("\ncommand: %s\n", buffer);
    ps->pos = prev;
    msg->command = buffer;

    ps->pos++;
}

static void parseParams(struct Message *msg, struct ParserState *ps)
{
    int len, prev = 0;
    while (parsePeek(ps) != '\r')
    {
        len++;
        ps->pos++;
    }

    char *buffer = malloc(sizeof(char) * len + 1);
    prev = ps->pos;
    ps->pos -= len;
    memcpy(buffer, ps->raw + ps->pos, len);
    buffer[len] = 0;

    printf("\nparams: %s\n", buffer);
    ps->pos = prev;
    msg->command = buffer;
    ps->state = PARSE_DONE;
}

struct Message *parse(struct ParserState *ps)
{
    struct Message *msg = malloc(sizeof(struct Message));

    // :source PRIVMSG user :test
    printf("\nbeginning parsing\n");
    char c = parsePeek(ps);
    while (ps->state != PARSE_DONE)
    {
        c = parsePeek(ps);

        switch (c)
        {
        case ':':
            ps->pos++;
            parseSource(msg, ps);
            ps->state = PARSED_SOURCE;
            break;
        case '\r':
            ps->state = PARSE_DONE;
            break;
        default:
            if (ps->state != PARSED_COMMAND)
            {
                parseCommand(msg, ps);
                ps->state = PARSED_COMMAND;
            }
            else
            {
                parseParams(msg, ps);
            }
            break;
        }
    }
    printf("\ndone parsing\n");

    return msg;
}

struct ParserState *parserCreate(const char *raw)
{
    struct ParserState *ps = malloc(sizeof(struct ParserState));
    ps->raw = raw;
    ps->pos = 0;
    ps->state = PARSE_INITIAL;

    return ps;
}

void parserFree(struct ParserState *ps)
{
    free(ps);
}