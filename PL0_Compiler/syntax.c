//
// Created by Tom on 2018/10/10.
//
#include <stdlib.h>
#include "syntax.h"
#include "lexical.h"
#include "helper.h"

Sub_program *getSubProgram() {  //check begin set
    info_log("Entering sub program");
    Const_declaration *const_declaration = NULL;
    Var_declaration *var_declaration = NULL;
    Procedure_declaration *procedure_declaration = NULL;
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 0) {    //const
        const_declaration = getConstDeclaration();
    }
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 1) {    //var
        var_declaration = getVarDeclaration();
    }
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 4) {    //procedure
        procedure_declaration = getProcedureDeclaration();
    }
    Statement *statement = getStatement();
    Sub_program *subProgram = malloc(sizeof(Sub_program));
    subProgram->statement = statement;

    subProgram->const_declaration = const_declaration;
    subProgram->var_declaration = var_declaration;
    subProgram->procedure_declaration = procedure_declaration;
    syntax_log(__func__);
    info_log("Exiting sub program");
    return subProgram;
}

Const_declaration *getConstDeclaration() {
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 0) {    //const
        getNextSymbol();
        Const_declaration *const_declaration = malloc(sizeof(Const_declaration));
        const_declaration->const_declaration_linked_list = malloc(sizeof(Const_definition_linked_list));
        Const_definition_linked_list *current = const_declaration->const_declaration_linked_list;

        current->const_definition = getConstDefinition();
        current->next = NULL;
        while (currentSymbol.type == COMMA) { // ,
            getNextSymbol();
            current->next = malloc(sizeof(Const_definition_linked_list));
            current = current->next;
            current->const_definition = getConstDefinition();
            current->next = NULL;
        }
        if (currentSymbol.type == SEMI) {
            getNextSymbol();
            syntax_log(__func__);
            return const_declaration;
        }

    }
    return error("Const declaration");
}

Const_definition *getConstDefinition() {
    Symbol *identifier = getID();
    if (currentSymbol.type == CMP && currentSymbol.value->number_or_index == 4) { // =
        getNextSymbol();
        Symbol *number = getNumber();
        Const_definition *const_definition = malloc(sizeof(Const_definition));
        const_definition->identifier = identifier;
        const_definition->number = number;
        syntax_log(__func__);
        return const_definition;
    }

    return error("Const definition");
}

Var_declaration *getVarDeclaration() {
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 1) {    //var
        getNextSymbol();
        Var_declaration *var_declaration = malloc(sizeof(Var_declaration));
        var_declaration->identifiers = malloc(sizeof(Symbol_linked_list));
        Symbol_linked_list *current = var_declaration->identifiers;
        current->symbol = getID();
        current->next = NULL;

        while (currentSymbol.type == COMMA) { // ,
            getNextSymbol();
            current->next = malloc(sizeof(Symbol_linked_list));
            current = current->next;
            current->symbol = getID();
            current->next = NULL;
        }
        if (currentSymbol.type == SEMI) {
            getNextSymbol();
            syntax_log(__func__);
            return var_declaration;
        }

    }
    return error("Var declaration");
}

Procedure_declaration *getProcedureDeclaration() {
    Procedure_declaration *procedure_declaration = malloc(sizeof(Procedure_declaration));
    procedure_declaration->procedure_head = getProcedureHead();
    procedure_declaration->sub_program = getSubProgram();
    procedure_declaration->procedure_declaration = NULL;
    if (currentSymbol.type == SEMI) {
        getNextSymbol();
        if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 4) { //procedure
            getNextSymbol();
            procedure_declaration->procedure_declaration = getProcedureDeclaration();
        }
        syntax_log(__func__);
        return procedure_declaration;
    }
    return error("Procedure declaration");
}

Procedure_head *getProcedureHead() {
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 4) {    // procedure
        getNextSymbol();
        Procedure_head *procedure_head = malloc(sizeof(Procedure_head));
        procedure_head->identifier = getID();

        if(currentSymbol.type == SEMI){
            getNextSymbol();
            syntax_log(__func__);
            return procedure_head;
        }
    }
    syntax_log(__func__);
    return error("Procedure head");
}

Statement *getStatement() { //check begin set
    Statement *statement = malloc(sizeof(Statement));
    if (currentSymbol.type == ID) {
        statement->statement_type = ASSIGN_STATEMENT;
        statement->statement.assign_statement = getAssignStatement();
    } else if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 5) {//if
        statement->statement_type = CONDITION_STATEMENT;
        statement->statement.condition_statement = getConditionStatement();
    } else if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 7) {//while
        statement->statement_type = WHILE_STATEMENT;
        statement->statement.while_statement = getWhileStatement();
    } else if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 9) {//call
        statement->statement_type = CALL_PROCEDURE_STATEMENT;
        statement->statement.call_procedure_statement = getProcedureCallStatement();
    } else if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 2) {//begin
        statement->statement_type = COMPOUND_STATEMENT;
        statement->statement.compound_statement = getCompoundStatement();
    } else if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 10) {//read
        statement->statement_type = READ_STATEMENT;
        statement->statement.read_statement = getReadStatement();
    } else if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 11) {//write
        statement->statement_type = WRITE_STATEMENT;
        statement->statement.write_statement = getWriteStatement();
    } else {
        statement->statement_type = EMPTY_STATEMENT;
    }
    syntax_log(__func__);
    return statement;
}

Assign_statement *getAssignStatement() {
    Symbol *identifier = getID();
    if (currentSymbol.type == ASSIGN) {
        getNextSymbol();
        Expression *expression = getExpression();
        Assign_statement *assign_statement = malloc(sizeof(Assign_statement));
        assign_statement->identifier = identifier;
        assign_statement->expression = expression;
        syntax_log(__func__);
        return assign_statement;
    }
    return error("Assign statement");
}

Expression *getExpression(){
    Expression* expression = malloc(sizeof(Expression));
    expression->plusOrMinusOrNull = NULL;
    if(currentSymbol.type == PLUS_OR_MINUS){
        expression->plusOrMinusOrNull = getPlus();
    }
    expression->term = getTerm();
    if(currentSymbol.type == PLUS_OR_MINUS){
        expression->sub_expression_linked_list = malloc(sizeof(Sub_expression_linked_list));
        Sub_expression_linked_list* current = expression->sub_expression_linked_list;
        current->plusOrMinus = getPlus();
        current->term = getTerm();
        current->next = NULL;
        while(currentSymbol.type == PLUS_OR_MINUS){
            current->next = malloc(sizeof(Sub_expression_linked_list));
            current = current->next;
            current->plusOrMinus = getPlus();
            current->term = getTerm();
            current->next = NULL;
        }
    }
    syntax_log(__func__);
    return expression;
}

Term *getTerm(){
    Term* term = malloc(sizeof(Term));
    term->factor = getFactor();
    term->sub_term_linked_list = NULL;
    if(currentSymbol.type == MUL_OR_DIV){
        term->sub_term_linked_list = malloc(sizeof(Sub_term_linked_list));
        Sub_term_linked_list* current = term->sub_term_linked_list;
        current->mulOrDiv = getMul();
        current->factor = getFactor();
        current->next = NULL;
        while(currentSymbol.type == MUL_OR_DIV){
            current->next = malloc(sizeof(Sub_term_linked_list));
            current = current->next;
            current->mulOrDiv = getMul();
            current->factor = getFactor();
            current->next = NULL;
        }
    }
    syntax_log(__func__);
    return term;
}

Factor *getFactor() {   //check begin set
    Factor *factor = malloc(sizeof(Factor));
    if (currentSymbol.type == ID) {
        factor->type = FACTOR_IDENTIFIER;
        factor->factor.identifier = getID();
    } else if (currentSymbol.type == NUMBER) {
        factor->type = FACTOR_NUMBER;
        factor->factor.number = getNumber();
    } else if (currentSymbol.type == LPAREN) {
        getNextSymbol();
        factor->type = FACTOR_EXPRESSION;
        factor->factor.expression = getExpression();
        if (currentSymbol.type == RPAREN) {
            getNextSymbol();
        }
    }else{
        error("factor");
    }
    syntax_log(__func__);
    return factor;
}

Condition *getCondition() {
    Expression *lExpression = getExpression();
    Symbol *relation = getRelational();
    Expression *rExpression = getExpression();
    Condition *condition = malloc(sizeof(Condition));
    condition->lExpression = lExpression;
    condition->cmp = relation;
    condition->rExpression = rExpression;
    syntax_log(__func__);
    return condition;
}

Condition_statement *getConditionStatement() {
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 5) {    //if
        getNextSymbol();
        Condition *condition = getCondition();
        if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 6) {    //then
            getNextSymbol();
            Statement *statement = getStatement();
            Condition_statement *condition_statement = malloc(sizeof(Condition_statement));
            condition_statement->condition = condition;
            condition_statement->statement = statement;
            syntax_log(__func__);
            return condition_statement;
        }
    }
    return error("condition statement");
}

While_statement *getWhileStatement() {
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 7) {    //while
        getNextSymbol();
        Condition *condition = getCondition();
        if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 8) {    //do
            getNextSymbol();
            Statement *statement = getStatement();
            While_statement *while_statement = malloc(sizeof(Condition_statement));
            while_statement->condition = condition;
            while_statement->statement = statement;
            syntax_log(__func__);
            return while_statement;
        }
    }
    return error("while statement");
}

Procedure_call_statement *getProcedureCallStatement() {
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 9) {    // call
        getNextSymbol();
        Symbol *identifier = getID();
        Procedure_call_statement *procedure_call_statement = malloc(sizeof(Procedure_call_statement));
        procedure_call_statement->identifier = identifier;
        syntax_log(__func__);
        return procedure_call_statement;
    }
    return error("procedure call statement");
}

Compound_statement *getCompoundStatement() {
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 2) {   //begin
        getNextSymbol();
        Compound_statement *compound_statement = malloc(sizeof(Compound_statement));
        compound_statement->statement_linked_list = malloc(sizeof(Statement_linked_list));
        Statement_linked_list *current = compound_statement->statement_linked_list;
        current->statement = getStatement();
        current->next = NULL;
        while (currentSymbol.type == SEMI) {
            getNextSymbol();
            current->next = malloc(sizeof(Statement_linked_list));
            current = current->next;
            current->statement = getStatement();
            current->next = NULL;
        }
        if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 3) {
            getNextSymbol();
            syntax_log(__func__);
            return compound_statement;
        }
    }
    return error("compound statement");
}

Read_statement *getReadStatement() {

    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 10) {   //read
        getNextSymbol();
        if (currentSymbol.type == LPAREN) {
            getNextSymbol();
            Read_statement *read_statement = malloc(sizeof(Read_statement));
            read_statement->symbol_linked_list = malloc(sizeof(Symbol_linked_list));
            Symbol_linked_list *current = read_statement->symbol_linked_list;
            current->symbol = getID();
            current->next = NULL;
            while (currentSymbol.type == COMMA) {
                getNextSymbol();
                current->next = malloc(sizeof(Symbol_linked_list));
                current = current->next;
                current->symbol = getID();
                current->next = NULL;
            }
            if (currentSymbol.type == RPAREN) {
                getNextSymbol();
                syntax_log(__func__);
                return read_statement;
            }
        }
    }
    return error("read statement");
}

Write_statement *getWriteStatement() {
    if (currentSymbol.type == RESERVED && currentSymbol.value->number_or_index == 11) {   //write
        getNextSymbol();
        if (currentSymbol.type == LPAREN) {
            getNextSymbol();
            Write_statement *write_statement = malloc(sizeof(Write_statement));
            write_statement->expression_linked_list = malloc(sizeof(Expression_linked_list));
            Expression_linked_list *current = write_statement->expression_linked_list;
            current->expression = getExpression();
            current->next = NULL;
            while (currentSymbol.type == COMMA) {
                getNextSymbol();
                current->next = malloc(sizeof(Expression_linked_list));
                current = current->next;
                current->expression = getExpression();
                current->next = NULL;
            }
            if (currentSymbol.type == RPAREN) {
                getNextSymbol();
                syntax_log(__func__);
                return write_statement;
            }
        }
    }
    return error("write statement");
}

Symbol *getID() {
    if (currentSymbol.type == ID) {
        syntax_log(__func__);
        return copyAndFreeAndRead(&currentSymbol);
    }
    return error("id");
}

Symbol *getNumber() {
    if (currentSymbol.type == NUMBER) {
        syntax_log(__func__);
        return copyAndFreeAndRead(&currentSymbol);
    }
    return error("number");
}

Symbol *getRelational() {
    if (currentSymbol.type == CMP) {
        syntax_log(__func__);
        return copyAndFreeAndRead(&currentSymbol);
    }
    return error("relational");
}

Symbol *getPlus() {
    if (currentSymbol.type == PLUS_OR_MINUS) {
        syntax_log(__func__);
        return copyAndFreeAndRead(&currentSymbol);
    }
    return error("plus");
}

Symbol *getMul() {
    if (currentSymbol.type == MUL_OR_DIV) {
        syntax_log(__func__);
        return copyAndFreeAndRead(&currentSymbol);
    }
    return error("mul");
}

Symbol *copyAndFreeAndRead(Symbol *src) {
    Symbol *symbol = (Symbol*)malloc(sizeof(Symbol));
    symbol->type = src->type;
    symbol->value = malloc(sizeof(Value));
    if (src->type == ID) {
        symbol->value->identifier = (char*) malloc(sizeof(char) * (1 + strlen(currentSymbol.value->identifier)));
        my_strcpy(symbol->value->identifier, src->value->identifier );
    } else symbol->value->number_or_index = src->value->number_or_index;
    // free(src);
    getNextSymbol();
    return symbol;
}

void syntax_log(const char *info){
    printf("%s\n", info);
}

void info_log(const char *info){
    // printf("%s\n", info);
}