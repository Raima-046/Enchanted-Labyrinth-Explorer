

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;
using namespace sf;


struct Node {
    int obj_points;
    char obj_initial;

    Sprite sprite;

    Node* left;
    Node* right;
    int height;

    int pos_x, pos_y;

    Node(int val, char ch, Sprite& _sprite, int xx, int yy)
        : obj_points(val), obj_initial(ch), sprite(_sprite), left(nullptr), right(nullptr), height(1), pos_x(xx), pos_y(yy) {}
};

class AVLTree 
{
public:
    Node* root;
    friend class Game;
    int getHeight(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    int getBalanceFactor(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    Node* insertNode(Node* node, int key, char c, Sprite sp, int pX, int pY) {
        if (node == nullptr) {
            return new Node(key, c, sp, pX, pY);
        }

        if (key < node->obj_points) {
            node->left = insertNode(node->left, key, c, sp, pX, pY);
        }
        else if (key > node->obj_points) {
            node->right = insertNode(node->right, key, c, sp, pX, pY);
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getBalanceFactor(node);

        if (balance > 1 && key < node->left->obj_points) {
            return rotateRight(node);
        }
        if (balance < -1 && key > node->right->obj_points) {
            return rotateLeft(node);
        }
        if (balance > 1 && key > node->left->obj_points) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && key < node->right->obj_points) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* minValueNode(Node* node)
    {
        Node* current = node;
        while (current->left != nullptr)
        {
            current = current->left;
        }
        return current;
    }

    Node* searchNode(Node* node, int key)
    {
        // Base case: If node is null or key matches current node's obj_points
        if (node == nullptr || node->obj_points == key)
        {
            return node;
        }

        // If key is smaller than current node's obj_points, search in left subtree
        if (key < node->obj_points) {
            return searchNode(node->left, key);
        }
        // If key is larger than current node's obj_points, search in right subtree
        else {
            return searchNode(node->right, key);
        }
    }

    Node* deleteNode(Node* root, int key, Node*& removedNode)
    {
        if (root == nullptr)
        {
            return root;
        }

        if (key < root->obj_points)
        {
            root->left = deleteNode(root->left, key, removedNode);
        }
        else if (key > root->obj_points)
        {
            root->right = deleteNode(root->right, key, removedNode);
        }
        else
        {
            removedNode = root; // Store the node to be removed
            if (root->left == nullptr || root->right == nullptr)
            {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr)
                {
                    temp = root;
                    root = nullptr;
                }
                else
                {
                    *root = *temp;
                }
                delete temp;
            }
            else
            {
                Node* temp = minValueNode(root->right);
                root->obj_points = temp->obj_points;
                root->right = deleteNode(root->right, temp->obj_points, removedNode);
            }
        }

        if (root == nullptr) return root;

        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

        int balance = getBalanceFactor(root);

        if (balance > 1 && getBalanceFactor(root->left) >= 0)
        {
            return rotateRight(root);
        }
        if (balance > 1 && getBalanceFactor(root->left) < 0)
        {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }
        if (balance < -1 && getBalanceFactor(root->right) <= 0)
        {
            return rotateLeft(root);
        }
        if (balance < -1 && getBalanceFactor(root->right) > 0)
        {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    void displayInOrder(Node* node, string& output)
    {
        if (node == nullptr) return;

        displayInOrder(node->left, output);
        output += to_string(node->obj_points) + " ";
        displayInOrder(node->right, output);
    }


    AVLTree() : root(nullptr) {}

    void insert(int key, char c, Sprite sp, int pX, int pY)
    {
        root = insertNode(root, key, c, sp, pX, pY);
    }

    /*Node* remove(int key)
    {

       return deleteNode(root, key);
    }*/

    void display()
    {
        string output;
        displayInOrder(root, output);
        cout << "In-order traversal: " << output << endl;
    }

    Node* search(int key) {
        return searchNode(root, key);
    }
};

