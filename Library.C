#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Book {
    int id;
    char title[50];
    int isIssued;
    char issuedTo[50];
    char issueDate[11];  // Format: DD-MM-YYYY
    char returnDate[11]; // Format: DD-MM-YYYY
    struct Book *next;
} Book;

Book *head = NULL;

// Helper function to calculate the difference in days between two dates
int calculateDateDifference(const char *startDate, const char *endDate) {
    struct tm start = {0}, end = {0};

    sscanf(startDate, "%d-%d-%d", &start.tm_mday, &start.tm_mon, &start.tm_year);
    sscanf(endDate, "%d-%d-%d", &end.tm_mday, &end.tm_mon, &end.tm_year);

    start.tm_year -= 1900;
    start.tm_mon -= 1;
    end.tm_year -= 1900;
    end.tm_mon -= 1;

    time_t start_time = mktime(&start);
    time_t end_time = mktime(&end);

    return (end_time - start_time) / (60 * 60 * 24);
}

void addBookToList(int id, const char *title) {
    Book *newBook = (Book *)malloc(sizeof(Book));
    newBook->id = id;
    strcpy(newBook->title, title);
    newBook->isIssued = 0;
    newBook->issuedTo[0] = '\0';
    newBook->issueDate[0] = '\0';
    newBook->returnDate[0] = '\0';
    newBook->next = head;
    head = newBook;
}

void addBook() {
    int id;
    char title[50];
    printf("\nEnter Book ID: ");
    scanf("%d", &id);

    // Validate book ID for non-negative values
    if (id <= 0) {
        printf("\nError: Book ID must be a positive integer!\n");
        return;
    }

    // Check if a book with the same ID already exists
    Book *temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("\nError: A book with ID %d already exists!\n", id);
            return;
        }
        temp = temp->next;
    }

    printf("Enter Book Title: ");
    scanf(" %[^\n]", title);

    // Check if a book with the same title already exists
    temp = head;
    while (temp != NULL) {
        if (strcmp(temp->title, title) == 0) {
            printf("\nError: A book with the title '%s' already exists!\n", title);
            return;
        }
        temp = temp->next;
    }

    addBookToList(id, title);
    printf("\nBook added successfully!\n");
}




void viewBooks() {
    Book *temp = head;
    printf("\nAvailable Books:\n");
    while (temp != NULL) {
        printf("ID: %d, Title: %s, Status: %s", 
               temp->id, temp->title, temp->isIssued ? "Issued" : "Available");
        if (temp->isIssued) {
            printf(", Issued to: %s, Issue Date: %s", temp->issuedTo, temp->issueDate);
        }
        if (temp->returnDate[0] != '\0') {
            printf(", Returned on: %s", temp->returnDate);
        }
        printf("\n");
        temp = temp->next;
    }
}

void issueBook(int id) {
    char name[50], date[11];
    Book *temp = head;

    while (temp != NULL) {
        if (temp->id == id) {
            if (temp->isIssued) {
                printf("\nBook ID %d is already issued to %s.\n", id, temp->issuedTo);
            } else {
                printf("\nEnter name of the person issuing the book: ");
                scanf(" %[^\n]", name);
                temp->isIssued = 1;
                strcpy(temp->issuedTo, name);
                printf("Enter Issue Date (DD-MM-YYYY): ");
                scanf("%s", date);
                strcpy(temp->issueDate, date);
                printf("\nBook ID %d issued successfully to %s on %s.\n", id, name, date);
            }
            return;
        }
        temp = temp->next;
    }
    printf("\nBook ID %d not found.\n", id);
}

void returnBook(int id) {
    char date[11];
    int fine = 0;
    Book *temp = head;

    while (temp != NULL) {
        if (temp->id == id) {
            if (!temp->isIssued) {
                printf("\nBook ID %d was not issued.\n", id);
            } else {
                printf("Enter Return Date (DD-MM-YYYY): ");
                scanf("%s", date);
                strcpy(temp->returnDate, date);

                int daysDiff = calculateDateDifference(temp->issueDate, date);

                if (daysDiff > 30) {
                    fine = (daysDiff - 30) * 10;
                }

                printf("\nBook ID %d returned by %s on %s.\n", id, temp->issuedTo, date);
                printf("Total days issued: %d\n", daysDiff);
                if (fine > 0) {
                    printf("Late by %d days. Fine: Rs.%d\n", daysDiff - 30, fine);
                } else {
                    printf("Returned on time. No fine.\n");
                }

                temp->isIssued = 0;
                temp->issuedTo[0] = '\0';
                temp->issueDate[0] = '\0';
            }
            return;
        }
        temp = temp->next;
    }
    printf("\nBook ID %d not found.\n", id);
}

int main() {
    int choice, id;

    // Add initial books
    addBookToList(1, "C Programming");
    addBookToList(2, "Data Structures");
    addBookToList(3, "Algorithms");

    do {
        printf("\nLibrary Management System\n");
        printf("1. View Books\n");
        printf("2. Add Book\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewBooks();
                break;
            case 2:
                addBook();
                break;
            case 3:
                printf("\nEnter Book ID to issue: ");
                scanf("%d", &id);
                issueBook(id);
                break;
            case 4:
                printf("\nEnter Book ID to return: ");
                scanf("%d", &id);
                returnBook(id);
                break;
            case 5:
                printf("\nExiting the system.\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
