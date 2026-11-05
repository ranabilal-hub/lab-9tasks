#include <stdio.h>

#define H1_ROWS 3
#define H1_COLS 4
#define H2_ROWS 3

#define VACANT 0
#define BOOKED 1

#define BOOK_OK            0
#define BOOK_BAD_ROW       1
#define BOOK_BAD_SEAT      2
#define BOOK_ALREADY_TAKEN 3

void print_hall1(int (*hall)[H1_COLS], int rows);
int  count_vacant_hall1(int (*hall)[H1_COLS], int rows);
void print_hall2(int *const *hall, const int *lengths, int rows);
int  count_vacant_hall2(int *const *hall, const int *lengths, int rows);
int  book_seat_hall2(int *const *hall, const int *lengths, int rows, int row, int seat);
void print_booking_result(int code, int row, int seat);
void show_hall1(int (*hall)[H1_COLS], int rows);
void show_hall2(int *const *hall, const int *lengths, int rows);
int  read_int(const char *prompt, int *value);

int main(void)
{
    int hall1[H1_ROWS][H1_COLS] = {
        {1, 0, 0, 1},
        {0, 0, 1, 0},
        {1, 1, 0, 0}
    };

    int front[3]  = {0, 1, 0};
    int middle[5] = {0, 0, 1, 0, 0};
    int back[7]   = {1, 0, 0, 0, 1, 0, 0};

    int *hall2[H2_ROWS] = {front, middle, back};
    int hall2_len[H2_ROWS] = {
        (int)(sizeof front  / sizeof front[0]),
        (int)(sizeof middle / sizeof middle[0]),
        (int)(sizeof back   / sizeof back[0])
    };

    int row, seat, code;

    printf("=== Cinema Seating and Timetable Manager ===\n");

    show_hall1(hall1, H1_ROWS);
    show_hall2(hall2, hall2_len, H2_ROWS);

    printf("\n--- Book a seat in Hall 2 (enter row 0 to finish) ---\n");
    for (;;) {
        if (!read_int("Row number: ", &row)) {
            printf("  Invalid input: please enter whole numbers only.\n");
            continue;
        }
        if (row == 0)
            break;
        if (!read_int("Seat number: ", &seat)) {
            printf("  Invalid input: please enter whole numbers only.\n");
            continue;
        }

        code = book_seat_hall2(hall2, hall2_len, H2_ROWS, row, seat);
        print_booking_result(code, row, seat);

        if (code == BOOK_OK)
            show_hall2(hall2, hall2_len, H2_ROWS);
    }

    printf("\nFinal state:\n");
    show_hall1(hall1, H1_ROWS);
    show_hall2(hall2, hall2_len, H2_ROWS);
    return 0;
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

void print_hall1(int (*hall)[H1_COLS], int rows)
{
    int (*row)[H1_COLS];  
    int *seat;

    for (row = hall; row < hall + rows; row++) {
        printf("  Row %d: ", (int)(row - hall) + 1);
        for (seat = *row; seat < *row + H1_COLS; seat++)
            printf("%s", *seat == BOOKED ? "[X] " : "[ ] ");
        printf("\n");
    }
}

int count_vacant_hall1(int (*hall)[H1_COLS], int rows)
{
    int (*row)[H1_COLS];
    int *seat;
    int vacant = 0;

    for (row = hall; row < hall + rows; row++)
        for (seat = *row; seat < *row + H1_COLS; seat++)
            if (*seat == VACANT)
                vacant++;
    return vacant;
}

void print_hall2(int *const *hall, const int *lengths, int rows)
{
    int r, s;

    for (r = 0; r < rows; r++) {
        printf("  Row %d (%d seats): ", r + 1, lengths[r]);
        for (s = 0; s < lengths[r]; s++)
            printf("%s", *(*(hall + r) + s) == BOOKED ? "[X] " : "[ ] ");
        printf("\n");
    }
}

int count_vacant_hall2(int *const *hall, const int *lengths, int rows)
{
    int r, s, vacant = 0;

    for (r = 0; r < rows; r++)
        for (s = 0; s < lengths[r]; s++)
            if (*(*(hall + r) + s) == VACANT)
                vacant++;
    return vacant;
}

int book_seat_hall2(int *const *hall, const int *lengths, int rows, int row, int seat)
{
    int *target;

    if (row < 1 || row > rows)
        return BOOK_BAD_ROW;
    if (seat < 1 || seat > lengths[row - 1])
        return BOOK_BAD_SEAT;

    target = *(hall + (row - 1)) + (seat - 1);
    if (*target == BOOKED)
        return BOOK_ALREADY_TAKEN;

    *target = BOOKED;
    return BOOK_OK;
}

void print_booking_result(int code, int row, int seat)
{
    switch (code) {
    case BOOK_OK:
        printf("  Success: row %d, seat %d is now booked.\n", row, seat);
        break;
    case BOOK_BAD_ROW:
        printf("  Rejected: row %d does not exist in Hall 2 (rows are 1 to %d).\n", row, H2_ROWS);
        break;
    case BOOK_BAD_SEAT:
        printf("  Rejected: seat %d does not exist in row %d.\n", seat, row);
        break;
    case BOOK_ALREADY_TAKEN:
        printf("  Rejected: row %d, seat %d is already booked.\n", row, seat);
        break;
    default:
        printf("  Rejected: unknown error.\n");
    }
}

void show_hall1(int (*hall)[H1_COLS], int rows)
{
    printf("\nHall 1 seating chart ([X] booked, [ ] vacant):\n");
    print_hall1(hall, rows);
    printf("  Vacant seats in Hall 1: %d\n", count_vacant_hall1(hall, rows));
}

void show_hall2(int *const *hall, const int *lengths, int rows)
{
    printf("\nHall 2 seating chart ([X] booked, [ ] vacant):\n");
    print_hall2(hall, lengths, rows);
    printf("  Vacant seats in Hall 2: %d\n", count_vacant_hall2(hall, lengths, rows));
}