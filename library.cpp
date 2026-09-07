#include <iostream>
#include <string>
using namespace std;

// Majahenkhosi Dlamini
class LibraryMember {
protected:
    int memberID;
    string name;
    int booksBorrowedCount;

public:
    LibraryMember(int id, string memberName)
        : memberID(id), name(memberName), booksBorrowedCount(0) {}

    virtual ~LibraryMember() {}

    int getMemberID() const { return memberID; }
    string getName() const { return name; }
    int getBooksBorrowedCount() const { return booksBorrowedCount; }

    void incrementBorrowedCount() { booksBorrowedCount++; }
    void decrementBorrowedCount() { if (booksBorrowedCount > 0) booksBorrowedCount--; }

    virtual int getBorrowLimit() const { return 2; }

    virtual double calculateMemberFine(int overdueDays) const {
        return overdueDays > 0 ? overdueDays * 1.0 : 0.0;
    }

    virtual string getMemberType() const { return "General"; }

    virtual void displayInfo() const {
        cout << "ID: " << memberID << " | Name: " << name
             << " | Type: " << getMemberType()
             << " | Borrow Limit: " << getBorrowLimit()
             << " | Currently Borrowed: " << booksBorrowedCount << endl;
    }
};

int main() {

    return 0;
}