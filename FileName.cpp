#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const string MANAGER_PASSWORD = "1234";
const string ITEMS_FILE = "items.txt";
const string RECEIPTS_FILE = "receipts.txt";

struct Item {
    string name;
    double price;
};

struct SnackBar {
    Item items[10];
    int itemCount = 0;
};
void printVendingMachine() {
    cout << "        ___________________\n"
        << "       |  _______________  |\n"
        << "       | |               | |\n"
        << "       | | Vending Machine | |\n"
        << "       | |_______________| |\n"
        << "       |  _ _ _ _ _ _ _ _  |\n"
        << "       | |               | |\n"
        << "       | | [ ] [ ] [ ] [ ] | |\n"
        << "       | |_______________| |\n"
        << "       |  _ _ _ _ _ _ _ _  |\n"
        << "       | |               | |\n"
        << "       | | [ ] [ ] [ ] [ ] | |\n"
        << "       | |_______________| |\n"
        << "       |  _ _ _ _ _ _ _ _  |\n"
        << "       | |               | |\n"
        << "       | | [ ] [ ] [ ] [ ] | |\n"
        << "       | |_______________| |\n"
        << "       |  _ _ _ _ _ _ _ _  |\n"
        << "       | |               | |\n"
        << "       | | [ ] [ ] [ ] [ ] | |\n"
        << "       | |_______________| |\n"
        << "       |  _ _ _ _ _ _ _ _  |\n"
        << "       | |               | |\n"
        << "       | | [ ] [ ] [ ] [ ] | |\n"
        << "       | |_______________| |\n"
        << "       |___________________|\n";
}

void printWelcome();
int getUserInput();
void readItemsFromFile(SnackBar& snack);
void writeItemsToFile(const SnackBar& snack);
void getOrders(SnackBar& snack);
void viewReceipts();
bool authenticateManager();
void managerMenu(SnackBar& snack);
void updateItem(SnackBar& snack);
void addItem(SnackBar& snack);
void deleteItem(SnackBar& snack);

int main() {
    SnackBar snack;
    printVendingMachine();
    printWelcome();
    readItemsFromFile(snack);

    int userMenuSelect;
    do {
        userMenuSelect = getUserInput();

        switch (userMenuSelect) {
        case 1:
            getOrders(snack);
            break;
        case 2:
            viewReceipts();
            break;
        case 3:
            cout << "Thank you for using the True Grit Snack Bar" << endl;
            break;
        case 4:
            if (authenticateManager()) {
                managerMenu(snack);
            }
            else {
                cout << "Incorrect password. Access denied." << endl;
            }
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (userMenuSelect != 3);

    return 0;
}

void printWelcome() {
    cout << "Welcome to the True Grit Snack Bar" << endl;
    cout << "The item list was successfully loaded" << endl;
}

int getUserInput() {
    int choice;
    cout << "What would you like to do:" << endl;
    cout << "1. New Order" << endl;
    cout << "2. View Receipts" << endl;
    cout << "3. Exit" << endl;
    cout << "4. Manager Menu" << endl;
    cin >> choice;
    return choice;
}

void readItemsFromFile(SnackBar& snack) {
    ifstream inputFile(ITEMS_FILE);
    if (!inputFile.is_open()) {
        ofstream createFile(ITEMS_FILE); // Create the file if it doesn't exist
        createFile.close();
        return;
    }

    while (inputFile >> snack.items[snack.itemCount].name >> snack.items[snack.itemCount].price) {
        snack.itemCount++;
        if (snack.itemCount >= 10) {
            break; // Prevent exceeding item limit
        }
    }

    inputFile.close();
}

void writeItemsToFile(const SnackBar& snack) {
    ofstream outputFile(ITEMS_FILE); // Open in overwrite mode
    if (!outputFile.is_open()) {
        cout << "Error writing to file '" << ITEMS_FILE << "'" << endl;
        return;
    }

    for (int i = 0; i < snack.itemCount; i++) {
        outputFile << snack.items[i].name << " " << snack.items[i].price << endl;
    }

    outputFile.close();
}

void getOrders(SnackBar& snack) {
    cout << "Available items:" << endl;
    for (int i = 0; i < snack.itemCount; i++) {
        cout << i + 1 << ". " << snack.items[i].name << " - $" << fixed << setprecision(2) << snack.items[i].price << endl;
    }

    cout << "Enter item number to order (0 to finish): ";
    int itemNumber;
    ofstream receiptFile(RECEIPTS_FILE, ios::app);
    if (!receiptFile.is_open()) {
        cout << "Error writing to file '" << RECEIPTS_FILE << "'" << endl;
        return;
    }

    while (cin >> itemNumber && itemNumber != 0) {
        if (itemNumber >= 1 && itemNumber <= snack.itemCount) {
            receiptFile << snack.items[itemNumber - 1].name << " $" << fixed << setprecision(2) << snack.items[itemNumber - 1].price << endl;
            cout << "Item added to receipt." << endl;
        }
        else {
            cout << "Invalid item number." << endl;
        }
        cout << "Enter next item number to order (0 to finish): ";
    }

    receiptFile.close();
}

void viewReceipts() {
    ifstream receiptFile(RECEIPTS_FILE);
    if (!receiptFile.is_open()) {
        cout << "No receipts available." << endl;
        return;
    }

    string line;
    cout << "Receipts:" << endl;
    while (getline(receiptFile, line)) {
        cout << line << endl;
    }

    receiptFile.close();
}

bool authenticateManager() {
    string password;
    cout << "Enter manager password: ";
    cin >> password;
    return password == MANAGER_PASSWORD;
}

void managerMenu(SnackBar& snack) {
    int choice;
    do {
        cout << "Manager Menu:" << endl;
        cout << "1. Add Item" << endl;
        cout << "2. Update Item" << endl;
        cout << "3. Delete Item" << endl;
        cout << "4. Exit" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            addItem(snack);
            break;
        case 2:
            updateItem(snack);
            break;
        case 3:
            deleteItem(snack);
            break;
        case 4:
            cout << "Exiting manager menu." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);
}

void updateItem(SnackBar& snack) {
    cout << "Enter the item number to update: ";
    int itemNumber;
    cin >> itemNumber;

    if (itemNumber >= 1 && itemNumber <= snack.itemCount) {
        cout << "Enter new name and price: ";
        cin >> snack.items[itemNumber - 1].name >> snack.items[itemNumber - 1].price;
        cout << "Item updated successfully." << endl;

        // Write the updated items to the items file
        writeItemsToFile(snack);
    }
    else {
        cout << "Invalid item number." << endl;
    }
}

void addItem(SnackBar& snack) {
    if (snack.itemCount < 10) {
        cout << "Enter item name and price: ";
        cin >> snack.items[snack.itemCount].name >> snack.items[snack.itemCount].price;
        snack.itemCount++;
        cout << "Item added successfully." << endl;

        // Write the new item to the items file
        writeItemsToFile(snack);
    }
    else {
        cout << "Maximum item limit reached." << endl;
    }
}

void deleteItem(SnackBar& snack) {
    cout << "Enter the item number to delete: ";
    int itemNumber;
    cin >> itemNumber;

    if (itemNumber >= 1 && itemNumber <= snack.itemCount) {
        // Shift items to the left to overwrite the deleted item
        for (int i = itemNumber - 1; i < snack.itemCount - 1; i++) {
            snack.items[i] = snack.items[i + 1];
        }
        snack.itemCount--;
        cout << "Item deleted successfully." << endl;

        // Write the updated items to the items file
        writeItemsToFile(snack);
    }
    else {
        cout << "Invalid item number." << endl;
    }
}
