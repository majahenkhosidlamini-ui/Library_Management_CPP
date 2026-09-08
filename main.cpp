#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

const int    MAX_BOOKS     = 100;
const int    MAX_MEMBERS   = 100;
const int    LOAN_PERIOD   = 14;
const double FINE_STUDENT  = 0.50;
const double FINE_STAFF    = 0.25;
const double FINE_FACULTY  = 0.10;
const double FINE_DEFAULT  = 0.75;

const string STATUS_AVAILABLE = "AVAILABLE";
const string STATUS_BORROWED  = "BORROWED";

struct Book {
    int    id = 0;
    string title;
    string author;
    string category;
    string status = STATUS_AVAILABLE;
};

struct Member {
    int    id = 0;
    string name;
    string type;
};

struct BorrowRecord {
    bool   active    = false;
    int    borrowDay  = 0;
    int    dueDay     = 0;
    double lastFine   = 0.0;
};


Book*   books        = nullptr;
int     bookCount     = 0;
int     bookCapacity  = 4;

Member* members       = nullptr;
int     memberCount    = 0;
int     memberCapacity = 4;


static BorrowRecord borrowTable[MAX_MEMBERS][MAX_BOOKS];

int systemDay = 1;

void  ensureBookCapacity();
void  ensureMemberCapacity();
int   findBookIndexById(int bookId);
int   findMemberIndexById(int memberId);
BorrowRecord* borrowCell(int memberIndex, int bookIndex);

void  registerBook();
void  registerMember();
void  searchBook();
void  borrowBook();
void  returnBook();
void  displayBooks();
double fineRateForMember(const string& memberType);
int   readInt(const string& prompt);
string readLine(const string& prompt);


void ensureBookCapacity() {
    if (bookCount < bookCapacity) return;

    int newCapacity = bookCapacity * 2;
    Book* newBlock = new Book[newCapacity];


    Book* src = books;
    Book* dst = newBlock;
    for (int i = 0; i < bookCount; ++i) {
        *(dst + i) = *(src + i);
    }

    delete[] books;
    books = newBlock;
    bookCapacity = newCapacity;
}

void ensureMemberCapacity() {
    if (memberCount < memberCapacity) return;

    int newCapacity = memberCapacity * 2;
    Member* newBlock = new Member[newCapacity];

    Member* src = members;
    Member* dst = newBlock;
    for (int i = 0; i < memberCount; ++i) {
        *(dst + i) = *(src + i);
    }

    delete[] members;
    members = newBlock;
    memberCapacity = newCapacity;
}


int findBookIndexById(int bookId) {
    Book* p = books;
    for (int i = 0; i < bookCount; ++i, ++p) {
        if (p->id == bookId) return i;
    }
    return -1;
}

int findMemberIndexById(int memberId) {
    Member* p = members;
    for (int i = 0; i < memberCount; ++i, ++p) {
        if (p->id == memberId) return i;
    }
    return -1;
}


BorrowRecord* borrowCell(int memberIndex, int bookIndex) {
    BorrowRecord* base = &borrowTable[0][0];
    BorrowRecord* cell = base + (memberIndex * MAX_BOOKS) + bookIndex;
    return cell;
}


int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid number, please try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

double fineRateForMember(const string& memberType) {
    if (memberType == "STUDENT") return FINE_STUDENT;
    if (memberType == "STAFF")   return FINE_STAFF;
    if (memberType == "FACULTY") return FINE_FACULTY;
    return FINE_DEFAULT;
}


// OPTION 1 Register Book

void registerBook() {
    ensureBookCapacity();

    Book newBook;
    newBook.id       = readInt("Enter Book ID: ");

    if (findBookIndexById(newBook.id) != -1) {
        cout << "A book with that ID already exists.\n";
        return;
    }

    newBook.title    = readLine("Enter Book Title: ");
    newBook.author   = readLine("Enter Book Author: ");
    newBook.category = readLine("Enter Book Category: ");
    newBook.status   = STATUS_AVAILABLE;


    *(books + bookCount) = newBook;
    ++bookCount;

    cout << "Book registered successfully.\n";
}


//  OPTION 2 Register Member

void registerMember() {
    ensureMemberCapacity();

    Member newMember;
    newMember.id = readInt("Enter Member ID: ");

    if (findMemberIndexById(newMember.id) != -1) {
        cout << "A member with that ID already exists.\n";
        return;
    }

    newMember.name = readLine("Enter Member Name: ");

    string type;
    while (true) {
        type = readLine("Enter Member Type (STUDENT/STAFF/FACULTY): ");
        if (type == "STUDENT" || type == "STAFF" || type == "FACULTY") break;
        cout << "Please enter STUDENT, STAFF, or FACULTY.\n";
    }
    newMember.type = type;

    *(members + memberCount) = newMember;
    ++memberCount;

    cout << "Member registered successfully.\n";
}


//  OPTION 3 Search Book

void searchBook() {
    if (bookCount == 0) {
        cout << "No books registered yet.\n";
        return;
    }

    cout << "Search by (1) Book ID or (2) Book Title? ";
    int mode = readInt("");

    bool found = false;
    Book* p = books;

    if (mode == 1) {
        int id = readInt("Enter Book ID: ");
        for (int i = 0; i < bookCount; ++i, ++p) {
            if (p->id == id) {
                found = true;
                cout << "\n[" << p->id << "] " << p->title << " by " << p->author
                     << " | Category: " << p->category
                     << " | Status: " << p->status << "\n";
                break;
            }
        }
    } else {
        string title = readLine("Enter Book Title (or part of it): ");
        for (int i = 0; i < bookCount; ++i, ++p) {
            if (p->title.find(title) != string::npos) {
                found = true;
                cout << "[" << p->id << "] " << p->title << " by " << p->author
                     << " | Category: " << p->category
                     << " | Status: " << p->status << "\n";
            }
        }
    }

    if (!found) cout << "No matching book found.\n";
}


//  OPTION 4 Borrow Book

void borrowBook() {
    int memberId = readInt("Enter Member ID: ");
    int bookId   = readInt("Enter Book ID: ");

    int mIdx = findMemberIndexById(memberId);
    int bIdx = findBookIndexById(bookId);

    if (mIdx == -1 || bIdx == -1) {
        cout << "Borrowing not allowed: member or book does not exist.\n";
        return;
    }
    if (bIdx >= MAX_BOOKS || mIdx >= MAX_MEMBERS) {
        cout << "Borrowing not allowed: tracking capacity exceeded.\n";
        return;
    }

    Book* bookPtr = books + bIdx;   // pointer to the specific book record

    if (bookPtr->status != STATUS_AVAILABLE) {
        cout << "Borrowing not allowed: book is not available.\n";
        return;
    }


    BorrowRecord* rec = borrowCell(mIdx, bIdx);
    rec->active    = true;
    rec->borrowDay = systemDay;
    rec->dueDay    = systemDay + LOAN_PERIOD;

    bookPtr->status = STATUS_BORROWED;

    cout << "Borrowing confirmed. Book \"" << bookPtr->title
         << "\" borrowed on day " << rec->borrowDay
         << ", due on day " << rec->dueDay << ".\n";

    ++systemDay;   // advance simulated time after each transaction
}


//  OPTION 5 Return Book

void returnBook() {
    int bookId = readInt("Enter Book ID being returned: ");
    int bIdx = findBookIndexById(bookId);

    if (bIdx == -1) {
        cout << "No such book exists.\n";
        return;
    }

    Book* bookPtr = books + bIdx;

    if (bookPtr->status != STATUS_BORROWED) {
        cout << "This book is not currently on loan.\n";
        return;
    }



    BorrowRecord* base = &borrowTable[0][0];
    int holderIdx = -1;
    for (int m = 0; m < memberCount; ++m) {
        BorrowRecord* cell = base + (m * MAX_BOOKS) + bIdx;
        if (cell->active) {
            holderIdx = m;
            break;
        }
    }

    if (holderIdx == -1) {
        cout << "No active borrowing record found for this book.\n";
        return;
    }

    int returnDay = readInt("Enter current day number for the return: ");

    BorrowRecord* rec = borrowCell(holderIdx, bIdx);
    Member* borrower  = members + holderIdx;

    int overdueDays = returnDay - rec->dueDay;
    double fine = 0.0;

    if (overdueDays > 0) {
        fine = overdueDays * fineRateForMember(borrower->type);
    } else {
        fine = 0.0;
        overdueDays = 0;
    }

    rec->lastFine = fine;
    rec->active   = false;
    bookPtr->status = STATUS_AVAILABLE;

    cout << fixed << setprecision(2);
    cout << "Return confirmed for \"" << bookPtr->title << "\" (Member: "
         << borrower->name << ").\n";
    if (overdueDays > 0) {
        cout << "Overdue by " << overdueDays << " day(s). Fine due: $"
             << fine << "\n";
    } else {
        cout << "Returned on time. No fine.\n";
    }
}


//   OPTION 6 Display Books

void displayBooks() {
    if (bookCount == 0) {
        cout << "No books registered yet.\n";
        return;
    }

    cout << "\n" << left
         << setw(6)  << "ID"
         << setw(25) << "Title"
         << setw(20) << "Author"
         << setw(15) << "Category"
         << setw(12) << "Status" << "\n";
    cout << string(78, '-') << "\n";

    Book* p = books;
    for (int i = 0; i < bookCount; ++i, ++p) {
        cout << left
             << setw(6)  << p->id
             << setw(25) << p->title
             << setw(20) << p->author
             << setw(15) << p->category
             << setw(12) << p->status << "\n";
    }
    cout << "\n";
}


int main() {
    books   = new Book[bookCapacity];
    members = new Member[memberCapacity];

    int selection = 0;

    do {
        cout << "\n===== UNIVERSITY LIBRARY SYSTEM (Day " << systemDay << ") =====\n";
        cout << "1. Register Book\n";
        cout << "2. Register Member\n";
        cout << "3. Search Book\n";
        cout << "4. Borrow Book\n";
        cout << "5. Return Book\n";
        cout << "6. Display Books\n";
        cout << "7. Exit\n";

        selection = readInt("Select an option: ");

        if (selection == 1) {
            registerBook();
        } else if (selection == 2) {
            registerMember();
        } else if (selection == 3) {
            searchBook();
        } else if (selection == 4) {
            borrowBook();
        } else if (selection == 5) {
            returnBook();
        } else if (selection == 6) {
            displayBooks();
        } else if (selection == 7) {
            cout << "Exiting the University Library System. Goodbye!\n";
        } else {
            cout << "Invalid selection. Please choose 1-7.\n";
        }

    } while (selection != 7);


    delete[] books;
    delete[] members;
    books = nullptr;
    members = nullptr;

    return 0;
}
