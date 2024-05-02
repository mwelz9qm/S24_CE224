#pragma once
// Hashtable Class
// Summary: Creates a hash table of account type objects that are encrypted and stored using the SHA2-256 and a custum 
//          Hashing funciton
// By: Alexander Olson
//
#pragma once
// Include files
#include<iostream>
#include"accountTemplate.h"
#include<vector>
#include"SHA_256Ao.h"

using namespace std;

class hashTable {
private:
    int size = 100; //Current Size of the salt and hash tables
    int cap = 1; // Current number of accounts in the system
    string masterAccess = "Hello!"; // Master Password
    vector<account> hashTable; // stores all of account info
    vector<pair<int,string>> salts;// Stores the salt values generatored by the hash algorthims

public:

    //Hash Function / Account access and modification
    pair<int,string> hashFunc(string key,int Salt = 0) {
        //Salt shifts the hash to increase diffculty of reversal
        srand(time(0)); // Setting the Randomizer Seed to be the curren time
        hashTable.resize(size);
        salts.resize(size);
        long double hashVal = 0;
        int salt = 0;
            if (Salt == 0) {
                salt = rand();  // Generating a random salt value
            }
            else if(Salt != 0) {
               salt = Salt; // If a salt is provided, use that instead
            }
            for (int i = 0; i < key.length(); i++) {
                hashVal += int(key[i]); // Genertoring Hash value
                hashVal += salt;// Adding salt
            }
        string hashval = to_string(hashVal);
        return make_pair(salt, hashval);// Returns the salt and hashval
    }
    int hashMap(string hV) { // takes in hashVal and returns location index
        int Hash = stoi(hV) % size;
        for (int i = 0; i < size - 1; i++) {
            if (hashTable[Hash].username == hV) {
                break;
            }
            else if (hashTable[Hash].username == "") {
                break;
            }
            else {
                Hash++;
            }
        }
        return Hash;
    }
    void changeMp(string oMp) { // Changes master Pass
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
    void changeAccountInfo(string uN) { // Modifies user data 
        string oMp;
        cout << "Please enter your current password to change any account info : ";
        cin >> oMp;
        int hash = 0;
        pair<string, string> curP("H","h");
        // This loop finds the current index
        while (curP.first != hashTable[hash].password) { 
            hash++;
            curP = sha256(oMp,salts[hash].second);
        }

        if ( curP.first == hashTable[hash].password) {
            char sel;
            cout << "What account infomation would you like to change." << endl << "Please enter 'U' for username, 'P' for password, or 'N' for name." << endl << ": ";
            while (cin >> sel) {
                if (sel == 'U') {
                    string uName;
                    cout << "Please enter the new username: ";
                    cin >> uName;
                    // Since placement is based on the username, deletes old placement and switchs data to new one
                    pair<int, string> New = hashFunc(uName); // New Hash
                    string tempS = salts[hash].second; // Saveing SHA salt
                    account t(New.second, hashTable[hash].password, hashTable[hash].name);// Saving User data
                    deleteAccount(hashTable[hash].username, oMp);// Deleting old account placement
                    // Creating new placement with given data and new hash
                    hash = hashMap(New.second);
                    hashTable[hash] = t;
                    salts[hash].second = tempS;
                    salts[hash].first = New.first;
                    break;

                }
                else if (sel == 'P') {
                    string nMp;
                    cout << "Please enter the new password: ";
                    cin >> nMp;
                    pair<string, string> t = sha256(nMp);
                    hashTable[hash].password = t.first;
                    salts[hash].second = t.second;
                    break;
                }
                else if (sel == 'N') {
                    string name;
                    cout << "Please enter the new name: ";
                    cin >> name;
                    hashTable[hash].name = name;
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
        if (float(cap / size) >= .75) { // Load Factor thresh hold to rehash the table
            size+= 100;
            hashTable.resize(size);
            reHash();
        }
        pair<int,string> Hashp = hashFunc(username);// Generatoring hash value
        int Hash = hashMap(Hashp.second); //Mapping to table
        pair<string, string> HashSha = sha256(pW); // Generatoring Password Hash

       // Adding new account to hash table
        salts[Hash].first = Hashp.first;
        hashTable[Hash].username = Hashp.second;
        salts[Hash].second = HashSha.second;
        hashTable[Hash].password = HashSha.first;
        hashTable[Hash].name = name;
        cap++;
    }
    void deleteAccount(string username, string pW) {
        int hash = 0;
        pair<string, string> curP("H", "h");
        // Finds the correct index
        while (curP.first != hashTable[hash].password) {
            hash++;
            if (hash > size) {
                cout << "Contact \"" << username << "\" Not Found" << endl;
                return;
            }
            curP = sha256(pW, salts[hash].second);
        }
        
        if (curP.first == hashTable[hash].password || pW == masterAccess)
        { // Erases accoutn currently at hash by replacing with what is basically a NULL account
            hashTable[hash] = account();
            cout << "Contact \"" << username << "\" Removed" << endl;
            cap--;
            return;
        }
        else {
            cout << "Please enter in the correct password of the master access to delete this account." << endl;
            return;
        }
    }
    void searchContact(string username) {
        int hashVal = 0;
        pair<int, string> curP(0, "h");
        // Finds the correct index
        while (curP.second != hashTable[hashVal].username) {
            hashVal++;
            curP = hashFunc(username, salts[hashVal].first);
        }
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
        // Rehashs table due to change in size
        hashTable.resize(size);
        salts.resize(size);
        int hashVal;
        for (int i = 0; i < size-1; i++) {
            pair<int,string> H = hashFunc(hashTable[i].username);
            hashVal = hashMap(H.second);
            hashTable[hashVal].username = H.second;
            salts[hashVal].first = H.first;
            pair<string, string>nP = sha256(hashTable[i].password);
            hashTable[hashVal].password = nP.first;
            salts[hashVal].second = nP.second;
        }
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
        int hashVal = 0;
        pair<int, string> curP(0, "h");
        //Finds correct Index
        while (curP.second != hashTable[hashVal].username) {
            hashVal++;
            curP = hashFunc(username, salts[hashVal].first);
        }
        if (sha256(pass, salts[hashVal].second).first == hashTable[hashVal].password) {
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