#include <stdio.h>

#define DAYS 7

int  read_non_negative(const char *prompt);
void swap_by_address(int *a, int *b);           
void swap_by_value(int a, int b);                     
void summarise_week(const int *data, int size,
                    int *total, int *highest, int *lowest);

int main(void)
{
    int ward_a, ward_b;           
    int test_x = 11, test_y = 22; 
    int daily[DAYS];
    int total, highest, lowest;
    int i;

    printf("=== Hospital Ward Repair Utility ===\n\n");

    printf("Enter the values as they were WRONGLY recorded.\n");
    ward_a = read_non_negative("Ward A counter (currently holds B's count): ");
    ward_b = read_non_negative("Ward B counter (currently holds A's count): ");

    printf("\n[main] Addresses: &ward_a = %p, &ward_b = %p\n", (void *)&ward_a, (void *)&ward_b);
    printf("[main] BEFORE repair: ward_a = %d, ward_b = %d\n", ward_a, ward_b);
    swap_by_address(&ward_a, &ward_b);
    printf("[main] AFTER  repair: ward_a = %d, ward_b = %d  --> original variables changed\n",
           ward_a, ward_b);

    printf("\n--- Junior developer's attempt (ordinary parameters) on test_x / test_y ---\n");
    printf("[main] Addresses: &test_x = %p, &test_y = %p\n", (void *)&test_x, (void *)&test_y);
    printf("[main] BEFORE: test_x = %d, test_y = %d\n", test_x, test_y);
    swap_by_value(test_x, test_y);
    printf("[main] AFTER : test_x = %d, test_y = %d  --> UNCHANGED, the job was not done\n",
           test_x, test_y);

    printf("\n--- Weekly occupancy summary ---\n");
    for (i = 0; i < DAYS; i++) {
        printf("Day %d: ", i + 1);
        daily[i] = read_non_negative("occupancy: ");
    }

    printf("\n[main] Addresses: daily = %p, &total = %p, &highest = %p, &lowest = %p\n",
           (void *)daily, (void *)&total, (void *)&highest, (void *)&lowest);
    summarise_week(daily, DAYS, &total, &highest, &lowest);
    printf("[main] Total = %d, Highest = %d, Lowest = %d\n", total, highest, lowest);

    return 0;
}

int read_non_negative(const char *prompt)
{
    int value;

    for (;;) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            printf("  Invalid input. Please enter a whole number.\n");
            while (getchar() != '\n') { }
            continue;
        }
        if (value < 0) {
            printf("  A count cannot be negative. Try again.\n");
            continue;
        }
        return value;
    }
}

void swap_by_address(int *a, int *b)
{
    int temp;

    printf("  [swap_by_address] Received addresses: a = %p, b = %p (same as main's variables)\n",
           (void *)a, (void *)b);
    temp = *a;
    *a = *b;
    *b = temp;
    printf("  [swap_by_address] Swapped through the pointers: *a = %d, *b = %d\n", *a, *b);
}

void swap_by_value(int a, int b)
{
    int temp;

    printf("  [swap_by_value] Addresses of its own parameters: &a = %p, &b = %p (different from main's)\n",
           (void *)&a, (void *)&b);
    temp = a;
    a = b;
    b = temp;
    printf("  [swap_by_value] Swapped the copies inside: a = %d, b = %d\n", a, b);
}

void summarise_week(const int *data, int size, int *total, int *highest, int *lowest)
{
    int i;

    printf("  [summarise_week] Array address received = %p\n", (const void *)data);
    printf("  [summarise_week] Output addresses: total = %p, highest = %p, lowest = %p\n",
           (void *)total, (void *)highest, (void *)lowest);

    if (size <= 0) {
        *total = 0;
        *highest = 0;
        *lowest = 0;
        return;
    }

    *total = 0;
    *highest = data[0];
    *lowest = data[0];
    for (i = 0; i < size; i++) {
        *total += data[i];
        if (data[i] > *highest)
            *highest = data[i];
        if (data[i] < *lowest)
            *lowest = data[i];
    }
}