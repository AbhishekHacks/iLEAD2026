#include <bits/stdc++.h>
using namespace std;

// ---- Interface for Playable items ----
class Playable {
public:
    virtual void play() const = 0;
    virtual ~Playable() = default;
};

// ---- Abstract Base Class ----
class LibraryItem {
protected:
    int id;
    string title;
    string author;
    bool available = true;

public:
    LibraryItem(int id, string title, string author)
        : id(id), title(move(title)), author(move(author)) {}

    virtual ~LibraryItem() = default;

    bool isAvailable() const { return available; }

    virtual void borrowItem(const string& duration, map<int, double>& fines) {
        if (!available) {
            cout << "Item already borrowed: " << title << "\n";
            return;
        }

        try {
            int days = stoi(duration);
            double fine = days * 10.0; // 10 rs/day
            fines[id] = fine;
            available = false;
            cout << title << " borrowed for " << days << " days.\n";
        } catch (const invalid_argument&) {
            cerr << "Invalid duration format for item " << title << ". Must be a number.\n";
        }
    }

    virtual void returnItem() {
        if (available) {
            cout << "Item was not borrowed: " << title << "\n";
            return;
        }
        available = true;
        cout << title << " returned.\n";
    }

    virtual void getDetails() const = 0; // pure virtual
};

// ---- Subclasses ----
class Book : public LibraryItem {
    int pageCount;
public:
    Book(int id, string title, string author, int pageCount)
        : LibraryItem(id, move(title), move(author)), pageCount(pageCount) {}

    int getPageCount() const { return pageCount; }

    void getDetails() const override {
        cout << "Book: " << title << " by " << author
             << ", Pages: " << pageCount << "\n";
    }
};

class AudioBook : public LibraryItem, public Playable {
    double duration; // in hours
public:
    AudioBook(int id, string title, string author, double duration)
        : LibraryItem(id, move(title), move(author)), duration(duration) {}

    void play() const override {
        cout << "Playing audiobook: " << title
             << " (" << duration << " hrs)\n";
    }

    void getDetails() const override {
        cout << "Audiobook: " << title << " by " << author
             << ", Duration: " << duration << " hrs\n";
    }
};

class EMagazine : public LibraryItem {
    int issueNumber;
public:
    EMagazine(int id, string title, string author, int issueNumber)
        : LibraryItem(id, move(title), move(author)), issueNumber(issueNumber) {}

    void archiveIssue() const {
        cout << "Archiving issue #" << issueNumber
             << " of " << title << "\n";
    }

    void getDetails() const override {
        cout << "E-Magazine: " << title
             << " Issue: " << issueNumber << "\n";
    }
};

// ---- Library System Manager ----
class LibrarySystem {
    vector<unique_ptr<LibraryItem>> items;
    map<int, double> fines;

public:
    void addItem(unique_ptr<LibraryItem> item) {
        items.push_back(move(item));
    }

    void searchByType(const string& typeName) const {
        for (const auto& item : items) {
            if (typeid(*item).name() == typeName) {
                item->getDetails();
            }
        }
    }

    void showFines() const {
        cout << "Outstanding fines:\n";
        for (const auto& entry : fines) {
            cout << "Item ID " << entry.first
                 << ": Rs " << entry.second << "\n";
        }
    }

    map<int, double>& getFines() { return fines; }
};

// ---- Demo ----
int main() {
    LibrarySystem library;

    auto book = make_unique<Book>(1, "Clean Code", "Robert C. Martin", 464);
    auto audioBook = make_unique<AudioBook>(2, "Atomic Habits", "James Clear", 8.5);
    auto magazine = make_unique<EMagazine>(3, "Tech Today", "Editorial", 42);

    library.addItem(move(book));
    library.addItem(move(audioBook));
    library.addItem(move(magazine));

    cout << "\n--- Borrowing ---\n";
    Book b(1, "Clean Code", "Robert C. Martin", 464);
    b.borrowItem("5", library.getFines());   // valid
    AudioBook a(2, "Atomic Habits", "James Clear", 8.5);
    a.borrowItem("abc", library.getFines()); // invalid
    EMagazine m(3, "Tech Today", "Editorial", 42);
    m.borrowItem("7", library.getFines());

    cout << "\n--- Playing & Archiving ---\n";
    a.play();
    m.archiveIssue();

    cout << "\n--- Fines ---\n";
    library.showFines();

    cout << "\n--- Return ---\n";
    b.returnItem();

    return 0;
}
