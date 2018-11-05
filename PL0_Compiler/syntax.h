//
// Created by Tom on 2018/10/10.
//

#ifndef PL0_SYNTAX_H
#define PL0_SYNTAX_H

#include "lexical.h"

struct Sub_program;
struct Const_declaration;
struct Const_definition;
struct Var_declaration;
struct Procedure_declaration;
struct Procedure_head;
struct Statement;
struct Assign_statement;
struct Expression;
struct Term;
struct Factor;
struct Condition;
struct Condition_statement;
struct While_statement;
struct Procedure_call_statement;
struct Compound_statement;
struct Read_statement;
struct Write_statement;

typedef struct Sub_program Sub_program;
typedef struct Const_declaration Const_declaration;
typedef struct Const_definition Const_definition;
typedef struct Var_declaration Var_declaration;
typedef struct Procedure_declaration Procedure_declaration;
typedef struct Procedure_head Procedure_head;
typedef struct Statement Statement;
typedef struct Assign_statement Assign_statement;
typedef struct Expression Expression;
typedef struct Term Term;
typedef struct Factor Factor;
typedef struct Condition Condition;
typedef struct Condition_statement Condition_statement;
typedef struct While_statement While_statement;
typedef struct Procedure_call_statement Procedure_call_statement;
typedef struct Compound_statement Compound_statement;
typedef struct Read_statement Read_statement;
typedef struct Write_statement Write_statement;

enum STATEMENT_TYPE {ASSIGN_STATEMENT, CONDITION_STATEMENT,
    WHILE_STATEMENT, CALL_PROCEDURE_STATEMENT,
    COMPOUND_STATEMENT, READ_STATEMENT, WRITE_STATEMENT,
    EMPTY_STATEMENT} ;

enum FACTOR_TYPE {FACTOR_IDENTIFIER, FACTOR_NUMBER, FACTOR_EXPRESSION};

// linked list

typedef struct node_sub_expression{
    Symbol* plusOrMinus;
    Term* term;
    struct node_sub_expression * next;
} Sub_expression_linked_list;

typedef struct node_sub_term{
    Symbol* mulOrDiv;
    Factor* factor;
    struct node_sub_term * next;
} Sub_term_linked_list;

typedef struct node_const_declaration{
    Const_definition* const_definition;
    struct node_const_declaration * next;
}Const_definition_linked_list;


typedef struct node_symbol{
    Symbol* symbol;
    struct node_symbol * next;
}Symbol_linked_list;

typedef struct node_statement{
    Statement* statement;
    struct node_statement * next;
}Statement_linked_list;

typedef struct node_expression{
    Expression* expression;
    struct node_expression * next;
}Expression_linked_list;

// linked list end

struct Sub_program{
    Const_declaration* const_declaration;
    Var_declaration* var_declaration;
    Procedure_declaration* procedure_declaration;
    Statement* statement;
};

struct Const_declaration{
    Const_definition_linked_list* const_declaration_linked_list;
};

struct Const_definition{
    Symbol* identifier;
    Symbol* number;
};

struct Var_declaration{
    Symbol_linked_list* identifiers;
};

struct Procedure_declaration{
    Procedure_head* procedure_head;
    Sub_program* sub_program;
    Procedure_declaration* procedure_declaration;
};

struct Procedure_head{
    Symbol* identifier;
};

struct Statement{
    enum STATEMENT_TYPE  statement_type;
    union{
        Assign_statement* assign_statement;
        Condition_statement* condition_statement;
        While_statement* while_statement;
        Procedure_call_statement* call_procedure_statement;
        Compound_statement* compound_statement;
        Read_statement* read_statement;
        Write_statement* write_statement;
    } statement;
};

struct Assign_statement{
    Symbol* identifier;
    Expression* expression;
};

struct Expression{
    Symbol* plusOrMinusOrNull;
    Term* term;
    Sub_expression_linked_list* sub_expression_linked_list;
};

struct Term{
    Factor* factor;
    Sub_term_linked_list* sub_term_linked_list;
};

struct Factor{
    enum FACTOR_TYPE type;
    union{
        Symbol* identifier;
        Symbol* number;
        Expression* expression;
    } factor;
};

struct Condition{
    Expression* lExpression;
    Symbol* cmp;
    Expression* rExpression;
};

struct Condition_statement{
    Condition* condition;
    Statement* statement;
};

struct While_statement{
    Condition* condition;
    Statement* statement;
};

struct Procedure_call_statement{
    Symbol* identifier;
};

struct Compound_statement{
    Statement_linked_list* statement_linked_list;
};

struct Read_statement{
    Symbol_linked_list* symbol_linked_list;
};

struct Write_statement{
    Expression_linked_list* expression_linked_list;
};

Sub_program* getSubProgram();
Const_declaration* getConstDeclaration();
Const_definition* getConstDefinition();
Var_declaration* getVarDeclaration();
Procedure_declaration* getProcedureDeclaration();
Procedure_head* getProcedureHead();
Statement* getStatement();
Assign_statement* getAssignStatement();
Expression* getExpression();
Term* getTerm();
Factor* getFactor();
Condition* getCondition();
Condition_statement* getConditionStatement();
While_statement* getWhileStatement();
Procedure_call_statement* getProcedureCallStatement();
Compound_statement* getCompoundStatement();
Read_statement* getReadStatement();
Write_statement* getWriteStatement();

Symbol* getID();
Symbol* getNumber();
Symbol* getRelational();
Symbol* getPlus();
Symbol* getMul();
Symbol* copyAndFreeAndRead(Symbol *src);
Symbol* copyWithStringAndRead(Symbol *src);
#endif //PL0_SYNTAX_H

void syntax_log(const char *info);
void info_log(const char *info);