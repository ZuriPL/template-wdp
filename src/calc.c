/*
 *  'Liczby kolosalne'
 * 
 *  Author: Michał Bąk (479115)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct _cnum {
    struct _cnum* exp;
    struct _cnum* next;
};

typedef struct _cnum CNumber;

struct _stack {
    CNumber* v;
    struct _stack* under;

};

typedef struct _stack Stack;

// ----------------------
// Data Structure Methods
// ----------------------

Stack* init() {
    return NULL;
}

CNumber* createNumber(CNumber* exp, CNumber* next) {
    CNumber* num = (CNumber*) malloc(sizeof(CNumber));
    num->exp = exp;
    num->next = next;

    return num;
}

bool Empty(Stack* s) {
    return s == NULL;
}

void push(Stack** s, CNumber* v) {
    Stack* element = (Stack*) malloc(sizeof(Stack));
    element->v = v;
    element->under = *s;
    
    *s = element;
}

CNumber* pop(Stack** s) {
    Stack* popped = *s;
    CNumber* v = popped->v;

    *s = (*s)->under;

    free(popped);

    return v;
}

void freeCNumber(CNumber* num) {
    if (num == NULL) return;

    freeCNumber(num->exp);
    freeCNumber(num->next);

    free(num);
}

void destroy(Stack* s) {
    while (!Empty(s)) {
        freeCNumber(pop((&s)));
    }
}

// ----------------
// Calculator Logic
// ----------------

CNumber* readInput(char character) {
    if (character != '1') return NULL;

    CNumber* num = createNumber(NULL, NULL);
    num->exp = readInput((char) getchar());
    num->next = readInput((char) getchar());

    return num;
}

void _printHelper(CNumber* num) {
    while (num != NULL) {
        printf("1");
        _printHelper(num->exp);
        printf("0");

        num = num-> next;
    }
}

void printNumber(CNumber* num) {
    _printHelper(num);
    printf("0\n");
}

/*
    Create a copy of a colossal number in a new place in memory
*/
CNumber* copyNumber(CNumber* num){
    if (num == NULL) return NULL;

    return createNumber(
        copyNumber(num->exp), 
        copyNumber(num->next)
    );
}

/*
    Compare 2 colossal numbers. Returns 1 if a is bigger than b, 0 if they're equal and -1 if a is smaller than b
*/
int compare(CNumber* a, CNumber* b) {
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;

    int c = compare(a->exp, b->exp);
    // if there's a difference now, we found the result, if a->exp == b->exp we need to compare the next digits
    if (c != 0) return c;

    return compare(a->next, b->next);
}

/*
    Split the list of digits of a colossal number into two lists, and returns the pointer to the second half
*/
CNumber* split(CNumber* num) {
    CNumber* fast = num;
    CNumber* slow = num;

    while (fast != NULL && fast->next != NULL) {
        fast = fast->next->next;

        if (fast != NULL) {
            slow = slow->next;
        }
    }

    CNumber* temp = slow->next;
    slow->next = NULL;

    return temp;
}

/*
    Merge two colossal numbers into one
*/
CNumber* merge(CNumber* first, CNumber* second) {
    if (first == NULL) return second;
    if (second == NULL) return first;

    if (compare(first, second) == 1) {
        first->next = merge(first->next, second);
        return first;
    }
    else {
        second->next = merge(first, second->next);
        return second;
    }
}

CNumber* mergeSort(CNumber* num) {
    if (num == NULL || num->next == NULL) return num;

    CNumber* second = split(num);

    num = mergeSort(num);
    second = mergeSort(second);

    return merge(num, second);
}

/*
    Adds two numbers to each other. The two numbers are transformed into the result in place
*/
CNumber* add(CNumber* a, CNumber* b) {
    if (a == NULL) return b;
    if (b == NULL) return a;

    CNumber* current;

    if (compare(a->exp, b->exp) == 1) {
        current = a;
        a = a->next;
    } else {
        current = b;
        b = b->next;
    }
    
    CNumber* start = current;

    while (a != NULL || b != NULL) {
        if ((b == NULL) || (a != NULL && compare(a->exp, b->exp) == 1)) {
            // (a != NULL && b == NULL - end of list b) || (a > b)
            current->next = a;
            a = a->next;
        } else {
            // (a == NULL && b != NULL - end of list a) || (a < b)
            current->next = b;
            b = b->next;
        }

        current = current->next;
    }

    return start;
}

/*
    Multiplies two numbers. The function creates new nodes in memory for the resulting number and calls free() on the arguments
*/
CNumber* multiply(CNumber* a, CNumber* b) {
    CNumber* result = NULL;

    CNumber* digitA = a;
    
    while (digitA != NULL) {
        CNumber* digitB = b;

        while (digitB != NULL) {
            result = add(
                createNumber(add(
                    copyNumber(digitA->exp),
                    copyNumber(digitB->exp)
                ), NULL),
                result
            );
            
            digitB = digitB->next;
        }
        
        digitA = digitA->next;
    }

    freeCNumber(a);
    freeCNumber(b);

    return result;
}

CNumber* sumEqualDigits(CNumber* num) {
    if (num == NULL || num->next == NULL) return num;

    bool changed = true;

    while (changed) { 
        changed = false;

        CNumber* digit = num;

        while (digit != NULL && digit->next != NULL) {
            if (compare(digit->exp, digit->next->exp) == 0) {
                // if we find two equal digits, we can create a new digit which is equal to another one, 
                // therefore for every time we add together two digits we have to re-enter the loop
                changed = true; 

                // 2^(exp) + 2^(exp) == 2^(exp+1)

                // remove the repeated digit
                CNumber* temp = digit->next;
                digit->next = digit->next->next;
                temp->next = NULL;
                freeCNumber(temp);

                // add 1 to the exponent of the first digit
                digit->exp = sumEqualDigits(add(
                    digit->exp, 
                    createNumber(NULL, NULL) // colossal number with value of 1
                ));
            }

            digit = digit->next;
        }
    }

    return num;
}

CNumber* normalize(CNumber* num) {
    if (num == NULL) return num;
    
    CNumber* digit = num;
    
    while (digit != NULL) {
        digit->exp = normalize(digit->exp);
        digit = digit->next;
    }

    num = mergeSort(num);

    num = sumEqualDigits(num);

    return num;
}

int main() {
    Stack* s = init();
    
    while (true) {
        char character = (char) getchar();
        CNumber* num;

        switch (character)
        {
        case '.':
            num = normalize(pop(&s));
            printNumber(num);
            freeCNumber(num);
            break;
        case ':':
            push(&s, copyNumber(s->v));
            break;
        case '^':
            push(&s, createNumber(pop(&s), NULL));
            break;
        case '+':
            push(&s, add(pop(&s), pop(&s)));
            break;
        case '*':
            // multiplying on normalized numbers increases efficiency
            push(&s, multiply(
                normalize(pop(&s)), 
                normalize(pop(&s))
            ));
            break;
        case '0':
        case '1':
            push(&s, readInput(character));
            break;
        case EOF:
            destroy(s);
            exit(0);
            break;
        default:
            break;
        }
    }

    return 0;
}