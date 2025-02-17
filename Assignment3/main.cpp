#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

enum SlotStatus { NEVER_USED, TOMBSTONE, OCCUPIED };

class HashTable 
{
private:
    static const int TABLE_SIZE = 26;  // Number of slots in the hash table
    vector<string> table;    // Using a simple array for linear probing
    vector<SlotStatus> status;    // Status array to track slot statuses

public:
    HashTable() 
    {
        // Initialize the hash table with empty strings and "never used" statuses
        table.resize(TABLE_SIZE);
        status.resize(TABLE_SIZE, NEVER_USED);
    }

    // Hash function to map the last character of the string to an index
    int hash(char key) 
    {
        return key - 'a';  // Assuming lowercase letters 'a' to 'z' are keys
    }

    void insertItem(const string& item) 
    {
    if (!item.empty()) 
        {
            char key = item.back();  // Get the last character as the key
            int index = hash(key);
        
            // Linear probing to find an empty or tombstone slot
            while (status[index] == OCCUPIED) 
            {
                // Check if the item is already in the table
                if (table[index] == item) 
                {
                    return;  // Item already exists, do not insert again
                }
            
                index = (index + 1) % TABLE_SIZE;  // Move to the next slot circularly
            }
        
            // Insert the item when an empty slot is found
            table[index] = item;
            status[index] = OCCUPIED;
        }   
    }

    // Search for an item in the hash table
    bool searchItem(const string& item) 
    {
        if (!item.empty()) 
        {
            char key = item.back();  // Get the last character as the key
            int index = hash(key);
            
            // Linear probing to search for the item
            while (status[index] != NEVER_USED) 
            {
                if (status[index] == OCCUPIED && table[index] == item) 
                {
                    return true;  // Item found
                }
                index = (index + 1) % TABLE_SIZE;  // Move to the next slot circularly
            }
        }
        return false;  // Item not found
    }

    // Delete an item from the hash table
    void deleteItem(const string& item) 
    {
        if (!item.empty()) 
        {
            char key = item.back();  // Get the last character as the key in lowercase
            int index = hash(key);
            
            // Linear probing to find and mark the item as a tombstone
            while (status[index] != NEVER_USED) 
            {
                if (status[index] == OCCUPIED && table[index] == item) 
                {
                    status[index] = TOMBSTONE;  // Mark as tombstone
                    return;
                }
                index = (index + 1) % TABLE_SIZE;  // Move to the next slot circularly
            }
        }
    }
    // Print the contents and statuses of the hash table
    void printHTable() 
    {
        bool first = true; // Flag to prevent adding a space before the first element
        for (int i = 0; i < TABLE_SIZE; ++i) 
            {
                if (status[i] == OCCUPIED) 
                {
                    if (!first) 
                    {
                        cout << ' '; // Add a space before each element (except the first)
                    }
                    cout << table[i];
                    first = false;
                }
            }
        cout << endl; // Add a newline character at the end
    }
};

int main() 
{
    HashTable h;
    string input;

    getline(cin, input);  // Read the entire line as input

    // Tokenize the input string
    istringstream iss(input);
    vector<string> tokens;

    while (iss >> input) 
    {
        tokens.push_back(input);
    }

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        string command = tokens[i];

        if (command[0] == 'A') 
        {
            string value = command.substr(1);  // Extract the value
            h.insertItem(value);
        }
        else if (command[0] == 'D') 
        {
            string value = command.substr(1);  // Extract the value
            h.deleteItem(value);
        } 
    }

    h.printHTable();

    return 0;
}