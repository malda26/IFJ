//
// Created by Matej Dubec on 23.11.2019.
//

#include "stack.h"
#include <stdio.h>
#include <malloc.h>

/// 1)Funkcie pre prácu so zásobníkom:

/**
 * Inicializuje sa zásobník.
 * @param s
 */
void stackInit (stack *s) {
    s->top = NULL;
}

/**
 * Vlozi na vrchol polozku.
 * Ak malo pamate, chyba.
 * @param s
 * @param token
 * @return
 */
int stackPush (stack *s, t_token current_token) {

    TSItem *tmp;
    t_token *prom = malloc(sizeof(current_token));
    prom->data = current_token.data;
    prom->data_size = current_token.data_size;
    prom->type = current_token.type;

    if ((tmp = (TSItem *)malloc(sizeof(TSItem))) != NULL ) {

        tmp->token = prom;
        tmp->next = s->top;
        s->top = tmp;
        return OK;
    }
    return ERROR; // malo pamati (E_INTERNAL)
}


/**
 * Pop vrchnej polozky zasobnika.
 * Ak je stack prazdny, chyba.
 * @param s
 * @return
 */
int stackPop (stack *s) {

    if (isEmpty(s)) return -1; // zasobnik je prazdny
    TSItem *tmp = s->top;	   // zasobnik nieje prazdny, popnem vrchol
    s->top = s->top->next;
    free(tmp);
    return OK;
}


/**
 * Vracia ukazatel na data na vrchole.
 * NULL ak prazdny.
 * @param s
 * @return
 */
void *stackTop (stack *s) {
    return (isEmpty(s)) ? NULL : s->top->token;
}


/**
 * Vyberie a vrati ukazatel na data na vrchole.
 * @param s
 * @return
 */
void *stackTopPop (stack *s) {
    void *pom = stackTop(s);
    if ((pom == NULL) || (stackPop(s) != OK)) return NULL; // Stack top neexistuje / neuspesny pop
    return pom;
}


/**
 * Vyberie zo zasobniku vsetky polozky. (vyprazdni)
 * @param s
 */
void stackDelete (stack *s) {
    while (!isEmpty(s)) {
        stackPop(s);
    }
}


/// 2)Funkcie pre prácu so zásobníkom na sledovanie odsadenia:

/**
 * Inicializuje sa zásobník, zaroven aj pushne prvy, nulovy indent, ktory reprezentuje dno stacku.
 * @param s
 */
void indentStackInit (ind_stack *s) {
    TSIndItem *tmp = malloc(sizeof(TSIndItem));
    tmp->indent = 0;
    tmp->next = NULL;
    s->top = tmp;
}


/**
 * Vlozi na vrchol polozku.
 * Ak malo pamate, chyba.
 * @param s
 * @param token
 * @return
 */
int indentStackPush (ind_stack *s, int indentation) {
    TSIndItem *prom;
    if ((prom = malloc(sizeof(TSIndItem))) == NULL) return INTERNAL_ERROR;
    prom->indent = indentation;
    prom->next = s->top;
    s->top = prom;

}


/**
 * Pop vrchnej polozky zasobnika.
 * Ak je stack prazdny, chyba.
 * @param s
 * @return
 */
int indentStackPop (ind_stack *s) {
    if (s->top == NULL) return -1; // zasobnik je prazdny
    TSIndItem *tmp = s->top;	   // zasobnik nieje prazdny, popnem vrchol
    s->top = tmp->next;
    free(tmp);
    return OK;
}


/**
 * Vracia ukazatel na data na vrchole.
 * NULL ak prazdny.
 * @param s
 * @return
 */
void *indentStackTop (ind_stack *s) {
    return (s->top == NULL) ? NULL : s->top;
}


/**
 * Vyberie a vrati ukazatel na data na vrchole.
 * @param s
 * @return
 */
void *indentStackTopPop (ind_stack *s) {
    void *tmp = indentStackTop(s);
    if ((tmp == NULL) || (indentStackPop(s) != OK)) return NULL; // Stack top neexistuje / neuspesny pop
    return tmp;
}


/**
 * Vyberie zo zasobniku vsetky polozky. (vyprazdni)
 * @param s
 */
void indentStackDelete (ind_stack *s) {
    while (s->top != NULL) {
        indentStackPop(s);
    }
}

/// Funkcie pre prácu so zásobníkom:

/**
 * Inicializuje sa zásobník pre expression.
 * @param s
 */
void eStackInit (eStack *s) {
    s->top = NULL;
}


/**
 * Vytvara z parametrov eItem a pushuje do zasobnika
 * Ak malo pamate, chyba.
 * @param s
 * @param token
 * @return
 */

int eStackPush (eStack *s, t_token * current_token, PSA_table_index_enum index, PSA_symbol_enum tableSymbol, int isT) {

    eItem *pom;

    if ((pom = (eItem *) malloc(sizeof(eItem) + ((current_token->data_size) * sizeof(char)))) != NULL) {
        pom->is_terminal = isT;
        pom->action = E;
        pom->data = current_token->data;

        printf("%s\n\n", pom->data);

        pom->data_size = current_token->data_size;
        pom->table_symbol = tableSymbol;
        pom->index = index;
        pom->next = s->top;
        s->top = pom;
        return OK;
    }

    return INTERNAL_ERROR; // malo pamati (E_INTERNAL)
}

/**
 * Push polozky typu eItem do zasobnika.
 * Ak je stack prazdny, chyba.
 * @param s
 * @return
 */

int eItemStackPush (eStack *s, eItem *nextTop) {

    eItem *pom;

    if ((pom = (eItem *) malloc(sizeof(eItem) + ((nextTop->data_size) * sizeof(char)))) != NULL) {
        pom->is_terminal = nextTop->is_terminal;
        pom->action = E;
        pom->data = nextTop->data;
        pom->data_size = nextTop->data_size;
        pom->table_symbol = nextTop->table_symbol;
        pom->index = nextTop->index;

        printf("%s\n\n", pom->data);

        pom->next = s->top;
        s->top = pom;
        return OK;
    } else return INTERNAL_ERROR;
}


/**
 * Pop vrchnej polozky zasobnika.
 * Ak je stack prazdny, chyba.
 * @param s
 * @return
 */

int eStackPop (eStack *s) {

    if (isEmpty(s)) return ERROR; // zasobnik je prazdny

        eItem *pom;
        pom = s->top;
        s->top = s->top->next;
        free(pom);
        return OK;

}

/**
 * Vracia ukazatel na data na vrchole.
 * NULL ak prazdny.
 * @param s
 * @return
 */

void *eStackTop (eStack *s) {
    return (isEmpty(s)) ? NULL : s->top->data;
}

/**
 * Vracia ukazatel na eItem na vrchole.
 * NULL ak prazdny.
 * @param s
 * @return
 */

eItem *eStackTopItem (eStack *s) {
    return (s->top == NULL) ? NULL : s->top;
}

/**
 * Vyberie a vrati ukazatel na eItem na vrchole.
 * @param s
 * @return
 */
 ///NEPOUZIVAT!

eItem *eStackTopPop (eStack *s) {
    eItem *pom;
    pom = eStackTopItem(s);
    if ((pom == NULL) || (eStackPop(s) != OK)) return NULL; // Stack top neexistuje / neuspesny pop
    return pom;
}


/**
 * Vyberie zo zasobniku vsetky polozky. (vyprazdni)
 * @param s
 */
void eStackDelete (eStack *s) {
    while (!isEmpty(s)) {
        eStackPop(s);
    }
}