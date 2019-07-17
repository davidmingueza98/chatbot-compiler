#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: lego.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define NUM 2
#define GRID 3
#define PLACE 4
#define MOVE 5
#define WHILE 6
#define AT 7
#define AND 8
#define NOT 9
#define OR 10
#define F 11
#define CLAU1 12
#define CLAU2 13
#define BRACKET1 14
#define BRACKET2 15
#define PAR1 16
#define PAR2 17
#define HASHTG 18
#define SEMICOLON 19
#define COMMA 20
#define EQUALS 21
#define MENOR 22
#define MAYOR 23
#define ENDQ 24
#define END 25
#define WRITE 26
#define TXT 27
#define COLON 28
#define SPACE 29

#ifdef __USE_PROTOS
void lego(AST**_root);
#else
extern void lego();
#endif

#ifdef __USE_PROTOS
void grid(AST**_root);
#else
extern void grid();
#endif

#ifdef __USE_PROTOS
void instructions(AST**_root);
#else
extern void instructions();
#endif

#ifdef __USE_PROTOS
void instruction(AST**_root);
#else
extern void instruction();
#endif

#ifdef __USE_PROTOS
void place(AST**_root);
#else
extern void place();
#endif

#ifdef __USE_PROTOS
void local(AST**_root);
#else
extern void local();
#endif

#ifdef __USE_PROTOS
void move(AST**_root);
#else
extern void move();
#endif

#ifdef __USE_PROTOS
void whiler(AST**_root);
#else
extern void whiler();
#endif

#ifdef __USE_PROTOS
void condition(AST**_root);
#else
extern void condition();
#endif

#ifdef __USE_PROTOS
void expr(AST**_root);
#else
extern void expr();
#endif

#ifdef __USE_PROTOS
void evaluation(AST**_root);
#else
extern void evaluation();
#endif

#ifdef __USE_PROTOS
void puta(AST**_root);
#else
extern void puta();
#endif

#ifdef __USE_PROTOS
void params(AST**_root);
#else
extern void params();
#endif

#ifdef __USE_PROTOS
void param(AST**_root);
#else
extern void param();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType setwd1[];
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType setwd2[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType setwd3[];
