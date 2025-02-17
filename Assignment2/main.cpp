#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;

struct AVLNode
{
    int data;
    AVLNode* left;
    AVLNode* right;
    int height;
};

AVLNode* CreateNewNode(int data)
{
    AVLNode* newNode = new AVLNode();
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

int GetHeight(AVLNode* root) 
{
    if (root == NULL)
        return 0;
    return root->height;
}

// Right rotation (clockwise)
AVLNode* rightRotate(AVLNode* y)
{
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + max(GetHeight(y->left), GetHeight(y->right));
    x->height = 1 + max(GetHeight(x->left), GetHeight(x->right));

    // Return new root
    return x;
}

// Left rotation (counter-clockwise)
AVLNode* leftRotate(AVLNode* x)
{
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + max(GetHeight(x->left), GetHeight(x->right));
    y->height = 1 + max(GetHeight(y->left), GetHeight(y->right));

    // Return new root
    return y;
}

AVLNode* FindMax(AVLNode* root)
{
    if(root == NULL)
    {
        cout << "Empty Tree\n";
        return NULL;
    }
    while (root->right != NULL)
    {
        root = root->right;
    }
    return root;
}

int GetBalanceValue(AVLNode *root) 
{
  if (root == NULL)
    return 0;
  return GetHeight(root->left) - GetHeight(root->right);
}

AVLNode* insert(AVLNode* root, int data)
{
    if (root == NULL)     // Empty Tree
        return CreateNewNode(data);
    
    if (data < root->data) // Go to left if smaller
        root->left = insert(root->left, data);
    else if (data > root->data) // Go to right if larger
        root->right = insert(root->right, data);
    else
        return root; // Duplicate data, return as is
    
    // Update height of the current node
    root->height = 1 + max(GetHeight(root->left), GetHeight(root->right));

    // Get the balance factor
    int balanceValue = GetBalanceValue(root);

    // Perform rotations if necessary
    if (balanceValue > 1)
    {
        if (data < root->left->data)
            return rightRotate(root); // Left-Left
        else if (data > root->left->data)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root); // Left-Right
        }
    }
    if (balanceValue < -1)
    {
        if (data > root->right->data)
            return leftRotate(root); // Right-Right
        else if (data < root->right->data)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root); // Right-Left
        }
    }

    return root; // Return the updated root
}

AVLNode* remove(AVLNode* root, int data)
{
    if (root == NULL)
        return root;

    if (data < root->data)
        root->left = remove(root->left, data);
    else if (data > root->data)
        root->right = remove(root->right, data);
    else // Target found
    {
        //Case 1: No Child
        if ((root->left == NULL) && (root->right == NULL))
        {
            delete root;
            root = NULL;
        }
        //Case 2: 1 child
        else if ((root->left == NULL) && (root->right != NULL))
        {
            AVLNode *temp = root;
            root=root->right;
            delete temp; 
        }
        else if ((root->right == NULL) && (root->left != NULL))
        {
            AVLNode *temp = root;
            root = root->left;
            delete temp;
        }
        //Case 3: 2 childs, replace with max from left
        else
        {
            AVLNode *temp = FindMax(root->left);
            root->data = temp->data;
            root->left = remove(root->left, temp->data);
        }
    }

    if (root == NULL)
        return root; // If the tree had only one node

    // Update height of the current node
    root->height = 1 + max(GetHeight(root->left), GetHeight(root->right));

    // Get the balance factor
    int balanceValue = GetBalanceValue(root);

    // Perform rotations if necessary
    if (balanceValue > 1)
    {
        if (GetBalanceValue(root->left) >= 0)
            return rightRotate(root); // Left-Left
        else
        {
            root->left = leftRotate(root->left);
            return rightRotate(root); // Left-Right
        }
    }
    if (balanceValue < -1)
    {
        if (GetBalanceValue(root->right) <= 0)
            return leftRotate(root); // Right-Right
        else
        {
            root->right = rightRotate(root->right);
            return leftRotate(root); // left-right
        }
    }

    return root;
}

void inorder(AVLNode* root) 
{
    if (root != NULL) 
    {
        inorder(root->left);
        cout << root->data << " ";
        inorder(root->right); 
    }
}

void preorder(AVLNode* root) 
{
    if (root != NULL) 
    {
        cout << root->data << " ";
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(AVLNode* root) 
{
    if (root != NULL) 
    {
        postorder(root->left);
        postorder(root->right);
        cout << root->data << " ";
    }
}

int main() 
{
    AVLNode* root = NULL;   // Initialize an empty AVLNode
    string input;

    getline(cin, input);  // Read the entire line as input

    // Tokenize the input string
    istringstream iss(input);
    vector<string> tokens;

    while (iss >> input) 
    {
        tokens.push_back(input);
    }

    // Process commands and values
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        string command = tokens[i];

        if (command[0] == 'A') 
        {
            int value = stoi(command.substr(1));  // Extract and convert the value
            root = insert(root, value);
        } 
        else if (command[0] == 'D') 
        {
            int value = stoi(command.substr(1));  // Extract and convert the value
            root = remove(root, value);
        } 
        else if (command == "PRE") 
        {
            preorder(root);
            if (root == NULL)
            cout << "EMPTY" << endl;
        } 
        else if (command == "IN") 
        {
            inorder(root);
            if (root == NULL)
            cout << "EMPTY" << endl;
        } 
        else if (command == "POST") 
        {
            postorder(root);
            if (root == NULL)
            cout << "EMPTY" << endl;
        }
    }
    return 0;
}