#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

struct Book {
    string title;
    string author;
    string ISBN;
    bool available;

    Book(string t, string a, string i) : title(t), author(a), ISBN(i), available(true) {}
};

struct Borrower {
    string name;
    vector<pair<Book*, time_t>> borrowedBooks; // Pair of Book pointer and borrowed time
};

class Library {
private:
    vector<Book> books;
    vector<Borrower> borrowers;

    void printMenu() {
        cout << "Library Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Search Book\n";
        cout << "3. Checkout Book\n";
        cout << "4. Return Book\n";
        cout << "5. Calculate Fine\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
    }

    void addBook() {
        string title, author, ISBN;
        cout << "Enter book title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter book author: ";
        getline(cin, author);
        cout << "Enter book ISBN: ";
        getline(cin, ISBN);
        books.push_back(Book(title, author, ISBN));
        cout << "Book added successfully.\n";
    }

    void searchBook() {
        string query;
        cout << "Enter title, author, or ISBN to search: ";
        cin.ignore();
        getline(cin, query);
        bool found = false;
        for (const auto& book : books) {
            if (book.title == query || book.author == query || book.ISBN == query) {
                cout << "Book found: " << book.title << " by " << book.author << " (ISBN: " << book.ISBN << ")\n";
                cout << "Status: " << (book.available ? "Available" : "Checked Out") << "\n";
                found = true;
            }
        }
        if (!found) {
            cout << "Book not found.\n";
        }
    }

    void checkoutBook() {
        string ISBN, borrowerName;
        cout << "Enter ISBN of the book to checkout: ";
        cin.ignore();
        getline(cin, ISBN);
        auto book = find_if(books.begin(), books.end(), [&](const Book& b) { return b.ISBN == ISBN; });
        if (book != books.end() && book->available) {
            cout << "Enter borrower's name: ";
            getline(cin, borrowerName);
            auto borrower = find_if(borrowers.begin(), borrowers.end(), [&](const Borrower& b) { return b.name == borrowerName; });
            if (borrower == borrowers.end()) {
                borrowers.push_back(Borrower{ borrowerName });
                borrower = borrowers.end() - 1;
            }
            book->available = false;
            borrower->borrowedBooks.push_back(make_pair(&(*book), time(0)));
            cout << "Book checked out successfully.\n";
        } else {
            cout << "Book is not available or does not exist.\n";
        }
    }

    void returnBook() {
        string ISBN, borrowerName;
        cout << "Enter ISBN of the book to return: ";
        cin.ignore();
        getline(cin, ISBN);
        auto book = find_if(books.begin(), books.end(), [&](const Book& b) { return b.ISBN == ISBN; });
        if (book != books.end() && !book->available) {
            cout << "Enter borrower's name: ";
            getline(cin, borrowerName);
            auto borrower = find_if(borrowers.begin(), borrowers.end(), [&](const Borrower& b) { return b.name == borrowerName; });
            if (borrower != borrowers.end()) {
                auto borrowedBook = find_if(borrower->borrowedBooks.begin(), borrower->borrowedBooks.end(),
                    [&](const pair<Book*, time_t>& b) { return b.first->ISBN == ISBN; });
                if (borrowedBook != borrower->borrowedBooks.end()) {
                    book->available = true;
                    borrower->borrowedBooks.erase(borrowedBook);
                    cout << "Book returned successfully.\n";
                } else {
                    cout << "This borrower did not borrow this book.\n";
                }
            } else {
                cout << "Borrower not found.\n";
            }
        } else {
            cout << "Book is already available or does not exist.\n";
        }
    }

    void calculateFine() {
        string borrowerName;
        cout << "Enter borrower's name: ";
        cin.ignore();
        getline(cin, borrowerName);
        auto borrower = find_if(borrowers.begin(), borrowers.end(), [&](const Borrower& b) { return b.name == borrowerName; });
        if (borrower != borrowers.end()) {
            const double finePerDay = 1.0; // Assume fine is $1 per day overdue
            time_t now = time(0);
            double totalFine = 0.0;
            for (const auto& b : borrower->borrowedBooks) {
                double daysOverdue = difftime(now, b.second) / (60 * 60 * 24);
                if (daysOverdue > 14) { // Assume 14 days borrowing period
                    totalFine += (daysOverdue - 14) * finePerDay;
                }
            }
            cout << "Total fine for " << borrowerName << " is $" << totalFine << "\n";
        } else {
            cout << "Borrower not found.\n";
        }
    }

public:
    void run() {
        int choice;
        do {
            printMenu();
            cin >> choice;

            switch (choice) {
                case 1:
                    addBook();
                    break;
                case 2:
                    searchBook();
                    break;
                case 3:
                    checkoutBook();
                    break;
                case 4:
                    returnBook();
                    break;
                case 5:
                    calculateFine();
                    break;
                case 6:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 6);
    }
};

int main() {
    Library library;
    library.run();
    return 0;
}