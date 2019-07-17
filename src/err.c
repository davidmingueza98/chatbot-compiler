/*
 * A n t l r  S e t s / E r r o r  F i l e  H e a d e r
 *
 * Generated from: lego.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
#include <map>
#include <cstdlib>
using namespace std;

// struct to store information about tokens
typedef struct {
  string kind; //tipus de token o el número que indica que tipus és, cada token en veritat té un número asociat
  string text;
} Attrib;

// function to fill token information (predeclaration)
void zzcr_attr(Attrib *attr, int type, char *text);

// fields for AST nodes
#define AST_FIELDS string kind; string text; //en el lloc del codi que posi AST_FIELDS cambiara per el que posa a la dreta
#include "ast.h"

// macro to create a new AST node (and function predeclaration)
#define zzcr_ast(as,attr,ttype,textt) as=createASTnode(attr,ttype,textt)
AST* createASTnode(Attrib* attr, int ttype, char *textt); // sinó el compilador es queixa
#define zzSET_SIZE 4
#include "antlr.h"
#include "ast.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[30]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"NUM",
	/* 03 */	"GRID",
	/* 04 */	"PLACE",
	/* 05 */	"MOVE",
	/* 06 */	"WHILE",
	/* 07 */	"AT",
	/* 08 */	"AND",
	/* 09 */	"NOT",
	/* 10 */	"OR",
	/* 11 */	"F",
	/* 12 */	"CLAU1",
	/* 13 */	"CLAU2",
	/* 14 */	"BRACKET1",
	/* 15 */	"BRACKET2",
	/* 16 */	"PAR1",
	/* 17 */	"PAR2",
	/* 18 */	"HASHTG",
	/* 19 */	"SEMICOLON",
	/* 20 */	"COMMA",
	/* 21 */	"EQUALS",
	/* 22 */	"MENOR",
	/* 23 */	"MAYOR",
	/* 24 */	"ENDQ",
	/* 25 */	"END",
	/* 26 */	"WRITE",
	/* 27 */	"TXT",
	/* 28 */	"COLON",
	/* 29 */	"SPACE"
};
SetWordType zzerr1[4] = {0x60,0x0,0x0,0x8};
SetWordType setwd1[30] = {0x0,0xfb,0x0,0x0,0x0,0xf6,0xf6,
	0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0xf6,0x0,0x0};
SetWordType zzerr2[4] = {0x0,0x5,0x0,0x0};
SetWordType zzerr3[4] = {0x0,0x0,0xc0,0x0};
SetWordType zzerr4[4] = {0x0,0x5,0xc2,0x0};
SetWordType zzerr5[4] = {0x4,0x0,0x0,0x8};
SetWordType setwd2[30] = {0x0,0x1,0x0,0x0,0x0,0x1,0x1,
	0x0,0xf2,0x0,0xf2,0x0,0x0,0x0,0x0,
	0x0,0x0,0xf4,0x0,0x0,0x0,0x0,0xc8,
	0xc8,0x0,0x0,0x0,0x1,0x0,0x0};
SetWordType zzerr6[4] = {0x4,0x0,0x0,0x8};
SetWordType zzerr7[4] = {0x0,0x0,0x12,0x0};
SetWordType setwd3[30] = {0x0,0x0,0x5,0x0,0x0,0x0,0x0,
	0x0,0x6,0x0,0x6,0x0,0x0,0x0,0x0,
	0x0,0x0,0x6,0x0,0x0,0x0,0x0,0x6,
	0x6,0x0,0x0,0x0,0x5,0x0,0x0};
