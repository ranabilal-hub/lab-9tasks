#include <stdio.h>

#define STATUS_OK    0
#define STATUS_ERROR 1


typedef int (*Operation)(int a, int b, int *result);


typedef struct {
    const char *name;
    char        symbol;
    Operation   run;
} MenuEntry;


void print_value(const void *data, char type);
void test_generic_printer(void);
int  op_add(int a, int b, int *result);
int  op_subtract(int a, int b, int *result);
int  op_multiply(int a, int b, int *result);
int  op_divide(int a, int b, int *result);
int  perform_operation(Operation op, int a, int b, int *result);
int  read_int(const char *prompt, int *value);
void print_menu(const MenuEntry *menu, int count);

int main(void)
{
    
    const MenuEntry menu[] = {
        {"Add",      '+', op_add},
        {"Subtract", '-', op_subtract},
        {"Multiply", '*', op_multiply},
        {"Divide",   '/', op_divide}
    };
    const int count = (int)(sizeof menu / sizeof menu[0]);

    int a, b, choice, result, status;

    printf("=== Smart Calculator ===\n");
    test_generic_printer();

    for (;;) {
        print_menu(menu, count);

        if (!read_int("Your choice: ", &choice)) {
            printf("  Invalid input: please enter a number.\n");
            continue;
        }
        if (choice == 0) {
            printf("Goodbye!\n");
            break;
        }
        if (choice < 1 || choice > count) {      
            
            printf("  Invalid choice: please pick a number from 0 to %d.\n", count);
            continue;
        }
        if (!read_int("Enter first number : ", &a) ||
            !read_int("Enter second number: ", &b)) {
            printf("  Invalid input: whole numbers only.\n");
            continue;
        }

        
        status = perform_operation(menu[choice - 1].run, a, b, &result);

        if (status != STATUS_OK) {
            printf("  Error: %s of %d and %d is not possible (division by zero).\n",
                   menu[choice - 1].name, a, b);
            continue;
        }

        printf("  %d %c %d = ", a, menu[choice - 1].symbol, b);
        print_value(&result, 'i');
        printf("\n");
    }

    return 0;
}


void print_value(const void *data, char type)
{
    switch (type) {
    case 'i':
        printf("%d", *(const int *)data);
        break;
    case 'f':
        printf("%.2f", *(const float *)data);
        break;
    case 'c':
        printf("%c", *(const char *)data);
        break;
    default:
        printf("<unknown type '%c'>", type);
    }
}

void test_generic_printer(void)
{
    int   whole   = 42;
    float decimal = 3.14159f;
    char  letter  = 'A';

    printf("\nGeneric printer test:\n  int   : ");
    print_value(&whole, 'i');
    printf("\n  float : ");
    print_value(&decimal, 'f');
    printf("\n  char  : ");
    print_value(&letter, 'c');
    printf("\n");
}

int op_add(int a, int b, int *result)
{
    *result = a + b;
    return STATUS_OK;
}

int op_subtract(int a, int b, int *result)
{
    *result = a - b;
    return STATUS_OK;
}

int op_multiply(int a, int b, int *result)
{
    *result = a * b;
    return STATUS_OK;
}


int op_divide(int a, int b, int *result)
{
    if (b == 0)
        return STATUS_ERROR;
    *result = a / b;
    return STATUS_OK;
}


int perform_operation(Operation op, int a, int b, int *result)
{
    if (op == NULL)
        return STATUS_ERROR;
    return op(a, b, result);
}

int read_int(const char *prompt, int *value)
{
    printf("%s", prompt);
    if (scanf("%d", value) != 1) {
        while (getchar() != '\n') { }
        return 0;
    }
    return 1;
}

void print_menu(const MenuEntry *menu, int count)
{
    int i;

    printf("\nMenu:\n");
    for (i = 0; i < count; i++)
        printf("  %d. %s (%c)\n", i + 1, menu[i].name, menu[i].symbol);
    printf("  0. Exit\n");
}