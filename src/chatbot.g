#header
<<
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
>>

<<
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
	cin \>\> ans;
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
	cin \>\> name;
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

>>

#lexclass START
#token A "ANSWERS"
#token Q "QUESTION"
#token C "CONVERSATION"
#token CHATBOT "CHATBOT"
#token I "INTERACTION"
#token TH "THEN"
#token OR "OR"
#token F "\->"
#token CLAU1 "\["
#token CLAU2 "\]"
#token PAR1 "\("
#token PAR2 "\)"
#token HASHTG "\#"
#token SEMICOLON "\;"
#token COLON "\:"
#token COMMA "\,"
#token ENDQ "?"
#token END "END"
#token NUM "[0-9]+"
#token ID "[a-zA-Z][a-zA-Z0-9]*"
#token SPACE "[\ \n]" << zzskip();>>


chatbot: conversations chats startchat<<#0=createASTlist(_sibling);>>;

conversations: (conversa)*  <<#0=createASTlist(_sibling);>>;

conversa: ID^ COLON! (questions | answers | conv);

questions: Q! llistaparaules ENDQ!;

answers: A! ( (formatNum)* | CLAU1! (formatList)* ) <<#0=createASTlist(_sibling);>>;

formatNum: NUM^ COLON! llistaparaules SEMICOLON!;

formatList: PAR1! NUM^ COMMA! llistaparaules PAR2! (COMMA! | CLAU2!);

conv: C! who F^ who;

who: HASHTG! ID;

llistaparaules: (ID)+ <<#0=createASTlist(_sibling);>>; 

chats: (chatbots)* <<#0=createASTlist(_sibling);>>;

chatbots: CHATBOT! ID^ COLON! relation;

relation: then (((OR^ then)(OR! then)*)| );

then: identif (TH^ identif)*;

identif: (HASHTG! ID) | (PAR1! relation PAR2!);

startchat: I! NUM (ID)+ END! <<#0=createASTlist(_sibling);>>;