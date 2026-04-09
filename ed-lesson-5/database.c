#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50

struct Person {
    char *name;
    int age;
};

struct Database {
    struct Person *people;
    int size;
    int capacity;
};

void initDatabase(struct Database *db, int initialCapacity) {
    db->people =
        (struct Person *)malloc(initialCapacity * sizeof(struct Person));
    db->size = 0;
    db->capacity = initialCapacity;
}

void addPerson(struct Database *db, const char *name, int age) {
    if (db->size == db->capacity) {
        db->capacity *= 2;
        // this was using malloc, but it needs to use realloc actually, unless
        // we want  to create a new allocation everytime. MEMORY LEAK
        db->people = (struct Person *)realloc(
            db->people, db->capacity * sizeof(struct Person));
    }

    // pointless temp, wasn't free before. MEMORY LEAK
    char *temp = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(temp, name);
    free(temp);

    db->people[db->size].name =
        (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(db->people[db->size].name, name);
    db->people[db->size].age = age;
    db->size++;
}

void displayDatabase(struct Database *db) {
    printf("Database contents:\n");
    for (int i = 0; i < db->size; i++) {
        printf("%d. Name: %s, Age: %d\n", i + 1, db->people[i].name,
               db->people[i].age);
    }
}

int searchPerson(struct Database *db, const char *name) {
    for (int i = 0; i < db->size; i++) {
        if (strcmp(db->people[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void deletePerson(struct Database *db, const char *name) {
    int index = searchPerson(db, name);
    if (index != -1) {
        // this free didn't exist before. MEMORY LEAK
        free(db->people[index].name);
        for (int i = index; i < db->size - 1; i++) {
            db->people[i] = db->people[i + 1];
        }
        db->size--;
        printf("Person deleted successfully.\n");
    } else {
        printf("Person not found.\n");
    }
}

void freeDatabase(struct Database *db) {
    for (int i = 0; i < db->size; i++) {
        free(db->people[i].name);
    }
    free(db->people);
}

int main() {
    struct Database db;
    initDatabase(&db, 5);

    int choice;
    char name[MAX_NAME];
    int age;

    do {
        printf("\nDatabase Menu:\n");
        printf("1. Add Person\n");
        printf("2. Display Database\n");
        printf("3. Search Person\n");
        printf("4. Delete Person\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            printf("Enter name: ");
            fgets(name, MAX_NAME, stdin);
            name[strcspn(name, "\n")] = 0;
            printf("Enter age: ");
            scanf("%d", &age);
            getchar();
            addPerson(&db, name, age);
            break;
        case 2:
            displayDatabase(&db);
            break;
        case 3:
            printf("Enter name to search: ");
            fgets(name, MAX_NAME, stdin);
            name[strcspn(name, "\n")] = 0;
            int index = searchPerson(&db, name);
            if (index != -1) {
                printf("Person found: Name: %s, Age: %d\n",
                       db.people[index].name, db.people[index].age);
            } else {
                printf("Person not found.\n");
            }
            break;
        case 4:
            printf("Enter name to delete: ");
            fgets(name, MAX_NAME, stdin);
            name[strcspn(name, "\n")] = 0;
            deletePerson(&db, name);
            break;
        case 5:
            printf("Exiting...\n");
            // The freedb wasn't being called before. MEMORY LEAK
            freeDatabase(&db);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
