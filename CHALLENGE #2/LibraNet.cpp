#include <bits/stdc++.h>
using namespace std;

// Base class for common attributes
class LibraryItem {
protected:
    int id;
    string title;
    string author;
    bool available;

public:
    LibraryItem(int id, string title, string author)
        : id(id), title(title), author(author), available(true) {}

    virtual ~LibraryItem() {}

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }

    bool isAvailable() const { return available; }

    virtual void borrowItem(const string& durationStr) {
        if (available) {
            available = false;
            cout << title << " borrowed for " << durationStr << endl;
        } else {
            cout << title << " is not available right now." << endl;
        }
    }

    virtual void returnItem(int lateDays) {
        if (!available) {
            available = true;
            int fine = lateDays * 10; // Rs 10 per day fine
            if (fine > 0)
                cout << title << " returned with fine Rs " << fine << endl;
            else
                cout << title << " returned on time." << endl;
        } else {
            cout << title << " was not borrowed!" << endl;
        }
    }

    virtual void showDetails() const {
        cout << "ID: " << id
             << ", Title: " << title
             << ", Author: " << author
             << ", Available: " << (available ? "Yes" : "No") << endl;
    }
};

// Derived class: Book
class Book : public LibraryItem {
    int pageCount;

public:
    Book(int id, string title, string author, int pages)
        : LibraryItem(id, title, author), pageCount(pages) {}

    int getPageCount() const { return pageCount; }

    void showDetails() const override {
        LibraryItem::showDetails();
        cout << "Pages: " << pageCount << endl;
    }
};

// Interface
class Playable {
public:
    virtual void play() = 0;
    virtual ~Playable() {}
};

// Derived class: AudioBook
class AudioBook : public LibraryItem, public Playable {
    int duration; // in minutes

public:
    AudioBook(int id, string title, string author, int duration)
        : LibraryItem(id, title, author), duration(duration) {}

    void play() override {
        cout << "Playing audiobook: " << title
             << " (" << duration << " mins)" << endl;
    }

    void showDetails() const override {
        LibraryItem::showDetails();
        cout << "Duration: " << duration << " minutes" << endl;
    }
};

// Derived class: E-Magazine
class EMagazine : public LibraryItem {
    int issueNumber;

public:
    EMagazine(int id, string title, string author, int issue)
        : LibraryItem(id, title, author), issueNumber(issue) {}

    void archiveIssue() {
        cout << "Archiving issue #" << issueNumber
             << " of " << title << endl;
    }

    void showDetails() const override {
        LibraryItem::showDetails();
        cout << "Issue: " << issueNumber << endl;
    }
};

// Library class to manage everything
class Library {
    vector<LibraryItem*> items; // store pointers

public:
    ~Library() {
        for (auto item : items) delete item;
    }

    void addItem(LibraryItem* item) {
        items.push_back(item);
    }

    void showAllItems() {
        for (auto item : items) {
            item->showDetails();
            cout << "--------------------------" << endl;
        }
    }

    void borrowById(int id, string durationStr) {
        for (auto item : items) {
            if (item->getId() == id) {
                item->borrowItem(durationStr);
                return;
            }
        }
        cout << "Item with ID " << id << " not found." << endl;
    }

    void returnById(int id, int lateDays) {
        for (auto item : items) {
            if (item->getId() == id) {
                item->returnItem(lateDays);
                return;
            }
        }
        cout << "Item with ID " << id << " not found." << endl;
    }
};

// Main function
int main() {
    Library lib;

    // Add items
    lib.addItem(new Book(1, "C++ Basics", "Bjarne", 500));
    lib.addItem(new AudioBook(2, "Learn C++", "Herbert", 300));
    lib.addItem(new EMagazine(3, "Tech Today", "Editorial Team", 42));

    cout << "Library Items:" << endl;
    lib.showAllItems();

    cout << "Borrowing and returning demo:" << endl;
    lib.borrowById(1, "7 days");
    lib.returnById(1, 2); // returned 2 days late

    cout << "Testing audiobook play:" << endl;
    AudioBook* ab = new AudioBook(4, "AI Podcast", "John Doe", 120);
    ab->play();
    delete ab;

    cout << "Testing magazine archive:" << endl;
    EMagazine* em = new EMagazine(5, "Science Weekly", "Editors", 100);
    em->archiveIssue();
    delete em;

    return 0;
}
