#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdio>
using namespace std;

class BankAccount {
private:
    int accountNumber;
    string name;
    double balance;

public:
    BankAccount() : accountNumber(0), name(""), balance(0.0) {}

    BankAccount(int accNo, string customerName, double initialBalance)
        : accountNumber(accNo), name(customerName), balance(initialBalance) {}

    int getAccountNumber() const {
        return accountNumber;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount!\n";
            return;
        }
        balance += amount;
        cout << "Amount deposited successfully.\n";
        cout << "Current Balance: Rs. " << fixed << setprecision(2)
             << balance << '\n';
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount!\n";
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient balance!\n";
            return false;
        }
        balance -= amount;
        cout << "Amount withdrawn successfully.\n";
        cout << "Current Balance: Rs. " << fixed << setprecision(2)
             << balance << '\n';
        return true;
    }

    void display() const {
        cout << "\n----------------------------------\n";
        cout << "Account Number : " << accountNumber << '\n';
        cout << "Customer Name  : " << name << '\n';
        cout << "Balance        : Rs. " << fixed << setprecision(2)
             << balance << '\n';
        cout << "----------------------------------\n";
    }

    void saveToFile(ofstream& file) const {
        file << accountNumber << '\n';
        file << name << '\n';
        file << balance << '\n';
    }

    bool loadFromFile(ifstream& file) {
        if (!(file >> accountNumber))
            return false;

        file.ignore();
        getline(file, name);
        file >> balance;
        return true;
    }
};

bool findAccount(int accountNumber, BankAccount& account) {
    ifstream file("accounts.txt");
    if (!file)
        return false;

    BankAccount temp;
    while (temp.loadFromFile(file)) {
        if (temp.getAccountNumber() == accountNumber) {
            account = temp;
            return true;
        }
    }
    return false;
}

void updateAccount(const BankAccount& updatedAccount) {
    ifstream inputFile("accounts.txt");
    ofstream tempFile("temp.txt");

    if (!inputFile || !tempFile) {
        cout << "Error updating account file!\n";
        return;
    }

    BankAccount account;
    while (account.loadFromFile(inputFile)) {
        if (account.getAccountNumber() == updatedAccount.getAccountNumber())
            updatedAccount.saveToFile(tempFile);
        else
            account.saveToFile(tempFile);
    }

    inputFile.close();
    tempFile.close();

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

void createAccount() {
    int accountNumber;
    string name;
    double initialBalance;

    cout << "\nEnter Account Number: ";
    cin >> accountNumber;

    BankAccount existing;
    if (findAccount(accountNumber, existing)) {
        cout << "Account already exists!\n";
        return;
    }

    cin.ignore();
    cout << "Enter Customer Name: ";
    getline(cin, name);

    cout << "Enter Initial Deposit: Rs. ";
    cin >> initialBalance;

    if (initialBalance < 0) {
        cout << "Invalid initial balance!\n";
        return;
    }

    BankAccount newAccount(accountNumber, name, initialBalance);

    ofstream file("accounts.txt", ios::app);
    if (!file) {
        cout << "Error opening account file!\n";
        return;
    }

    newAccount.saveToFile(file);
    file.close();

    cout << "\nAccount created successfully!\n";
    newAccount.display();
}

void depositMoney() {
    int accountNumber;
    double amount;

    cout << "\nEnter Account Number: ";
    cin >> accountNumber;

    BankAccount account;
    if (!findAccount(accountNumber, account)) {
        cout << "Account not found!\n";
        return;
    }

    cout << "Enter Deposit Amount: Rs. ";
    cin >> amount;

    account.deposit(amount);
    updateAccount(account);
}

void withdrawMoney() {
    int accountNumber;
    double amount;

    cout << "\nEnter Account Number: ";
    cin >> accountNumber;

    BankAccount account;
    if (!findAccount(accountNumber, account)) {
        cout << "Account not found!\n";
        return;
    }

    cout << "Enter Withdrawal Amount: Rs. ";
    cin >> amount;

    if (account.withdraw(amount))
        updateAccount(account);
}

void checkBalance() {
    int accountNumber;

    cout << "\nEnter Account Number: ";
    cin >> accountNumber;

    BankAccount account;
    if (!findAccount(accountNumber, account)) {
        cout << "Account not found!\n";
        return;
    }

    cout << "\nBalance Inquiry\n";
    account.display();
}

void displayAllAccounts() {
    ifstream file("accounts.txt");

    if (!file) {
        cout << "\nNo account records found.\n";
        return;
    }

    BankAccount account;
    bool found = false;

    cout << "\n========== ALL BANK ACCOUNTS ==========\n";

    while (account.loadFromFile(file)) {
        account.display();
        found = true;
    }

    if (!found)
        cout << "No accounts available.\n";

    file.close();
}

int main() {
    int choice;

    do {
        cout << "\n========================================\n";
        cout << "       BANK MANAGEMENT APPLICATION\n";
        cout << "========================================\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Check Balance\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Exit\n";
        cout << "========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: displayAllAccounts(); break;
            case 6:
                cout << "\nThank you for using the Bank Management System!\n";
                break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
