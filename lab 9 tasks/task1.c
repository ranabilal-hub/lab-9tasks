#include <stdio.h>

#define DISCOUNT_THRESHOLD 1000.0
#define DISCOUNT_RATE      0.10

double round_to_paisa(double amount);
int    is_valid_price(double price);
int    is_valid_quantity(int quantity);
double read_price(int item_number);
int    read_quantity(int item_number);
double calculate_line_total(double price, int quantity);
double calculate_subtotal(double line1, double line2);
double calculate_discount(double subtotal);
double calculate_net_total(double subtotal, double discount);
int    higher_priced_item(double price1, double price2);
void   print_item_line(int item_number, double price, int quantity, double line_total);
void   print_comparison(double price1, double price2);
void   print_receipt(double price1, int qty1, double price2, int qty2);

int main(void)
{
    double price1, price2;
    int qty1, qty2;

    printf("=== FAST Karachi Campus Canteen ===\n\n");

    price1 = read_price(1);
    qty1   = read_quantity(1);
    price2 = read_price(2);
    qty2   = read_quantity(2);

    print_receipt(price1, qty1, price2, qty2);
    return 0;
}

double round_to_paisa(double amount)
{
    return (double)(long)(amount * 100.0 + 0.5) / 100.0;
}

int is_valid_price(double price)
{
    return price >= 0.0;
}

int is_valid_quantity(int quantity)
{
    return quantity >= 0;
}

double read_price(int item_number)
{
    double price;
    int ok;

    for (;;) {
        printf("Enter unit price of item %d (Rs.): ", item_number);
        ok = scanf("%lf", &price);
        if (ok != 1) {
            printf("  Invalid input. Please enter a number.\n");
            while (getchar() != '\n') { }   
            continue;
        }
        if (!is_valid_price(price)) {
            printf("  Price cannot be negative. Try again.\n");
            continue;
        }
        return price;
    }
}
int read_quantity(int item_number)
{
    int quantity, ok;

    for (;;) {
        printf("Enter quantity of item %d: ", item_number);
        ok = scanf("%d", &quantity);
        if (ok != 1) {
            printf("  Invalid input. Please enter a whole number.\n");
            while (getchar() != '\n') { }
            continue;
        }
        if (!is_valid_quantity(quantity)) {
            printf("  Quantity cannot be negative. Try again.\n");
            continue;
        }
        return quantity;
    }
}

double calculate_line_total(double price, int quantity)
{
    return round_to_paisa(price * quantity);
}

double calculate_subtotal(double line1, double line2)
{
    return round_to_paisa(line1 + line2);
}
double calculate_discount(double subtotal)
{
    if (subtotal > DISCOUNT_THRESHOLD + 0.005)
        return round_to_paisa(subtotal * DISCOUNT_RATE);
    return 0.0;
}

double calculate_net_total(double subtotal, double discount)
{
    return round_to_paisa(subtotal - discount);
}

int higher_priced_item(double price1, double price2)
{
    double diff = price1 - price2;

    if (diff > 0.005)  return 1;
    if (diff < -0.005) return 2;
    return 0;
}

void print_item_line(int item_number, double price, int quantity, double line_total)
{
    printf("Item %d   %10.2f x %-5d = %12.2f\n", item_number, price, quantity, line_total);
}

void print_comparison(double price1, double price2)
{
    int higher = higher_priced_item(price1, price2);

    if (higher == 0)
        printf("Both items have the same unit price (Rs. %.2f).\n", price1);
    else
        printf("Item %d has the higher unit price (Rs. %.2f).\n",
               higher, higher == 1 ? price1 : price2);
}

void print_receipt(double price1, int qty1, double price2, int qty2)
{
    double line1    = calculate_line_total(price1, qty1);
    double line2    = calculate_line_total(price2, qty2);
    double subtotal = calculate_subtotal(line1, line2);
    double discount = calculate_discount(subtotal);
    double net      = calculate_net_total(subtotal, discount);

    printf("\n=========== RECEIPT ===========\n");
    print_item_line(1, price1, qty1, line1);
    print_item_line(2, price2, qty2, line2);
    printf("--------------------------------\n");
    printf("Subtotal:             Rs. %10.2f\n", subtotal);
    if (discount > 0.0)
        printf("Discount (10%%):       Rs. %10.2f\n", discount);
    else
        printf("Discount:             Rs. %10.2f (not eligible)\n", discount);
    printf("--------------------------------\n");
    printf("TOTAL PAYABLE:        Rs. %10.2f\n", net);
    printf("================================\n");
    print_comparison(price1, price2);
    printf("Thank you!\n");
}