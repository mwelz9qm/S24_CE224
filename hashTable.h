#pragma once
//Hashtable Class
// By: Alexander Olson
//
#pragma once
#include<iostream>
#include"accountTemplate.h"
#include<vector>

using namespace std;

class hashTable {
private:
    int size = 100;
    string masterAccess = "Hello!";
    vector<account> hashTable;
    vector<int> salts;

public:

    //Hash Function / Account access and modification
    int hashFunc(string key) {
        //Salt shifts the hash to increase diffculty of reversal
        srand(time(0)); // Setting the Randomizer Seed to be the curren time
        long double hashVal = 0, 
        int salt = (rand()%1000) + 1; // Generating a random salt value
        for (int i = 0; i < key.length(); i++) {
            hashVal += int(key[i]);
            hashVal += salt;
        }
        // Makes the hash larger then the size of the hash table
        string hashval = to_string(hashVal);
        while (hashval.length() < size) {
            hashVal += hashVal;
            hashval = to_string(hashVal);
        }
        int Hash = stoi(hashval) % (size);
        for(int i = 0; i < salts.size() ; i ++){ if (salts[Hash] == NULL) { salts[Hash] = salt; break; }}
        
        return Hash;
    }
    string sha256Pass(string pass) {

    }
    void changeMp(string oMp) {
        if (oMp == masterAccess) {
            string nMp;
            cout << "Please enter the new master password: ";
            cin >> nMp;
            masterAccess = nMp;
        }
        else {
            cout << "Sorry you entered in the incorrect master password. To change the master password you must enter the old one first." << endl;
        }
    }
    void changeAccountInfo(string uN) {
        string oMp;
        cout << "Please enter your current password to change any account info : ";
        cin >> oMp;
        if (sha256Pass(oMp) == hashTable[hashFunc(uN)].password) {
            char sel;
            cout << "What account infomation would you like to change." << endl << "Please enter 'U' for username, 'P' for password, or 'N' for name." << endl << ": ";
            while (cin >> sel) {
                if (sel == 'U') {
                    string uName;
                    cout << "Please enter the new username: ";
                    cin >> uName;
                    hashTable[hashFunc(uN)].name = uName;
                    break;

                }
                else if (sel == 'P') {
                    string nMp;
                    cout << "Please enter the new password: ";
                    cin >> nMp;
                    hashTable[hashFunc(uN)].password = nMp;
                    break;
                }
                else if (sel == 'N') {
                    string name;
                    cout << "Please enter the new name: ";
                    cin >> name;
                    hashTable[hashFunc(uN)].name = name;
                    break;
                }
                else if (sel == 'q') {
                    cout << "Canceling Account changes." << endl;
                    break;
                }
                else {
                    cout << " I am sorry but it seems you entered a character that is not recongised." << endl;
                    cout << "Please enter 'U' for username, 'P' for password,'N' for , or if you would like to exit adn make no changes please enter 'q'." << endl << ": ";
                    continue;
                }
            }

        }
        else {
            cout << "Sorry you entered in the incorrect password. To change any account infomation you must enter your current password first." << endl;
        }
    }
    // Methods/Functions
    void addAccount(string username, string pW, string name) {
        // Modifies Hashtable to fit new size with old values
        if (float(hashTable.size() / hashTable.capacity()) >= .75) { // Load Factor thresh hold to rehash the table
            size+= 100;
            hashTable.resize(size);
            reHash();
        }
        int hashVal = hashFunc(username);// Generatoring hash value

        for (int i = 0; i < size; i++) {// Adding new account to hash table
            auto cell = hashTable[hashVal];
            if (cell.username != "") {
                hashVal = hashVal + 1;
                continue;
            }
            else {
                hashTable[hashVal].username = username;
                hashTable[hashVal].password = sha256Pass(pW);
                hashTable[hashVal].name = name;
                break;
            }
        }
    }
    void deleteAccount(string username, string pW) {
        int hashVal = hashFunc(username);
        bool isNameExists = false;

        for (int i = 0; i < size; i++) {
            auto cell = hashTable[hashVal];
            if (cell.username != username) {
                hashVal = hashVal + 1;
                continue;
            }
            else
            {
                if (pW == hashTable[hashVal].password || pW == masterAccess)
                {
                    hashTable[hashVal] = account();
                    reHash();

                    cout << "Contact \"" << username << "\" Removed" << endl;
                    return;
                }
                cout << "Please enter in the correct password of the master access to delete this account." << endl;
                return;
            }
            if (!isNameExists) {
                cout << "Contact \"" << username << "\" Not Found" << endl;
                return;
            }
        }
    }
    void searchContact(string username) {
        int hashVal = hashFunc(username);
        bool isNameExists = false;

        for (int i = 0; i < size; i++) {
            auto cell = hashTable[hashVal];
            if (cell.username != username) {
                hashVal = hashVal + 1;
                continue;
            }
            else {
                isNameExists = true;
                cout << "USER FOUND: " << hashTable[hashVal].username << " registered as " << hashTable[hashVal].name << endl;
                return;
            }
        }
        if (!isNameExists) {
            cout << "User Not Found - Please Try Again" << endl;
        }
    }
    void reHash() {
        vector<account> tempHash;
        tempHash.resize(size);
        int hashVal;
        for (int i = 0; i < size; i++) {
            hashVal = hashFunc(hashTable[i].username);
            tempHash[hashVal] = hashTable[i];
        }
        hashTable = tempHash;
    }

    bool isEmpty() {
        for (int i = 0; i < size; i++) {
            if (hashTable[i].username != "") {
                return false;
            }
        }
        return true;
    }
    // Display Methods
    void displayAccount(string username, string pass) {
        int hashVal = hashFunc(username);
        if (pass == hashTable[hashVal].password) {
            cout << hashVal << ". USERNAME: " << hashTable[hashVal].username << " - " << "\t NAME: " << hashTable[hashVal].name << "\t Password: " << hashTable[hashVal].password << endl;
        }
        else {
            cout << " You mush enter the correct password to access this infomation" << endl;
        }

    }
    void displayHashTable(string pass) {
        if (pass == masterAccess) {
            for (int i = 0; i < size; i++) {
                if (hashTable[i].username != "") {
                    cout << i << ". USERNAME: " << hashTable[i].username << " - " << "\t NAME: " << hashTable[i].name << "\t Password: " << hashTable[i].password << endl;
                }
            }
        }
        else {
            cout << " You mush enter the master password to access this infomation" << endl;
        }

    }

};