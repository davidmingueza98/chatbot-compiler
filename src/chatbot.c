/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   antlr -gt chatbot.g
 *
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
#include <map>
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
#define GENAST

#include "ast.h"

#define zzSET_SIZE 4
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "mode.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

#include "ast.c"
zzASTgvars

ANTLR_INFO

#include <cstdlib>
#include <cmath>
// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) { //type of token que el tindre disponible en altres moments
  
    attr->kind = text;
  attr->text = "";
  
}

// donat un apuntador als germans, retorna l’AST amb “list” com arrel i els germans com a fills.
AST* createASTlist(AST* n){
  
 AST* as = new AST;
  as->kind = "list";
  as->right = NULL;
  as->down = n;
  return as;
}

// function to create a new AST node
AST* createASTnode(Attrib* attr, int type, char* text) {
  AST* as = new AST;
  as->kind = attr->kind; 
  as->text = attr->text;
  as->right = NULL; 
  as->down = NULL;
  return as;
}

/// get nth child of a tree. Count starts at 0.
/// if no such child, returns NULL
//right i down es per transformar un arbre general a un arbre binari, down=primer fill(esquerra), right= germans REGLA D'OR
AST* child(AST *a,int n) {
AST *c=a->down;
for (int i=0; c!=NULL && i<n; i++) c=c->right;
return c;
} 

/// print AST, recursively, with indentation
void ASTPrintIndent(AST *a,string s)
{
if (a==NULL) return;

  cout<<a->kind;
if (a->text!="") cout<<"("<<a->text<<")";
cout<<endl;

  AST *i = a->down;
while (i!=NULL && i->right!=NULL) {
cout<<s+"  \\__";
ASTPrintIndent(i,s+"  |"+string(i->kind.size()+i->text.size(),' '));
i=i->right;
}

  if (i!=NULL) {
cout<<s+"  \\__";
ASTPrintIndent(i,s+"   "+string(i->kind.size()+i->text.size(),' '));
i=i->right;
}
}

/// print AST 
void ASTPrint(AST *a)
{
while (a!=NULL) {
cout<<" ";
ASTPrintIndent(a,"");
a=a->right;
}
}

map<string, AST*> nodos;

int seed;
string idChatBot;
string name;

void leerConversationTags(AST *a){
a = (a->down);
a = (a->down);
while(a!=NULL){
nodos[a->kind]=a;
a = a->right;
}
}

void leerChatbotsTags(AST *a){ //guarda on estan les definicions en un map
a = (a->down);
while(a!=NULL){
nodos[a->kind]=a;
a = a->right;
}
}

void evaluateQuestion(AST *a){
a = nodos[a->kind];
a = a->down;
a = a->down;

	cout << idChatBot << " > " << name << ", ";

		while(a!=NULL){
cout << a->kind << " ";
a = a->right;
}
cout << "?" << endl;

}

void evaluateAnswer(AST *a){
a = nodos[a->kind];
a = a->down;
a = a->down;

	while(a!=NULL){
cout << a->kind << ": ";
AST *b = a;
b = b->down;
b = b->down; //baixa a les respostes
while(b!=NULL){
cout << b->kind << " ";
b = b->right;
}
cout << endl;
a = a->right;
}

}

void evaluateConversation(AST *a){
a = nodos[a->kind];
a = a->down;
a = a->down;

	evaluateQuestion(a);

	a = a->right;
evaluateAnswer(a);

	cout << name << " > ";

	string ans;
cin >> ans;
}

int getSize(AST *b){
int result = 0;
b = b->down;
while(b!=NULL){
++result;
b = b->right;
}
return result;
}

void evaluateOr(AST *a);


void evaluateThen (AST *a) {
a = a->down;
while(a!=NULL){
if (a->kind == "OR") evaluateOr(a);
else if (a->kind == "THEN") evaluateThen(a);
else evaluateConversation(a);
a = a->right;
}
}

void evaluateOr(AST *a){
AST *b = a;
int size = getSize(b); //obte el numero de fills
int elect = rand()%size;

	a = child(a,elect); //selecciona un fill
if (a->kind == "THEN") evaluateThen(a);
else if (a->kind == "OR") evaluateOr(a);
else evaluateConversation(a);
}

void evaluateStartchat(AST *a){
a = nodos[a->kind]; //busca en el map l'arbre on està definit
idChatBot = a->kind;
cout << "\n\n"; //per visualitzar millor
cout << idChatBot << " > WHAT IS YOUR NAME ?" << endl;
cin >> name;
a = a->down;

	if (a->kind == "THEN") evaluateThen(a);
else if (a->kind == "OR") evaluateOr(a);
else evaluateConversation(a);

	cout << idChatBot << " > THANKS FOR THE CHAT " << name << "!" << endl;

}


void evaluate(AST *a) {
a = a->down;
seed = atoi(a->kind.c_str());
srand(seed); //llavor de generació
a = a->right;
while(a!=NULL){
AST *b = a;
evaluateStartchat(b);
a = a->right;
}

}


int main() {
AST *root = NULL;
ANTLR(chatbot(&root), stdin);
ASTPrint(root);
leerConversationTags(root);
leerChatbotsTags(child(root,1));
evaluate(child(root,2));
}

  

void
#ifdef __USE_PROTOS
chatbot(AST**_root)
#else
chatbot(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  conversations(zzSTR); zzlink(_root, &_sibling, &_tail);
  chats(zzSTR); zzlink(_root, &_sibling, &_tail);
  startchat(zzSTR); zzlink(_root, &_sibling, &_tail);
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x1);
  }
}

void
#ifdef __USE_PROTOS
conversations(AST**_root)
#else
conversations(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==ID) ) {
      conversa(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x2);
  }
}

void
#ifdef __USE_PROTOS
conversa(AST**_root)
#else
conversa(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(ID); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COLON);  zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==Q) ) {
      questions(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==A) ) {
        answers(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==C) ) {
          conv(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x4);
  }
}

void
#ifdef __USE_PROTOS
questions(AST**_root)
#else
questions(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(Q);  zzCONSUME;
  llistaparaules(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ENDQ);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x8);
  }
}

void
#ifdef __USE_PROTOS
answers(AST**_root)
#else
answers(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(A);  zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (setwd1[LA(1)]&0x10) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        while ( (LA(1)==NUM) ) {
          formatNum(zzSTR); zzlink(_root, &_sibling, &_tail);
          zzLOOP(zztasp3);
        }
        zzEXIT(zztasp3);
        }
      }
    }
    else {
      if ( (LA(1)==CLAU1) ) {
        zzmatch(CLAU1);  zzCONSUME;
        {
          zzBLOCK(zztasp3);
          zzMake0;
          {
          while ( (LA(1)==PAR1) ) {
            formatList(zzSTR); zzlink(_root, &_sibling, &_tail);
            zzLOOP(zztasp3);
          }
          zzEXIT(zztasp3);
          }
        }
      }
      else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x20);
  }
}

void
#ifdef __USE_PROTOS
formatNum(AST**_root)
#else
formatNum(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(NUM); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COLON);  zzCONSUME;
  llistaparaules(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(SEMICOLON);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x40);
  }
}

void
#ifdef __USE_PROTOS
formatList(AST**_root)
#else
formatList(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(PAR1);  zzCONSUME;
  zzmatch(NUM); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COMMA);  zzCONSUME;
  llistaparaules(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(PAR2);  zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==COMMA) ) {
      zzmatch(COMMA);  zzCONSUME;
    }
    else {
      if ( (LA(1)==CLAU2) ) {
        zzmatch(CLAU2);  zzCONSUME;
      }
      else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x80);
  }
}

void
#ifdef __USE_PROTOS
conv(AST**_root)
#else
conv(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(C);  zzCONSUME;
  who(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(F); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  who(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x1);
  }
}

void
#ifdef __USE_PROTOS
who(AST**_root)
#else
who(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(HASHTG);  zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x2);
  }
}

void
#ifdef __USE_PROTOS
llistaparaules(AST**_root)
#else
llistaparaules(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    int zzcnt=1;
    zzMake0;
    {
    do {
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      zzLOOP(zztasp2);
    } while ( (LA(1)==ID) );
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x4);
  }
}

void
#ifdef __USE_PROTOS
chats(AST**_root)
#else
chats(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==CHATBOT) ) {
      chatbots(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x8);
  }
}

void
#ifdef __USE_PROTOS
chatbots(AST**_root)
#else
chatbots(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(CHATBOT);  zzCONSUME;
  zzmatch(ID); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COLON);  zzCONSUME;
  relation(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x10);
  }
}

void
#ifdef __USE_PROTOS
relation(AST**_root)
#else
relation(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  then(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==OR) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        {
          zzBLOCK(zztasp4);
          zzMake0;
          {
          zzmatch(OR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          then(zzSTR); zzlink(_root, &_sibling, &_tail);
          zzEXIT(zztasp4);
          }
        }
        {
          zzBLOCK(zztasp4);
          zzMake0;
          {
          while ( (LA(1)==OR) ) {
            zzmatch(OR);  zzCONSUME;
            then(zzSTR); zzlink(_root, &_sibling, &_tail);
            zzLOOP(zztasp4);
          }
          zzEXIT(zztasp4);
          }
        }
        zzEXIT(zztasp3);
        }
      }
    }
    else {
      if ( (setwd2[LA(1)]&0x20) ) {
      }
      else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x40);
  }
}

void
#ifdef __USE_PROTOS
then(AST**_root)
#else
then(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  identif(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==TH) ) {
      zzmatch(TH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      identif(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x80);
  }
}

void
#ifdef __USE_PROTOS
identif(AST**_root)
#else
identif(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==HASHTG) ) {
    {
      zzBLOCK(zztasp2);
      zzMake0;
      {
      zzmatch(HASHTG);  zzCONSUME;
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      zzEXIT(zztasp2);
      }
    }
  }
  else {
    if ( (LA(1)==PAR1) ) {
      {
        zzBLOCK(zztasp2);
        zzMake0;
        {
        zzmatch(PAR1);  zzCONSUME;
        relation(zzSTR); zzlink(_root, &_sibling, &_tail);
        zzmatch(PAR2);  zzCONSUME;
        zzEXIT(zztasp2);
        }
      }
    }
    else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x1);
  }
}

void
#ifdef __USE_PROTOS
startchat(AST**_root)
#else
startchat(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(I);  zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  {
    zzBLOCK(zztasp2);
    int zzcnt=1;
    zzMake0;
    {
    do {
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      zzLOOP(zztasp2);
    } while ( (LA(1)==ID) );
    zzEXIT(zztasp2);
    }
  }
  zzmatch(END); 
  (*_root)=createASTlist(_sibling);
 zzCONSUME;

  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x2);
  }
}
