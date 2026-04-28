#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define INPUT_BUF (100)

void finish_day();
char *day_name = NULL;

struct purchase_line {
    char item[INPUT_BUF];
    unsigned quantity;
    double cost;
};

// This should be implemented as a priority queue. I'll see if with a heap tree,
// or a in insert sort
struct purchase {
    struct purchase_line **items;
    unsigned number_of_lines;
};

struct client {
    char name[INPUT_BUF];
    char phone[INPUT_BUF];
    time_t checkout_time;
    struct purchase *purchase;
    struct client *next_client;
};

struct client *first_customer = NULL;

void Malloc(size_t size, void **var) {
    *var = malloc(size);
    if (!var) {
        errno = 12;
        exit(-1);
    }
}

void get_input(char *input_result, char *label, size_t label_size,
               bool allow_terminate) {

    char str[INPUT_BUF];

    write(STDOUT_FILENO, label, label_size);

    // using a long because this could return -1 on error
    long str_len = 0;
    while ((str_len = read(STDIN_FILENO, str, INPUT_BUF)) < 3) {
        if (str_len == 0) {
            if (allow_terminate)
                finish_day();
            else {
                char mess[] =
                    "Day can't be terminated in the middle of a purchase";
                write(STDOUT_FILENO, mess, sizeof(mess));
            }
        }
        char error[] = "Sorry, I couldn't get that. Try again \n";
        write(STDOUT_FILENO, error, sizeof(error));
        write(STDOUT_FILENO, label, label_size);
    }

    str[str_len - 1] = '\0'; // replacing \n for the null term
    strcpy(input_result, str);
}

void finish_day() {
    mode_t mode = 0666; // rw for all
    int fd = open(day_name, O_WRONLY | O_CREAT | O_TRUNC, mode);
    struct client *customer = first_customer;
    char buf[INPUT_BUF * 4];
    while (customer) {
        sprintf(buf, "\nName: %s, Phone: %s Checkout Time: %s", customer->name,
                customer->phone, ctime(&customer->checkout_time));
        write(fd, buf, strlen(buf));
        for (int i = 0; i < customer->purchase->number_of_lines; i++) {
            struct purchase_line *purchase = customer->purchase->items[i];
            sprintf(buf, "Item: %s Quantity: %d Price: %.2lf Full Price: %.2lf\n",
                    purchase->item, purchase->quantity, purchase->cost,
                    purchase->cost * purchase->quantity);
            write(fd, buf, strlen(buf));
            free(purchase);
        }
        free(customer->purchase->items);
        free(customer->purchase);
        struct client *prev_cus = customer;
        customer = customer->next_client;
        free(prev_cus);
    }
    char bye_message[] = "\nThanks fo working today! Ily\n";
    write(STDOUT_FILENO, bye_message, sizeof(bye_message));
    exit(1);
}

void get_purchase(struct purchase *purchase_list) {
    char label[] = "Please input item lines like: <item> <quantity> <cost with "
                   "two dec>: \n";
    char line_item[INPUT_BUF];
    struct purchase_line **items = NULL;
    Malloc(sizeof(void *), (void **)&items);
    purchase_list->items = items;
    get_input(line_item, label, sizeof(label), false);
    while (strcmp(line_item, "done") != 0) {
        char name[INPUT_BUF];
        int quantity;
        double price;
        // the 99 is important as sscanf can overflow my string
        if (sscanf(line_item, "%99s %d %lf", name, &quantity, &price) != 3) {
            char message[] = "There was an error on the input, try again\n";
            write(STDOUT_FILENO, message, sizeof(message));
            continue;
        }
        struct purchase_line *purchase;
        Malloc(sizeof(*purchase), (void **)&purchase);
        purchase->cost = price;
        strcpy(purchase->item,name);
        purchase->quantity = quantity;
        purchase_list->number_of_lines++;
        items =
            realloc(items, sizeof(void *) * (purchase_list->number_of_lines));
        if (!items) {
            errno = 12;
            return;
        }
        purchase_list->items = items;
        items[purchase_list->number_of_lines-1] = purchase;
        get_input(line_item, label, sizeof(label), false);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        char message[] = "Please input the name of the day\n";
        write(STDOUT_FILENO, message, sizeof(message));
        exit(0);
    }
    day_name = argv[1];
    struct client *today_list = NULL;
    while (1) {
        char welcome_message[] =
            "Welcome to the market! please input the data of the customer:\n";
        write(STDOUT_FILENO, welcome_message, sizeof(welcome_message));

        char name[INPUT_BUF];
        char name_label[] = "name: ";
        get_input(name, name_label, sizeof(name_label), true);

        char phone[INPUT_BUF];
        char phone_label[] = "phone: ";
        get_input(phone, phone_label, sizeof(phone_label), true);

        // I'm not allowing a safe exit on purchase lines as that might
        // require me to dealloc in the middle of a transaction.
        struct purchase *client_purchase = NULL;
        Malloc(sizeof(struct purchase), (void **)&client_purchase);
        client_purchase->number_of_lines = 0;
        get_purchase(client_purchase);

        struct client *client = NULL;
        Malloc(sizeof(struct client), (void **)&client);

        strcpy(client->name, name);
        strcpy(client->phone, phone);
        client->checkout_time = time(NULL);
        client->purchase = client_purchase;
        client->next_client = NULL;

        if (first_customer) {
            struct client *customer = first_customer;
            while (customer->next_client) {
                customer = customer->next_client;
            }
            customer->next_client = client;
        } else {
            first_customer = client;
        }
        char bye_message[] = "Purchase finished!, now for the next client\n";
        write(STDOUT_FILENO, bye_message, sizeof(bye_message));
    }
}
