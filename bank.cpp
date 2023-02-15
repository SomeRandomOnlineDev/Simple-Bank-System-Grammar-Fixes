#include <iostream>
#include <conio.h>
#include <ctime>
#include <random>
#include <fstream>
#include <cstring>
#include "Account.h"
#include "Bank.h"

using namespace std;
/*
 * Maybe don't import all of the std namespace
 * try using "using std::cout;" and "std::cin;" in future
 * Maybe I'll go through and clean that up later
 */

void menu()
{
    Account account;

    int choice; // I'd suggest putting variable declarations at the top
                // makes finding all variables used in a project much easier to find

    cout << "<<<<<\tBanking System\t>>>>>\n";
    cout << "<<<\tMain menu\t>>>\n";

    cout << "1 - Create an account\n";
    cout << "2 - Login to an account\n";
    cout << "3 - Show all accounts\n";
    cout << "4 - Search account by ID\n";
    cout << "5 - Delete account\n";
    cout << "Choose a number: ";
    cin >> choice;
   
     switch (choice)
     {
      case 1:
         account_create();
         break;
      case 2:
         account_login();
         break;
      case 3:
         account_view_all();
         break;
      case 4:
         account_search();
         break;
      case 5:
         account_deletion();
         break;
     default:
         cout << "Invalid choice...\n";
         break;
     }
}

void account_create()
{
   Account person;
            
   ofstream fp("Accounts.dat",ios::binary | ios::app);

   random_device dev;
   mt19937 rng(dev());
   uniform_int_distribution<mt19937::result_type> dist6(1,10000); 

   time_t now = time(0);
   char* dt = ctime(&now);
   tm *gmtm = gmtime(&now);
   dt = asctime(gmtm);

   person.id = dist6(rng);
   cout << "ID: " << person.id << endl;

   cin.ignore(1); // 
   cout << "Name: ";
   cin.getline(person.name,100);

   cout << "Pin: ";
   cin >> person.pin;

   cout << "Deposit: ";
   cin >> person.balance;

   strcpy(person.created_at,dt);
   cout << "Created at: " << person.created_at;

   fp.write((char*)&person,sizeof(person));
   fp.close();

}

void account_view_all()
{
   Account person;
   ifstream fp("Accounts.dat");
   while (fp.read((char *)&person,sizeof(person)))
   {
      cout << "*******************************************\n";
      cout << "Account's ID: " << person.id << endl
      << "Account's name: " << person.name << endl
      << "Account's balance: $" << person.balance << endl
      << "Created At: " << person.created_at << endl;  
   }

   fp.close();
}

void account_search()
{
   Account person;
   ifstream fp("Accounts.dat");
   int choice;
   cout << "Enter ID number :";
   cin >> choice;

   while (fp.read((char *)&person,sizeof(person)))
   {
      if (choice == person.id)
      {
         cout << "*******************************************" << endl;
         cout << "Account found" << endl;
         cout << "Account's ID: " << person.id << endl
         << "Account's name: " << person.name << endl
         << "Account's balance: $" << person.balance << endl
         << "Created At: " << person.created_at << endl;  
      }
      
   }
   fp.close();
}

void account_deletion()
{
   Account person;
   ifstream fp("Accounts.dat");
   ofstream tmp("tmp.dat",ios::binary | ios::app);
   int choice;
   cout << "Enter ID number :";
   cin >> choice;

   while (fp.read((char *)&person,sizeof(person)))
   {
      if (choice != person.id) tmp.write((char*)&person,sizeof(person));

      if (choice == person.id)
      {
         cout << "Account found and deleted" << endl;
         cout << "*******************************************" << endl;
         cout << "Account's ID: " << person.id << endl
         << "Account's name: " << person.name << endl
         << "Account's balance: $" << person.balance << endl
         << "Created At: " << person.created_at << endl; 
      }
   }

   fp.close();
   tmp.close();

   int status = remove("Accounts.dat");
   if(status == 0) cout <<"\nFile Deleted Successfully!" << endl;
   else cout<<"\nError Occurred!";

   if (rename("tmp.dat", "Accounts.dat") != 0) perror("Error renaming file\n");
	else cout << "File renamed successfully";
}

void account_login()
{
   int choice;
   int found = 0;
   Account person;
   ifstream fp("Accounts.dat");
   cout << "Enter ID number :";
   cin >> choice;

   while (fp.read((char *)&person,sizeof(person))) if (choice == person.id) found = 1;

   if (found)
   {
      int pin;
      cout << "Account found" << endl;
      cout << "Enter pin number" << endl;
      cin >> pin;
      cout << "Checking...." << endl;
      if (pin == person.pin)
      {
         cout << "Pin correct" << endl;
         long double balance = person.menu();
         fp.close();
         account_update(choice,balance);
     
      }

      else cout << "Wrong pin number. Please try again..." << endl;
   }
   else cout << "Account not found..." << endl;
      
}

void account_update(int id,long double balance)
{
   Account person;
   ifstream fp("Accounts.dat");
   ofstream tmp("tmp.dat",ios::binary | ios::app);
   int choice = id;

   while (fp.read((char *)&person,sizeof(person)))
   {
      if (choice == person.id) 
      {
         person.balance = balance;
         tmp.write((char*)&person,sizeof(person));
      }
   }

   fp.close();
   tmp.close();

   int status = remove("Accounts.dat");
   if(status == 0) cout<<"\nFile Deleted Successfully!\n";
   else cout<<"\nError Occurred!";

   if (rename("tmp.dat", "Accounts.dat") != 0) perror("Error renaming file\n");
	else cout << "File renamed successfully";
}
