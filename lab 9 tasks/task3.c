#include <stdio.h>
#include <stddef.h>

#define DAYS 7
void       read_readings(int *data, int size);
int        read_non_negative(const char *prompt);
void       display_addresses(const int *data, int size);
int        weekly_total(const int *data, int size);
const int *find_peak_reading(const int *data, int size);
void       apply_peak_tariff(int *data, int size, int threshold);
void       print_readings(const char *title, const int *data, int size);
void       report_linked_meter(const char *label, const int *meter, const int *base);

int main(void)
{
    int readings[DAYS];            
    int *linked_meter = NULL;     
    const int *peak;
    int threshold;

    printf("=== Smart Meter Memory Inspector ===\n\n");

    read_readings(readings, DAYS);

    display_addresses(readings, DAYS);

    printf("\nWeekly total (original): %d units\n", weekly_total(readings, DAYS));
    peak = find_peak_reading(readings, DAYS);
    printf("Highest consumption: Day %d with %d units\n", (int)(peak - readings) + 1, *peak);

    printf("\n--- Linked meter check #1 (before assignment) ---\n");
    report_linked_meter("Linked meter", linked_meter, readings);

    linked_meter = readings + 2;
    printf("\n--- Linked meter check #2 (assigned to Day 3) ---\n");
    report_linked_meter("Linked meter", linked_meter, readings);

    threshold = read_non_negative("\nEnter peak-tariff threshold (units): ");
    apply_peak_tariff(readings, DAYS, threshold);
    print_readings("Readings after peak-tariff adjustment", readings, DAYS);
    printf("Weekly total (adjusted): %d units\n", weekly_total(readings, DAYS));
    peak = find_peak_reading(readings, DAYS);
    printf("Highest consumption (adjusted): Day %d with %d units\n", (int)(peak - readings) + 1, *peak);

    printf("\n--- Linked meter check #3 (after adjustment) ---\n");
    report_linked_meter("Linked meter", linked_meter, readings);

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
            printf("  Consumption cannot be negative. Try again.\n");
            continue;
        }
        return value;
    }
}

void read_readings(int *data, int size)
{
    int *p;

    for (p = data; p < data + size; p++) {
        printf("Enter units consumed on day %d: ", (int)(p - data) + 1);
        *p = read_non_negative("");
    }
}

void display_addresses(const int *data, int size)
{
    const int *p;

    printf("\nDay   Reading   Address\n");
    for (p = data; p < data + size; p++)
        printf("%3d   %7d   %p\n", (int)(p - data) + 1, *p, (const void *)p);

    printf("\nAddress gaps between consecutive days:\n");
    for (p = data; p < data + size - 1; p++) {
        ptrdiff_t bytes = (const char *)(p + 1) - (const char *)p;
        printf("  Day %d -> Day %d : %td bytes apart\n",
               (int)(p - data) + 1, (int)(p - data) + 2, bytes);
    }

    printf("\nWhy? Array elements sit back-to-back in memory, and each element is an\n"
           "int occupying sizeof(int) = %zu bytes here. So the next element's address is\n"
           "exactly %zu bytes after the previous one. Pointer arithmetic is scaled by the\n"
           "type: (p + 1) moves forward by ONE element (%zu bytes), not by one byte,\n"
           "which is why (p + 1) - p equals 1 element but %zu bytes when viewed as raw bytes.\n",
           sizeof(int), sizeof(int), sizeof(int), sizeof(int));
}

int weekly_total(const int *data, int size)
{
    const int *p;
    int sum = 0;

    for (p = data; p < data + size; p++)
        sum += *p;
    return sum;
}

const int *find_peak_reading(const int *data, int size)
{
    const int *p;
    const int *best = data;

    for (p = data + 1; p < data + size; p++)
        if (*p > *best)
            best = p;
    return best;
}

void apply_peak_tariff(int *data, int size, int threshold)
{
    int *p;

    for (p = data; p < data + size; p++)
        if (*p > threshold)
            *p = *p * 2;
}

void print_readings(const char *title, const int *data, int size)
{
    const int *p;

    printf("\n%s:\n", title);
    for (p = data; p < data + size; p++)
        printf("  Day %d: %d units\n", (int)(p - data) + 1, *p);
}

void report_linked_meter(const char *label, const int *meter, const int *base)
{
    if (meter == NULL) {
        printf("%s is not assigned to any reading yet (NULL). Not dereferencing it.\n", label);
        return;
    }
    printf("%s is assigned: it points to Day %d, address %p, value %d units.\n",
           label, (int)(meter - base) + 1, (const void *)meter, *meter);
}