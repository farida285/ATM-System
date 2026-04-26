#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std ;
const string clientsFileName = "BankClients.txt";
const string separator = "#//#";
struct stClient {
    string accountNumber;
    string  pinCode;
    string name;
    string phoneNumber;
    double accountBalance;
     bool markAsDelete = false;
};
stClient currentClient;
enum class  enOperation { nothing, quickWithdraw, normalWithdraw, deposit, checkBalance,logout };

unsigned short PositiveShortNumberInRange(const string& message, short from, short to) {
    int number;

    do {
        cout << message;
        if (!(cin >> number))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
        }
        else if (number<from || number>to) {
            cout << "Unavailable number!!\n";
            cout << "Enter number in range from " << from << " to " << to << ".\n";
        }
        else {
            return (unsigned short)number;
        }
    } while (true);

}
double  DoublePositiveNumber(const string& message) {
    double number;

    while (true) {
        cout << message;

        if (cin >> number) {
            if (number >= 0) {
                return number;
            }
            else {
                cout << "Invalid input!\nPlease enter a non-negative number:\n";
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\nPlease enter a number:\n";
        }
    }
}
long long GetAmount(const string& message) {
    double amount;
    do {
        amount = DoublePositiveNumber(message);
        if (amount == 0) {
            cout << "\nPlease enter a amount greater than zero.\n";
        }
        if(amount!=static_cast<long long>(amount)){
             cout << "\nPlease enter a integer amount.\n";

        }
    } while (amount == 0||amount!=static_cast<long long>(amount));
    return static_cast<long long>(amount);
}
char MyToLower(char letter) {
    if (letter >= 'A' && letter <= 'Z') {
        letter += 32;
    }
    return letter;
}
string LowerAllString(string text) {
    for (unsigned int i = 0; i < text.size(); i++) {
        if (text[i] != ' ') {
            text[i] = MyToLower(text[i]);
        }
    }
    return text;
}
bool YesOrNo(const string& message) {
    string Answer;
    cout << message << endl;
    do {
        cin >> Answer;
        Answer = LowerAllString(Answer);

        if (!(Answer == "yes" || Answer == "y" || Answer == "no" || Answer == "n"))
        {
            cout << "Please Enter yes or no : \n";
        }
    } while (!(Answer == "yes" || Answer == "y" || Answer == "no" || Answer == "n"));
    if (Answer == "yes" || Answer == "y") {
        return 1;
    }
    else {
        return 0;
    }


}
string  TrimLeft(const string& s1) {
    unsigned int size = s1.length();
    for (unsigned int i = 0; i < size; i++) {
        if (s1[i] != ' ') {
            return (s1.substr(i, size - i));
        }
    }
    return "";
}
string  TrimRight(const string& s1) {
    int size = (int)s1.length();
    for (int i = (int)size - 1; i >= 0; i--) {
        if (s1[i] != ' ') {
            return (s1.substr(0, i + 1));
        }
    }
    return "";

}
string  Trim(const string& s1) {

    return (TrimLeft(TrimRight(s1)));
}
string GetStringInSize(const string& message, unsigned short from = 1, unsigned short to = 6000, const string& errormessage = "Invalid Input!!\nPlease Try Again : ") {
    string text;
    unsigned size;
    cout << message;
    do {
        getline(cin >> ws, text);
        text = Trim(text);
        size = text.size();
        if (size > to || size < from) {
            cout << errormessage;
        }
    } while (size > to || size < from);
    return text;
}
vector <string> SplitString(string s1, const string& separator) {
    vector <string> words;
    size_t  position = 0;
    string word;
    while ((position = s1.find(separator)) != string::npos)
    {
        word = s1.substr(0, position);
        s1.erase(0, position + separator.size());
        if (word != "") {
            words.push_back(word);
        }
    }
    if (s1 != "") {
        words.push_back(s1);
    }
    return words;


}
stClient ConvertLineToRecord(const string& s, const string& separator) {
    vector <string> split = SplitString(s, separator);
    stClient client;
    client.accountNumber = split[0];
    client.pinCode = split[1];
    client.name = split[2];
    client.phoneNumber = split[3];
    client.accountBalance = stod(split[4]);
    return client;
}
string ConvertRecordToLine(const stClient& c1, const string& separator) {
    string s1 = c1.accountNumber + separator;
    s1 += c1.pinCode + separator;
    s1 += c1.name + separator;
    s1 += c1.phoneNumber + separator;
    s1 += to_string(c1.accountBalance);
    return s1;
}
vector <stClient> LoadclientsDataFromFile(const string& clientsFileName, const string& separator) {
    fstream file;
    string line;
    vector <stClient> vClients;
    file.open(clientsFileName, ios::in);
    if (file.is_open()) {
        while (getline(file, line))
        {
            vClients.push_back(ConvertLineToRecord(line, separator));
        }
        file.close();
    }
    return vClients;
}
void SaveUnMarkedVClientsInFile(const string& clientsFileName, const string& separator, const vector <stClient>& vClients) {
    fstream file;
    file.open(clientsFileName, ios::out);
    if (file.is_open()) {
        for (const stClient& c1 : vClients) {
            if (c1.markAsDelete == false) {
                file << ConvertRecordToLine(c1, separator) << endl;
            }
        }
        file.close();
    }
}
bool ClientIsExistsInFileByAccNum( string accountNumber, stClient& c1) {
    fstream file;
    string line;
    stClient c;
    file.open(clientsFileName, ios::in);
    if (file.is_open()) {
        while (getline(file, line))
        {
            c = ConvertLineToRecord(line, separator);
            if (c.accountNumber == accountNumber) {
                c1 = c;
                file.close();
                return 1;
            }
        }
        file.close();
    }
    return 0;
}
void  UpdateBalanceByAccNum( const string& accountNumber, double amount) {
    vector <stClient> vClients = LoadclientsDataFromFile(clientsFileName, separator);
    for (stClient& c1 : vClients) {
        if (c1.accountNumber == accountNumber) {
            c1.accountBalance += amount;
            break;
        }
    }
    SaveUnMarkedVClientsInFile(clientsFileName, separator, vClients);
}
short getQuickWithDrawAmount(short QuickWithDrawOption)
{
switch (QuickWithDrawOption)
{
case 1:
return 20;
case 2:
return 50;
case 3:
return 100;
case 4:
return 200;
case 5:
return 400;
case 6:
return 600;
case 7:
return 800;
case 8:
return 1000;
default:
return 0;
}
}
void ShowConfirmLogoutScreen(enOperation& oper) {
    system("cls");
    if (!YesOrNo("Are you sure to logout ? Y/N")) {
        oper = enOperation::nothing;
    }
}
void ShowCheckBalanceScreen (){
    system("cls");
    cout<<"=========================================\n";
    cout<<"             Balance Screen\n";
    cout<<"=========================================\n\n";
    cout<<"Your Balance is : "<<fixed<<setprecision(2)<<currentClient.accountBalance<<endl;
}
void ShowNormalWithdrawScreen(){
    system("cls");
    long long amount ;
    while (true){
      cout<<"=========================================\n";
      cout<<"             Normal Withdraw\n";
      cout<<"=========================================\n\n";
      do{
         amount =GetAmount("Enter an amount multiple of 5's :") ;
      }while((amount % 5)!=0);
      if(amount>currentClient.accountBalance){
        cout<<"The amount exceeds your balance, make another choice.\n";
        cout << "Press any key to continue...\n";
        system("pause > nul");
        continue;
      }else{
        break;
      }
    }
  if(YesOrNo("Are you sure you want perform this transaction ? Yes/No?")){
      UpdateBalanceByAccNum(currentClient.accountNumber, -1 * amount);
      currentClient.accountBalance-=amount;
      cout << "\nThe amount has been successfully withdrawn from the account.\n";
      cout << "New balance is : "<<fixed<<setprecision(2)<<currentClient.accountBalance<<endl;
  }

}
void ShowQuickWithdrawScreen(){
    system("cls");
    short amount ;
     while(true){ 
      cout<<"=========================================\n";
      cout<<"             Quick Withdraw\n";
      cout<<"=========================================\n";
      cout<<"       [1]20                  [2]50\n";
      cout<<"       [3]100                 [4]200\n";
      cout<<"       [5]400                 [6]600\n";
      cout<<"       [7]800                 [8]1000\n";
      cout<<"       [9]Exit\n";
      cout<<"=========================================\n";
      cout<<"Your Balance is : "<<fixed<<setprecision(2)<<currentClient.accountBalance<<endl;
      amount=getQuickWithDrawAmount(PositiveShortNumberInRange("What is your choice from [1] to [9]?\n", 1, 9));
      if(amount==0){
        return ;
      }
      if(amount>currentClient.accountBalance){
        cout<<"The amount exceeds your balance, make another choice.\n";
        cout << "Press any key to continue...\n";
        system("pause > nul");
        continue;
      }else{
        break;
      }
    }
      if(YesOrNo("Are you sure you want perform this transaction ? Yes/No?")){
      UpdateBalanceByAccNum(currentClient.accountNumber, -1 * amount);
      currentClient.accountBalance-=amount;
      cout << "\nThe amount has been successfully withdrawn from the account.\n";
      cout << "New balance is : "<<fixed<<setprecision(2)<<currentClient.accountBalance<<endl<<endl;
      }
    
}
void ShowDepositScreen(){
    system("cls");
    long long amount ;
     do{ 
      cout<<"==============================================\n";
      cout<<"                Deposit Screen\n";
      cout<<"==============================================\n\n";
      amount =GetAmount("Enter positive amount multiple of 5's deposit :") ;
    }while((amount % 5)!=0);
      if(YesOrNo("Are you sure you want perform this transaction ? Yes/No?")){
      UpdateBalanceByAccNum(currentClient.accountNumber, amount);
      currentClient.accountBalance+=amount;
      cout << "\nThe amount has been successfully added to the account.\n";
      cout << "New balance is : "<<fixed<<setprecision(2)<<currentClient.accountBalance<<endl<<endl;
      } 
}
void ExecuteOperation (enOperation &oper){
    
 switch (oper)
 {
 case enOperation::logout :
    ShowConfirmLogoutScreen(oper);
    break;
  case enOperation::checkBalance :
    ShowCheckBalanceScreen ();
    break;
  case enOperation::normalWithdraw :
    ShowNormalWithdrawScreen();
    break;
  case enOperation::quickWithdraw :
    ShowQuickWithdrawScreen();
    break;
  case enOperation::deposit :
    ShowDepositScreen();
    break;
 default:
    break;
 }
 if(oper!=enOperation::logout){
        cout << "Press any key to go back to main menu...";
        system("pause > nul");
 }
}
void ShowMainMenuScreen (){
        system("cls");
        cout<<"==================================================\n";
        cout<<"                 Main Menu Screen\n";
        cout<<"==================================================\n";
        cout<<"           [1] Quick Withdraw.\n";
        cout<<"           [2] Normal Withdraw.\n";
        cout<<"           [3] Deposit.\n";
        cout<<"           [4] Check Balance.\n";
        cout<<"           [5] Logout.\n";
        cout<<"==================================================\n";
}
void OpenATMSystem(){
enOperation operationNumber ;
    do{
    ShowMainMenuScreen ();
    operationNumber= (enOperation)PositiveShortNumberInRange("Choose what do you want to do? [1 to 5]?\n", 1, 5);
    ExecuteOperation(operationNumber);
}while (operationNumber!=enOperation::logout);
}
bool LoadCurrentClientInfo() {
string accountNumber = GetStringInSize("Enter Account Number: ",1,40,"Writing this number of characters is not allowed. Please try entering the Account Number again : ");
string pinCode = GetStringInSize("Enter Pin Code: ",1,40,"Writing this number of characters is not allowed. Please try entering the Pin Code again : ");
stClient client ;
if(ClientIsExistsInFileByAccNum(accountNumber,client)&&client.pinCode==pinCode){
    currentClient=client;
    return  1;
}else{
    return 0;
}
}
void Login (){
    bool loginFailed=false;
    while (true)
    {
      do{ 
        system("cls");
        cout<<"=====================================\n";
        cout<<"            Login Screen\n";
        cout<<"=====================================\n";
        if(loginFailed){
            cout<<"Invalid Account Number/PinCode!!!\n";
        }
        loginFailed=!LoadCurrentClientInfo();
      }while(loginFailed);
      OpenATMSystem();
    }
    
}

int main (){
    Login ();
    return 0;
}
