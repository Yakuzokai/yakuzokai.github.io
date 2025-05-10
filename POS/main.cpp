#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <ctime>
#include <iomanip>
#include <cctype>


using namespace std;

class Product{
    public:
        string name;
        double price;
        int stock;

        Product(string n, double p, int s) :
        name(n), price(p), stock(s){}
};

class Transaction{
    public:
        string productName;
        int quantity;
        double total_price;
        time_t time_stamp;

        Transaction(string name, int qty, double price) :
        productName(name), quantity(qty), total_price(price){
            time_stamp = time(0);
        }
};

class User{
    public:
        string u_name;
        string p_word;
        bool isAdmin;

        User( string u, string p, bool admin) :
            u_name(u), p_word(p), isAdmin(admin){}
};

class Receipt{
    public:
        vector<Transaction> items;
        time_t store_time;
        int id;
        double total_amount;
        static int next_id;

        Receipt() : store_time(time(0)), total_amount(0.0){
            id = next_id++;
        }

        void addTransaction(const Transaction& transaction){
            items.push_back(transaction);
            total_amount += transaction.total_price;
        }

        string getTime() const {
            return ctime(&store_time);
        }
};


int Receipt::next_id = 1;

class PosSystem{
    private:
        vector<Product> products;
        vector<Transaction> transactions;
        vector<Receipt> receipts;
        vector<User> users;
        User* current_user;

         void printReceipt(const Receipt& receipt){
          cout << "==================== Reciept #"<< receipt.id << " ====================" << endl;
          cout << left << setw(20) << "Item " << setw(10) << "Quantity" << setw(15) <<"Price" << endl;
          cout << "-------------------------------------------------" << endl;

          double totalOrderPrice = 0;
          for (const auto& transaction : receipt.items) {
            cout << left << setw(20) << transaction.productName <<
            setw(10) << transaction.quantity << "P" << fixed << setprecision(2) <<
                    transaction.total_price << endl;
                totalOrderPrice += transaction.total_price;
          }
          cout << "-------------------------------------------------" << endl;
          cout << "Total: P" << fixed << setprecision(2) << receipt.total_amount << endl;
          cout << "Time: " << receipt.getTime();
          cout << "==================== ======== ====================" << endl;

        }

        void processPayment (double amount){
            system("cls");
            double payment;
            cout << "Please Pay: P" << amount << endl;
            cout << "Enter amount: "; cin >> payment;
             if(cin.fail() || payment <= 0){
                cout << "Invalid input! Please try again!" << endl;
                Sleep(1000);
                cin.clear();
                cin.ignore(1000, '\n');
                processPayment(amount);
             }else if (payment >= amount){
                cout << "Payment Successful! Thank you!" << endl;
                if(payment > amount){
                    cout << "Amount Paid: " << amount << endl;
                    cout << "Change: P" << (payment - amount) << endl;
                }
             }
             else{
                cout << "Insufficient Payment! Please Try again!" << endl;
                Sleep(2000);
                processPayment(amount);
             }
            }

        string toLowerCase(const string& str){
            string lowered;
            for (char ch : str){
                lowered += tolower(ch);
            }
            return lowered;
        }

public:
    PosSystem(){

        users.push_back(User("Manager", "mgr123", true));
        current_user = nullptr;

        products.push_back(Product("Apple", 15, 100));
        products.push_back(Product("Apple Green Juice", 10, 100));
        products.push_back(Product("Banana", 20, 100));
        products.push_back(Product("Orange", 20, 100));
        products.push_back(Product("Watermelon",50 , 100));
        products.push_back(Product("Potato", 15, 100));



    }


    void registration(){
        if (!current_user->isAdmin){
            cout << "Access Denied!" << endl;
            Sleep(2000);
            return;
        }
        system("cls");
        cout << "============================================" << endl;
        cout << "=============== Registration ===============" << endl;
        cout << "============================================" << endl;

        string name, password;

        bool userExist = false;
        do{
        userExist = false;
        cout << "Enter a username for employee: "; cin >> name;
            for(const auto& user : users){
            if(user.u_name == name){
                userExist = true;
                cout << "Username is already exist!" << endl;
                Sleep(2000);
                system("cls");
                cout << "============================================" << endl;
                cout << "=============== Registration ===============" << endl;
                cout << "============================================" << endl;
                break;
                }
            }
        }while(userExist);

        cout << "Enter password: "; cin >> password;

        while(password.length() < 6){
                cout << "Password is must be at least 6 character or number long!" << endl;
                Sleep(2000);
                if(password.empty()){
                cout << "Password cannot be empty." << endl;
                Sleep(2000);
            }
        cout << "Enter password: "; cin >> password;
        }
        users.push_back(User(name, password, false));
        cout << "User added successfully! Please login again with the new user credentials" << endl;
        Sleep(2000);
        system("pause");
        system("cls");

        current_user = nullptr;
        while (!login()){
            cout << "Invalid Credentials! Please try again...." << endl;
            Sleep(2000);
            system("cls");
        }
    }


    bool login(){

        cout << "==============================" << endl;
        cout << "==========   LOGIN  ==========" << endl;
        cout << "==============================" << endl;
        string username, password;

        cout << "Enter username: "; cin >> username;
        cout << "Enter password: "; cin >> password;

        for (auto& user : users){
            if (user.u_name == username && user.p_word == password) {
                current_user = &user;
                return true;
            }
        }
        return false;
    }



    void showMenu(){
        system("cls");
        cout << "=============================" << endl;
        cout << "==========   MENU   =========" << endl;
        cout << "=============================" << endl;
        cout << "1. Order" << endl;
        cout << "2. Puchase History" << endl;
        if (current_user->isAdmin){
            cout << "3. Profit Data" << endl;
            cout << "4. Stocks" << endl;
            cout << "5. Add Product" << endl;
            cout << "6. Register a employee" << endl;
        }
        cout << "7. Logout" << endl;
        cout << "8. Exit" << endl;
    }

    void orderProduct(){

        vector<Transaction> currentOrder;
        double totalOrderPrice = 0;
        string addMore;
        system("cls");

do{
    showProductMenu();
    string product_name;

    cout << "Type '0' if you want to cancel the all order(s)" << endl;
    cout << "Type 'c' if you want to proceed to payment or if the cart is empty return to menu " << endl;
    cout << endl;
    cout  << "Enter Product Name: "; cin.ignore(); getline(cin, product_name);

    system("cls");
    showProductMenu();
    cout << "Selected Product: " << product_name << endl;
    if(product_name == "C" || product_name == "c"){
        break;
        } else if(product_name == "0"){
            for (auto& item : currentOrder) {
                for (auto& product : products) {
                    if (product.name == item.productName) {
                        product.stock += item.quantity;
                        break;
                        }
                    }
                }
                return;
        }
        bool productFound = false;
            for (auto& product : products){
                if(toLowerCase(product.name) == toLowerCase(product_name)){
                    productFound = true;

                    cout << "Available Stock: '" << product.stock << "' units"  << endl;
                    int quantity;

                    if(product.stock == 0){
                        cout << "No Available stocks" << endl;
                        break;
                    }

                    cout << "Enter Quantity: "; cin >> quantity;
                    while(cin.fail() || quantity <= 0){
                        cout << "Invalid input! Please try again!" << endl;
                        Sleep(2000);
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Enter Quantity: "; cin >> quantity;
                    }

                    while(product.stock < quantity){
                        cout << "Insufficient stock! Only " << product.stock << " units left." << endl;
                        Sleep(1000);
                        cout << "Enter Quantity: "; cin >> quantity;
                     }

                     string confirm;
                     cout << "Confirm Order? (Y/N): "; cin >> confirm;
                     while(cin.fail() || true || toLowerCase(confirm) != "y" ){
                         if( toLowerCase(confirm) == "y"){
                                break;
                            }else if(toLowerCase(confirm)== "n"){
                                cout << "Order Cancelled!" << endl;
                                Sleep(2000);
                                for (auto& item : currentOrder) {
                                    for(auto& product : products) {
                                    if(product.name == item.productName) {
                                        product.stock += item.quantity;
                                break;
                                }
                            }
                            cin.clear();
                        }
                        return;
                    }           else{
                                    cout << "Invalid input! Please try again!" << endl;
                                    Sleep(2000);
                                    cout << "Confirm Order? (Y/N): "; cin >> confirm;
                                }
                            }

                     product.stock -= quantity;
                     double totalPrice = product.price * quantity;
                     totalOrderPrice += totalPrice;
                     currentOrder.push_back(Transaction(product.name, quantity, totalPrice));
                     cout << quantity << " Unit(s) of " << product.name << " added to the order" << endl;
                     Sleep(1000);
                     break;
                }
            }
            if(!productFound){
                cout << "Item '" << product_name << "' not found" << endl;
                Sleep(2000);
            }
            cout << "Do you want to add product again? (Y/N): "; cin >> addMore;

                        while(cin.fail() || true){
                            if( toLowerCase(addMore) == "y"){
                                system("cls");
                                break;
                            }
                                if(toLowerCase(addMore)== "n"){
                                    break;
                                for (auto& item : currentOrder) {
                                    for(auto& product : products) {
                                    if(product.name == item.productName) {
                                        product.stock += item.quantity;
                                break;
                                }
                            }
                            cin.clear();
                        }
                        return;
                    }           else{
                                    cout << "Invalid input! Please try again!" << endl;
                                    Sleep(2000);
                                    cout << "Do you want to add product again? (Y/N): "; cin >> addMore;
                                }
                            }


        }while(addMore == "Y" || addMore == "y");

                    if (!currentOrder.empty()){
                            cout << "Total order price: P" << fixed << setprecision(2) << totalOrderPrice << endl;
                            Sleep(2000);
                            processPayment(totalOrderPrice);

                            Receipt newReceipt;
                            for(const auto& transaction : currentOrder){
                                newReceipt.addTransaction(transaction);
                            }

                            printReceipt(newReceipt);
                            receipts.push_back(newReceipt);


                        cout << "Order Completed! Thank You! Please Come Again!" << endl;
                        system("pause");
                        system("cls");
                    }

                    cout << "Returning to main menu...." << endl;
                    Sleep(2000);
                    system("cls");
                    return;
        }



    void showProductMenu(){
        system("cls");
        cout << "=============== Products MENU ==============" << endl;
        for (const auto& product : products){
        cout << left << setw(20) <<  product.name << " | " <<  setw(10) << " - each for P" << product.price  <<  setw(10) << " - Stocks: " << product.stock << endl;
        }
        cout << "=============== ============= ==============" << endl;
        }

    void showPurchaseHistory(){
        system("cls");
        cout << "=============  PURCHASE HISTORY ============" << endl;
            if(receipts.empty()){
            cout << "No transaction yet" << endl;
            system("pause");
            return;
        }
        for (const auto& receipt : receipts){
            printReceipt(receipt);
            cout << endl;
        }
        system("pause");
        cout << "=============  ================ ============" << endl;
    }


    void showProfitData(){
        if(!current_user->isAdmin){
            cout << "Access Denied!" << endl;
            Sleep(2000);
            return;
        }
        system("cls");
        cout << "==============  PROFIT HISTORY =============" << endl;
        double total_profit = 0;
        if(receipts.empty()){
            cout << "No transaction yet" << endl;
            system("pause");
            return;
        }
        for (const auto& receipt : receipts){
            total_profit += receipt.total_amount;
        }
        cout << "Total Profit: P" << fixed << setprecision(2) << total_profit  << endl;
        cout << "==============  ============== =============" << endl;
            system("pause");
            system("cls");
            cout << "Returning to main menu...." << endl;
            Sleep(2000);
            system("cls");
            return;

    }

    void showStocks(){
        if(!current_user->isAdmin){
            cout << "Access Denied!" << endl;
            Sleep(2000);
            return;
        }
        string addMore;

            system("cls");
            cout << "================= STOCKS ================" << endl;
            for (const auto& product : products){
                    cout << left << setw(20) <<  product.name << " | " << setw(10) << " - Stocks: " << product.stock << endl;
        }
        cout << "================= ===== ================" << endl;
        cout << "Do u want to restock products? (Y/N): "; cin >> addMore;


            while(cin.fail() || true){
                if( toLowerCase(addMore) == "y"){
                string product_name;
                cout << "Enter Product Name: "; cin.ignore(); getline(cin, product_name);

                bool productFound = false;

                for (auto& product : products){
                int quantity;
                if (toLowerCase(product.name) == toLowerCase(product_name)){
                    productFound = true;
                    cout << "Enter Quantity to restock: "; cin >> quantity;
                while(cin.fail() || quantity <= 0){
                        cout << "Invalid input! Please try again!" << endl;
                        Sleep(2000);
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Enter Quantity to restock: "; cin >> quantity;
                    }
                    product.stock += quantity;
                    cout << "Product Restocked Successfully!" << endl;
                    Sleep(2000);
                    system("cls");
                    return;
                }
            }
            if(!productFound){
                system("cls");
                cout << "Product not Found!" << endl;
                Sleep(2000);
                cout << "Returning to the main menu" << endl;
                Sleep(2000);
                system("cls");
                return;
                }
                cin.clear();
            }
        if(toLowerCase(addMore)== "n"){
        return;
    }

    else {
        cout << "Returning to main menu..." << endl;
        Sleep(2000);
        system("cls");
        showMenu();
    }
        }
}




    void addProduct(){
        if (!current_user->isAdmin){
            cout << "Access Denied!" << endl;
            Sleep(2000);
            return;
        }
        system("cls");
        cout << "================= ADD PRODUCT ================" << endl;
        string name;
        double price;
        int stock;

        bool productExist = false;
        do{
        productExist = false;
       cout << "Enter Product Name: "; cin.ignore(); getline(cin, name);
            for(const auto& product : products){
            if(toLowerCase(product.name) == toLowerCase(name)){
                productExist = true;
                cout << "Product is already exist!" << endl;
                Sleep(2000);
                system("cls");
                cout << "================= ADD PRODUCT ================" << endl;
                break;
                }
            }
        }while(productExist);



        cout << "Enter Product Price: "; cin >> price;

        while(cin.fail() || price <= 0){
            cout << "Invalid input! Please try again!" << endl;
            Sleep(2000);
            cin.clear();
            cin.ignore(10000, '\n');
             cout << "Enter Product Price: "; cin >> price;
        }

        cout << "Enter Product Stock: "; cin >> stock;

        while(cin.fail() || stock <= 0){
            cout << "Invalid input! Please try again!" << endl;
            Sleep(2000);
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Enter Product Stock: "; cin >> stock;
        }


        products.push_back(Product(name, price, stock));
        cout << "Product Added Successfully!" << endl;
        Sleep(2000);
        system("cls");

    }

    void logout(){
        current_user = nullptr;
        system("cls");
        cout << "Logging out...." << endl;
        Sleep(2000);
        system("cls");
        while (!login()){
            cout << "Invalid Credentials! Please try again...." << endl;
            Sleep(2000);
            system("cls");
        }
    }

};

int main(){
    PosSystem pos;
        while (!pos.login()){
            cout << "Invalid Credentials! Please try again...." << endl;
            Sleep(2000);
            system("cls");
        }

 while(true){
    int choice;
        pos.showMenu();
        cout << "Enter your choice: "; cin >> choice;
        switch (choice){
            case 1:
                pos.orderProduct();
                break;
            case 2:
                pos.showPurchaseHistory();
                break;
            case 3:
                pos.showProfitData();
                break;
            case 4:
                pos.showStocks();
                break;
            case 5:
                pos.addProduct();
                break;
            case 6:
                pos.registration();
                break;
            case 7:
                pos.logout();
                break;
            case 8:
                cout << "Exiting Program..." << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again!" << endl;
                if(cin.fail() || choice != 8){
                    Sleep(2000);
                    cin.clear();
                    cin.ignore(10000, '\n');
                    }
                    break;
                }
            }
        }
