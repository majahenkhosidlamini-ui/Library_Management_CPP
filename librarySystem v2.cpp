#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>

using namespace std;

// Struct for Book Information
struct Book {
    int bookID;
    string title;
    string author;
    string category;
    bool isAvailable;
};

// Base Class: LibraryMember
class LibraryMember {
protected:
    int memberID;
    string name;
    string memberType;
public:
    LibraryMember(int id, string n, string type) : memberID(id), name(n), memberType(type) {}
    virtual ~LibraryMember() {}

    virtual int getBorrowLimit() const { return 3; }
    virtual double calculateFine(int overdueDays) const { return overdueDays * 1.0; }

    string getName() const { return name; }
    int getID() const { return memberID; }
    string getType() const { return memberType; }

    friend void auditRecords(const LibraryMember& m);
};

// Derived Class: StudentMember
class StudentMember : public LibraryMember {
public:
    StudentMember(int id, string n) : LibraryMember(id, n, "Student") {}
    int getBorrowLimit() const override { return 2; }
    double calculateFine(int overdueDays) const override { return overdueDays * 0.50; }
};

// Derived Class: LecturerMember
class LecturerMember : public LibraryMember {
public:
    LecturerMember(int id, string n) : LibraryMember(id, n, "Lecturer") {}
    int getBorrowLimit() const override { return 5; }
    double calculateFine(int overdueDays) const override { return overdueDays * 0.20; }
};

// Friend function for auditing
void auditRecords(const LibraryMember& m) {
    cout << "[AUDIT] Member ID: " << m.memberID << " | Name: " << m.name << " | Type: " << m.memberType << endl;
}

// Cross-platform screen clear function
void clearScreen() {
#if defined(_WIN32)
    system("cls");
#else
    system("clear");
#endif
}

// Registration Functions with looping ID validation and uniqueness checks
void registerBook(Book books[], int& bookCount, int maxCapacity) {
    if (bookCount >= maxCapacity) {
        cout << "Error: Book registry is full.\n";
        return;
    }

    int id;
    while (true) {
        cout << "Enter 4-digit Book ID: ";
        if (!(cin >> id)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter numbers only.\n";
            continue;
        }

        if (id < 1000 || id > 9999) {
            cout << "Error: Book ID must be exactly 4 digits long. Please try again.\n";
            continue;
        }

        // Check if Book ID has already been used
        bool unique = true;
        for (int i = 0; i < bookCount; i++) {
            if (books[i].bookID == id) {
                cout << "Error: Book ID " << id << " has already been used. Please use a unique ID.\n";
                unique = false;
                break;
            }
        }

        if (unique) {
            books[bookCount].bookID = id;
            break;
        }
    }

    cout << "Enter Title: ";
    cin >> ws;
    getline(cin, books[bookCount].title);
    cout << "Enter Author: ";
    getline(cin, books[bookCount].author);
    cout << "Enter Category: ";
    getline(cin, books[bookCount].category);
    books[bookCount].isAvailable = true;
    bookCount++;
    cout << "Book registered successfully!\n";
}

void registerMember(LibraryMember* members[], int& memberCount, int maxCapacity) {
    if (memberCount >= maxCapacity) {
        cout << "Error: Member registry is full.\n";
        return;
    }

    int id;
    while (true) {
        cout << "Enter 6-digit Member ID: ";
        if (!(cin >> id)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter numbers only.\n";
            continue;
        }

        if (id < 100000 || id > 999999) {
            cout << "Error: Member ID must be exactly 6 digits long. Please try again.\n";
            continue;
        }

        // Check if Member ID has already been used
        bool unique = true;
        for (int i = 0; i < memberCount; i++) {
            if (members[i]->getID() == id) {
                cout << "Error: Member ID " << id << " has already been used. Please use a unique ID.\n";
                unique = false;
                break;
            }
        }

        if (unique) break;
    }

    string name;
    cout << "Enter Name: ";
    cin >> ws;
    getline(cin, name);

    int typeChoice;
    cout << "Select Member Type (1. Student, 2. Lecturer): ";
    cin >> typeChoice;

    if (typeChoice == 1) {
        members[memberCount] = new StudentMember(id, name);
    } else {
        members[memberCount] = new LecturerMember(id, name);
    }
    memberCount++;
    cout << "Library member registered successfully!\n";
}

// Authenticate with looping until a valid ID is provided or user cancels (enters 0)
LibraryMember* authenticateMember(LibraryMember* members[], int memberCount) {
    int id;
    while (true) {
        cout << "Enter your 6-digit Member ID (or 0 to cancel): ";
        if (!(cin >> id)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please try again.\n";
            continue;
        }
        if (id == 0) return nullptr;

        for (int i = 0; i < memberCount; i++) {
            if (members[i]->getID() == id) {
                cout << "Welcome back, " << members[i]->getName() << " (" << members[i]->getType() << ")!\n";
                return members[i];
            }
        }
        cout << "Error: Member ID not found. Please try again or enter 0 to cancel.\n";
    }
}

// Display all books
void displayAllBooks(const Book books[], int bookCount) {
    cout << "\n--- Current Library Books ---\n";
    if (bookCount == 0) {
        cout << "No books registered in the system.\n";
        return;
    }
    for (int i = 0; i < bookCount; i++) {
        cout << "[" << books[i].bookID << "] " << books[i].title
             << " by " << books[i].author
             << " | Status: " << (books[i].isAvailable ? "Available" : "Borrowed") << endl;
    }
}

void displayAvailableBooks(const Book books[], int bookCount) {
    cout << "\n--- Available Books ---\n";
    bool anyAvailable = false;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isAvailable) {
            cout << "[" << books[i].bookID << "] " << books[i].title << " by " << books[i].author << endl;
            anyAvailable = true;
        }
    }
    if (!anyAvailable) cout << "No available books found.\n";
}

// Partial string matching for searching books
void searchBook(const Book books[], int bookCount, string query) {
    bool found = false;
    cout << "\n--- Search Results ---\n";
    for (int i = 0; i < bookCount; i++) {
        if (to_string(books[i].bookID) == query || books[i].title.find(query) != string::npos) {
            cout << "Found: [" << books[i].bookID << "] " << books[i].title
                 << " by " << books[i].author
                 << " | Category: " << books[i].category
                 << " | Status: " << (books[i].isAvailable ? "Available" : "Borrowed") << endl;
            found = true;
        }
    }
    if (!found) cout << "No matching books found.\n";
}

// Borrow book by entering title (with pointer usage)
void borrowBookByTitle(Book books[], int bookCount, string titleQuery) {
    bool found = false;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].title.find(titleQuery) != string::npos) {
            found = true;
            Book* bookPtr = &books[i]; // Pointer usage
            if (bookPtr->isAvailable) {
                bookPtr->isAvailable = false;
                cout << "Success: Book '" << bookPtr->title << "' has been borrowed.\n";
            } else {
                cout << "Error: Book '" << bookPtr->title << "' is already borrowed.\n";
            }
            return;
        }
    }
    if (!found) cout << "Book title not found.\n";
}

void returnBook(Book* bookPtr, int overdueDays, const LibraryMember* member) {
    bookPtr->isAvailable = true;
    double fine = member->calculateFine(overdueDays);
    cout << "Book returned successfully. Overdue fine: $" << fixed << setprecision(2) << fine << endl;
}

int main() {
    const int MAX = 50;
    Book libraryBooks[MAX];
    int bookCount = 0;

    LibraryMember* members[MAX];
    int memberCount = 0;

    int roleChoice;
    do {
        clearScreen();
        cout << "\n=== Botho University Library System - Login Panel ===\n"
             << "1. Admin Login\n"
             << "2. Student Login\n"
             << "3. Exit System\n"
             << "Select Role: ";
        if (!(cin >> roleChoice)) break;

        if (roleChoice == 1) {
            // Admin Menu Loop
            int choice;
            do {
                cout << "\n=== Botho University Library System ===\n"
                     << "=== Admin Menu ===\n"
                     << "1. Register Book\n"
                     << "2. Register Library Member\n"
                     << "3. Display Available Books\n"
                     << "4. Search Book (Partial Title Supported)\n"
                     << "5. Borrow Book (Member Verification Required)\n"
                     << "6. Return Book & Calculate Fine\n"
                     << "7. Audit Member Record\n"
                     << "8. Logout / Back to Login\n"
                     << "Enter choice: ";
                if (!(cin >> choice)) break;

                switch (choice) {
                    case 1:
                        registerBook(libraryBooks, bookCount, MAX);
                        break;
                    case 2:
                        registerMember(members, memberCount, MAX);
                        break;
                    case 3:
                        displayAvailableBooks(libraryBooks, bookCount);
                        break;
                    case 4: {
                        string query;
                        cout << "Enter Book ID or part of the title: ";
                        cin >> ws;
                        getline(cin, query);
                        searchBook(libraryBooks, bookCount, query);
                        break;
                    }
                    case 5: {
                        if (memberCount == 0) {
                            cout << "No members registered in the system.\n";
                            cout << "Error: You are not a registered member. Please register first.\n";
                            break;
                        }
                        LibraryMember* currentMember = authenticateMember(members, memberCount);
                        if (currentMember == nullptr) {
                            cout << "Borrowing canceled.\n";
                            break;
                        }
                        displayAllBooks(libraryBooks, bookCount);
                        if (bookCount > 0) {
                            string titleQuery;
                            cout << "Enter the book title (or part of it) to borrow: ";
                            cin >> ws;
                            getline(cin, titleQuery);
                            borrowBookByTitle(libraryBooks, bookCount, titleQuery);
                        }
                        break;
                    }
                    case 6: {
                        int bID, days;
                        cout << "Enter 4-digit Book ID to return: ";
                        cin >> bID;
                        cout << "Enter Overdue Days: ";
                        cin >> days;
                        if (memberCount == 0) {
                            cout << "No members registered yet.\n";
                            break;
                        }
                        bool foundBook = false;
                        for (int i = 0; i < bookCount; i++) {
                            if (libraryBooks[i].bookID == bID) {
                                returnBook(&libraryBooks[i], days, members[0]);
                                foundBook = true;
                                break;
                            }
                        }
                        if (!foundBook) cout << "Book ID not found.\n";
                        break;
                    }
                    case 7:
                        if (memberCount > 0) {
                            auditRecords(*members[0]);
                        } else {
                            cout << "No members to audit.\n";
                        }
                        break;
                    case 8:
                        cout << "Logging out from Admin panel...\n";
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 8);

        } else if (roleChoice == 2) {
            // Student Menu Loop
            int choice;
            do {
                cout << "\n=== Botho University Library System ===\n"
                     << "=== Student Menu ===\n"
                     << "1. Register Library Member\n"
                     << "2. Display Available Books\n"
                     << "3. Search Book (Partial Title Supported)\n"
                     << "4. Borrow Book (Member Verification Required)\n"
                     << "5. Return Book & Calculate Fine\n"
                     << "6. Logout / Back to Login\n"
                     << "Enter choice: ";
                if (!(cin >> choice)) break;

                switch (choice) {
                    case 1:
                        registerMember(members, memberCount, MAX);
                        break;
                    case 2:
                        displayAvailableBooks(libraryBooks, bookCount);
                        break;
                    case 3: {
                        string query;
                        cout << "Enter Book ID or part of the title: ";
                        cin >> ws;
                        getline(cin, query);
                        searchBook(libraryBooks, bookCount, query);
                        break;
                    }
                    case 4: {
                        if (memberCount == 0) {
                            cout << "No members registered in the system.\n";
                            cout << "Error: You are not a registered member. Please register first using Option 1.\n";
                            break;
                        }
                        LibraryMember* currentMember = authenticateMember(members, memberCount);
                        if (currentMember == nullptr) {
                            cout << "Borrowing canceled.\n";
                            break;
                        }
                        displayAllBooks(libraryBooks, bookCount);
                        if (bookCount > 0) {
                            string titleQuery;
                            cout << "Enter the book title (or part of it) to borrow: ";
                            cin >> ws;
                            getline(cin, titleQuery);
                            borrowBookByTitle(libraryBooks, bookCount, titleQuery);
                        }
                        break;
                    }
                    case 5: {
                        int bID, days;
                        cout << "Enter 4-digit Book ID to return: ";
                        cin >> bID;
                        cout << "Enter Overdue Days: ";
                        cin >> days;
                        if (memberCount == 0) {
                            cout << "No members registered yet.\n";
                            break;
                        }
                        bool foundBook = false;
                        for (int i = 0; i < bookCount; i++) {
                            if (libraryBooks[i].bookID == bID) {
                                returnBook(&libraryBooks[i], days, members[0]);
                                foundBook = true;
                                break;
                            }
                        }
                        if (!foundBook) cout << "Book ID not found.\n";
                        break;
                    }
                    case 6:
                        cout << "Logging out from Student panel...\n";
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 6);

        } else if (roleChoice == 3) {
            cout << "Exiting system....\n";
            break;
        } else {
            cout << "Invalid selection. Please choose 1, 2, or 3.\n";
        }
    } while (true);

    // Clean up dynamically allocated memory
    for (int i = 0; i < memberCount; i++) {
        delete members[i];
    }

    return 0;
}
