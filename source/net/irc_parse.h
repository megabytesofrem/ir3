/**
 * IRC message parser written in C
 */

#ifndef _IRTHREE_PARSE_
#define _IRTHREE_PARSE_

#include <string.h>

struct Message
{
    // optional note of where the message came from (prefixed with :)
    char *source;
    char *command;
    char *params;
};

enum ParseState
{
    PARSE_INITIAL,
    PARSE_DONE,
    PARSED_SOURCE,
    PARSED_COMMAND,
    PARSED_ARGS,
};

struct ParserState
{
    char *raw;
    int pos;
    enum ParseState state;
};

static char parsePeek(struct ParserState *ps);
static void parseSource(struct Message *msg, struct ParserState *ps);
static void parseCommand(struct Message *msg, struct ParserState *ps);
static void parseParams(struct Message *msg, struct ParserState *ps);

struct Message *parse(struct ParserState *ps);
struct ParserState *parserCreate(const char *raw);
void parserFree(struct ParserState *ps);

#endif