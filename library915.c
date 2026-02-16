#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[50];
    char author[50];
    int isBorrowed;
    char borrower[50];
    int daysBorrowed; // number of days since borrowed
};

// Function declarations
void addBook();
void viewBooks();
void searchBook();
void borrowBook();
void returnBook();
void showStatistics();
int getBookCount(FILE *fp);

int main() {
    int choice;
    do {
        printf("\n=== Cool Library Management System ===\n");
        printf("1. Add Book\n");
        printf("2. View All Books\n");
        printf("3. Search Book\n");
        printf("4. Borrow Book\n");
        printf("5. Return Book\n");
        printf("6. Library Statistics\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        switch(choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: searchBook(); break;
            case 4: borrowBook(); break;
            case 5: returnBook(); break;
            case 6: showStatistics(); break;
            case 7: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while(choice != 7);

    return 0;
}

// Add a new book
void addBook() {
    struct Book b;
    FILE *fp = fopen("books.dat", "ab");
    if (!fp) { printf("Error opening file.\n"); return; }

    printf("Enter Book ID: "); scanf("%d", &b.id); getchar();
    printf("Enter Book Title: "); fgets(b.title,50,stdin); b.title[strcspn(b.title,"\n")]=0;
    printf("Enter Book Author: "); fgets(b.author,50,stdin); b.author[strcspn(b.author,"\n")]=0;

    b.isBorrowed = 0;
    strcpy(b.borrower, "");
    b.daysBorrowed = 0;

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}

// View all books
void viewBooks() {
    struct Book b;
    FILE *fp = fopen("books.dat", "rb");
    if (!fp) { printf("No books found.\n"); return; }

    printf("\n%-5s %-30s %-20s %-10s %-15s %-10s\n", "ID", "Title", "Author", "Status", "Borrower", "Days");
    printf("-------------------------------------------------------------------------------\n");

    while (fread(&b,sizeof(b),1,fp)) {
        printf("%-5d %-30s %-20s %-10s %-15s %-10d\n", 
            b.id, b.title, b.author, b.isBorrowed?"Borrowed":"Available", b.isBorrowed?b.borrower:"-", b.daysBorrowed);
    }
    fclose(fp);
}

// Search by title or author
void searchBook() {
    char query[50];
    struct Book b;
    FILE *fp = fopen("books.dat", "rb");
    if (!fp) { printf("No books found.\n"); return; }

    printf("Enter Title or Author to search: ");
    fgets(query,50,stdin); query[strcspn(query,"\n")]=0;

    int found = 0;
    printf("\nSearch Results:\n");
    printf("%-5s %-30s %-20s %-10s\n", "ID","Title","Author","Status");
    printf("-------------------------------------------------------\n");

    while (fread(&b,sizeof(b),1,fp)) {
        if (strstr(b.title, query) || strstr(b.author, query)) {
            found = 1;
            printf("%-5d %-30s %-20s %-10s\n", b.id, b.title, b.author, b.isBorrowed?"Borrowed":"Available");
        }
    }

    if (!found) printf("No books matched your search.\n");
    fclose(fp);
}

// Borrow book with borrower name and days borrowed
void borrowBook() {
    int id, found = 0;
    char borrower[50];
    int days;
    struct Book b;
    FILE *fp = fopen("books.dat", "rb+");
    if (!fp) { printf("No books found.\n"); return; }

    printf("Enter Book ID to borrow: "); scanf("%d", &id); getchar();
    printf("Enter Borrower Name: "); fgets(borrower,50,stdin); borrower[strcspn(borrower,"\n")]=0;
    printf("Enter number of days to borrow: "); scanf("%d", &days); getchar();

    while (fread(&b,sizeof(b),1,fp)) {
        if (b.id == id) {
            found = 1;
            if (b.isBorrowed) {
                printf("Book is already borrowed.\n");
            } else {
                b.isBorrowed = 1;
                strcpy(b.borrower, borrower);
                b.daysBorrowed = days;
                fseek(fp, -sizeof(b), SEEK_CUR);
                fwrite(&b,sizeof(b),1,fp);
                printf("Book borrowed successfully!\n");
            }
            break;
        }
    }

    if (!found) printf("Book ID not found.\n");
    fclose(fp);
}

// Return book
void returnBook() {
    int id, found = 0;
    struct Book b;
    FILE *fp = fopen("books.dat", "rb+");
    if (!fp) { printf("No books found.\n"); return; }

    printf("Enter Book ID to return: "); scanf("%d", &id); getchar();

    while (fread(&b,sizeof(b),1,fp)) {
        if (b.id == id) {
            found = 1;
            if (!b.isBorrowed) {
                printf("This book was not borrowed.\n");
            } else {
                b.isBorrowed = 0;
                strcpy(b.borrower,"");
                b.daysBorrowed = 0;
                fseek(fp, -sizeof(b), SEEK_CUR);
                fwrite(&b,sizeof(b),1,fp);
                printf("Book returned successfully!\n");
            }
            break;
        }
    }

    if (!found) printf("Book ID not found.\n");
    fclose(fp);
}

// Show statistics
void showStatistics() {
    struct Book b;
    FILE *fp = fopen("books.dat","rb");
    if (!fp) { printf("No books found.\n"); return; }

    int total=0, borrowed=0, available=0;
    while (fread(&b,sizeof(b),1,fp)) {
        total++;
        if (b.isBorrowed) borrowed++;
        else available++;
    }

    printf("\nLibrary Statistics:\n");
    printf("Total Books: %d\n", total);
    printf("Borrowed Books: %d\n", borrowed);
    printf("Available Books: %d\n", available);
    fclose(fp);
}

