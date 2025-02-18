#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int TABLE_SIZE = 13;
const int PRIME = 11;

enum EntryType { Legitimate, Empty, Deleted };

class HashEntry {
public:
    int key;
    enum EntryType info;
};

class HashMap {
private:
    int current_size;
    HashEntry *htable;

public:
    HashMap() {
        current_size = TABLE_SIZE;
        htable = new HashEntry[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) {
            htable[i].info = Empty;
            htable[i].key = NULL;
        }
    }

    int hash1(int key) {
        return key % TABLE_SIZE;
    }

    int hash2(int key) {
        return PRIME - (key % PRIME);
    }

    void Insert(int key) {
        int position = Search(key);
        if (htable[position].info != Legitimate) {
            htable[position].info = Legitimate;
            htable[position].key = key;
        }
    }

    int Search(int key) {
        int hash_val = hash1(key);
        int hash_val2 = hash2(key);
        while (htable[hash_val].info != Empty &&
               htable[hash_val].key != key) {
            hash_val = (hash_val + hash_val2) % TABLE_SIZE;
        }
        if (htable[hash_val].info == Legitimate && htable[hash_val].key == key)
            return hash_val;
        else
            return NULL;
    }

    void Remove(int key) {
        int hash_val = Search(key);
        if (hash_val != NULL) {
            htable[hash_val].info = Deleted;
            htable[hash_val].key = NULL;
        }
    }

    void Display() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int value = htable[i].key;
            if (!value) {
                cout << i << ": " << endl;
            } else {
                cout << i << ": " << htable[i].key << endl;
            }
        }
    }

    ~HashMap() {
        delete[] htable;
    }
};

int main() {
    HashMap hash;
    int key, flag;
    int choice;
    int size, size2, size3;
    int i = 0;
    int find;

    ifstream file;
    file.open("input.txt", ios::in);

    if (!file) {
        cout << "Error in opening file!!!" << endl;
        return 0;
    }

    cout << "File opened successfully." << endl;

    file >> size;

    for (int i = 0; i < size; i++) {
        file >> key;
        hash.Insert(key);
    }

    file >> size2;

    for (i = 0; i < size2; i++) {
        file >> key;
        hash.Remove(key);
    }

    file >> size3;

    for (i = 0; i < size3; i++) {
        file >> key;
        int location = hash.Search(key);
        if (location != -1)
            break;
        else
            cout << key << " not found" << endl;
    }

    	while (1) {
		cout << "\n----------------------" << endl;
		cout << "Operations on Hash Table" << endl;
		cout << "\n----------------------" << endl;
		cout << "1.Insert element into the table" << endl;
		cout << "2.Search element from the key" << endl;
		cout << "3.Delete element at a key" << endl;
		cout << "4.Display all the elements" << endl;
		cout << "5.Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		switch (choice) 
		{
		case 1:
			cout << "Enter element to be inserted: ";
			cin >> key;
			hash.Insert(key);
			hash.Display();
			break;

		case 2:
			cout << "Enter element to be found: ";
			cin >> key;
			find = hash.Search(key);
			cout << key << "found at: " << find;
			break;

		case 3:
			cout << "Enter element to be deleted: ";
			cin >> key;
			hash.Remove(key);
			hash.Display();
			break;

		case 4:
			hash.Display();
			break;

		case 5:
			file.close();
			exit(1);
			break;

		default:
			cout << "\nEnter correct option\n";
		}
	}

	return 0;
}