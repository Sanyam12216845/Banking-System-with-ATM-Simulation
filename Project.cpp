#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

class Account {
protected:
    int accountNo;
    float balance;
public:
    Account(int accNo, float bal) : accountNo(accNo), balance(bal) {}
    void deposit(float amount) {
        balance += amount;
    }
    void withdraw(float amount) {
        if (amount <= balance) {
            balance -= amount;
        } else {
            cout << "Insufficient balance." << endl;
        }
    }
    float getBalance() const {
        return balance;
    }
    int getAccountNo() const {
        return accountNo;
    }
};

class SavingsAccount : public Account {
private:
    float interestRate;
public:
    SavingsAccount(int accNo, float bal, float rate) : Account(accNo, bal), interestRate(rate) {}
    void calculateInterest() {
        balance += balance * (interestRate / 100);
    }
};

class CheckingAccount : public Account {
private:
    float overdraftLimit;
public:
    CheckingAccount(int accNo, float bal, float limit) : Account(accNo, bal), overdraftLimit(limit) {}
    void checkOverdraft(float amount) {
        if (balance + overdraftLimit >= amount) {
            balance -= amount;
        } else {
            cout << "Overdraft limit exceeded." << endl;
        }
    }
};

class Customer {
private:
    string name;
    vector<Account*> accounts;
public:
    Customer(string n) : name(n) {}
    void addAccount(Account* acc) {
        accounts.push_back(acc);
    }
    vector<Account*> getAccounts() const {
        return accounts;
    }
    string getName() const {
        return name;
    }
};

class Transaction {
protected:
    time_t date;
    float amount;
public:
    Transaction(float amt) : amount(amt) {
        date = time(0);
    }
    virtual void execute() = 0;
    time_t getDate() const {
        return date;
    }
    float getAmount() const {
        return amount;
    }
};

class Deposit : public Transaction {
private:
    Account* account;
public:
    Deposit(Account* acc, float amt) : Transaction(amt), account(acc) {}
    void execute() override {
        account->deposit(amount);
    }
    Account* getAccount() const {
        return account;
    }
};

class Withdrawal : public Transaction {
private:
    Account* account;
public:
    Withdrawal(Account* acc, float amt) : Transaction(amt), account(acc) {}
    void execute() override {
        account->withdraw(amount);
    }
    Account* getAccount() const {
        return account;
    }
};

class ATM {
public:
    bool validate(const Customer& customer, int accountNo) {
        for (auto acc : customer.getAccounts()) {
            if (acc->getAccountNo() == accountNo) {
                return true;
            }
        }
        return false;
    }

    bool verify(const Customer& customer) {

        return true;
    }

    bool authenticate(const Customer& customer) {

        return true;
    }

    void getReceipt(const Transaction& transaction, const Account& account) {
        cout << "Receipt:" << endl;
        time_t date = transaction.getDate();  
        cout << "Date: " << ctime(&date);
        cout << "Account Number: " << account.getAccountNo() << endl;
        cout << "Amount: " << transaction.getAmount() << endl;
        cout << "New Balance: " << account.getBalance() << endl;
    }

    void printReceipt(const Transaction& transaction, const Account& account) {
        getReceipt(transaction, account);
    }
};

int main() {
    string name;
    cout << "Enter customer name: ";
    cin >> name;
    Customer customer(name);

    int accountNo;
    float initialBalance;
    cout << "Enter savings account number: ";
    cin >> accountNo;
    cout << "Enter initial balance for savings account: ";
    cin >> initialBalance;
    SavingsAccount* savings = new SavingsAccount(accountNo, initialBalance, 3.0);
    customer.addAccount(savings);

    cout << "Enter checking account number: ";
    cin >> accountNo;
    cout << "Enter initial balance for checking account: ";
    cin >> initialBalance;
    CheckingAccount* checking = new CheckingAccount(accountNo, initialBalance, 200.0);
    customer.addAccount(checking);

    ATM atm;
    int choice;
    float amount;
    while (true) {
        cout << "\n1. Deposit\n2. Withdraw\n3. Check Balance\n4. Exit\nChoose an option: ";
        cin >> choice;
        if (choice == 4) break;
        cout << "Enter account number: ";
        cin >> accountNo;
        if (!atm.validate(customer, accountNo)) {
            cout << "Account not found." << endl;
            continue;
        }
        if (!atm.verify(customer) || !atm.authenticate(customer)) {
            cout << "Verification or authentication failed." << endl;
            continue;
        }
        Account* account = nullptr;
        for (auto acc : customer.getAccounts()) {
            if (acc->getAccountNo() == accountNo) {
                account = acc;
                break;
            }
        }
        if (!account) {
            cout << "Account not found." << endl;
            continue;
        }
        switch (choice) {
            case 1:
                cout << "Enter amount to deposit: ";
                cin >> amount;
                {
                    Deposit deposit(account, amount);
                    deposit.execute();
                    atm.printReceipt(deposit, *account);
                }
                break;
            case 2:
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                {
                    Withdrawal withdrawal(account, amount);
                    withdrawal.execute();
                    atm.printReceipt(withdrawal, *account);
                }
                break;
            case 3:
                cout << "Balance: " << account->getBalance() << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    }

    delete savings;
    delete checking;

    return 0;
}
