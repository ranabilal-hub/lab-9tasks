#include <stdio.h>

#define NUM_STUDENTS 10   
#define MIN_MARK 0
#define MAX_MARK 100
int    total_marks(const int marks[], int size);
double average_marks(const int marks[], int size);
int    highest_mark(const int marks[], int size);
int    lowest_mark(const int marks[], int size);
int    count_passed(const int marks[], int size, int threshold);
int    count_top_scorers(const int marks[], int size);
void   moderate_marks(int marks[], int size, int bonus);
void   print_report(const char *title, const int marks[], int size, int threshold);
int    read_int_in_range(const char *prompt, int low, int high);

int main(void)
{
    int marks[NUM_STUDENTS];
    int n = NUM_STUDENTS;
    int i, threshold, bonus;
    char prompt[64];

    printf("=== Examination Cell Marks Analyzer ===\n\n");

    for (i = 0; i < n; i++) {
        sprintf(prompt, "Enter marks of student %d (0-100): ", i + 1);
        marks[i] = read_int_in_range(prompt, MIN_MARK, MAX_MARK);
    }

    threshold = read_int_in_range("\nEnter pass threshold (0-100): ", MIN_MARK, MAX_MARK);

    print_report("ORIGINAL REPORT", marks, n, threshold);

    bonus = read_int_in_range("\nEnter moderation bonus (0-100): ", 0, MAX_MARK);
    moderate_marks(marks, n, bonus);

    print_report("MODERATED REPORT", marks, n, threshold);

    return 0;
}

int read_int_in_range(const char *prompt, int low, int high)
{
    int value;

    for (;;) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            printf("  Invalid input. Please enter a whole number.\n");
            while (getchar() != '\n') { }
            continue;
        }
        if (value < low || value > high) {
            printf("  Value must be between %d and %d. Try again.\n", low, high);
            continue;
        }
        return value;
    }
}

int total_marks(const int marks[], int size)
{
    int i, sum = 0;
    for (i = 0; i < size; i++)
        sum += marks[i];
    return sum;
}

double average_marks(const int marks[], int size)
{
    if (size <= 0)
        return 0.0;
    return (double)total_marks(marks, size) / size;
}

int highest_mark(const int marks[], int size)
{
    int i, best;
    if (size <= 0)
        return 0;
    best = marks[0];
    for (i = 1; i < size; i++)
        if (marks[i] > best)
            best = marks[i];
    return best;
}

int lowest_mark(const int marks[], int size)
{
    int i, worst;
    if (size <= 0)
        return 0;
    worst = marks[0];
    for (i = 1; i < size; i++)
        if (marks[i] < worst)
            worst = marks[i];
    return worst;
}

int count_passed(const int marks[], int size, int threshold)
{
    int i, count = 0;
    for (i = 0; i < size; i++)
        if (marks[i] >= threshold)
            count++;
    return count;
}
int count_top_scorers(const int marks[], int size)
{
    int i, count = 0;
    int top = highest_mark(marks, size);

    if (size <= 0)
        return 0;
    for (i = 0; i < size; i++)
        if (marks[i] == top)
            count++;
    return count;
}

void moderate_marks(int marks[], int size, int bonus)
{
    int i;
    for (i = 0; i < size; i++) {
        marks[i] += bonus;
        if (marks[i] > MAX_MARK)
            marks[i] = MAX_MARK;
    }
}

void print_report(const char *title, const int marks[], int size, int threshold)
{
    int i;

    printf("\n========== %s ==========\n", title);
    printf("Marks: ");
    for (i = 0; i < size; i++)
        printf("%d ", marks[i]);
    printf("\n");
    printf("Class total          : %d\n", total_marks(marks, size));
    printf("Class average        : %.2f\n", average_marks(marks, size));
    printf("Highest mark         : %d\n", highest_mark(marks, size));
    printf("Lowest mark          : %d\n", lowest_mark(marks, size));
    printf("Passed (>= %d)       : %d student(s)\n", threshold, count_passed(marks, size, threshold));
    printf("Students with top mark: %d\n", count_top_scorers(marks, size));
    printf("=====================================\n");
}