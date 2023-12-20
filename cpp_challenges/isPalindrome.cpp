#include <iostream>
#include <vector>

// Define the Node structure
struct Node {
    char data;
    Node* next;
    
    Node(char value) : data(value), next(nullptr) {}
};

// Function to check if a linked list is a palindrome
bool 
isPalindrome(Node* head) 
{
    Node *n = head;
    std::string str;
    int length = 0;
    std::vector<char> vector;
    bool istrue = false;

    while (true)
    {
        if (n != nullptr)
        {
            length++;
            std::cout << "isPalindrome : num = " << length << " : data = " << n->data << "\n";
            vector.push_back(n->data);
            n = n->next;
            continue;
        }
        break;
    }

    if (length % 2 == 0)
    {
        
    }
    else 
    {
        int cur_length = (length / 2) - 1;

        while (cur_length != -1)
        {
            if (vector[cur_length] == vector[(vector.size() - cur_length) - 1])
            {
                cur_length--;
                istrue = true;
                continue;
            }
            else 
            {
                istrue = false;
            }
            break;
        } 
    }
    
    return istrue;
}

// Test the palindrome check
int main() {
    // Create a sample linked list: r -> a -> d -> a -> r
    Node* head = new Node('r');
    head->next = new Node('a');
    head->next->next = new Node('d');
    head->next->next->next = new Node('a');
    head->next->next->next->next = new Node('r');

    std::cout << "Linked List: r -> a -> d -> a -> r" << std::endl;

    if (isPalindrome(head)) {
        std::cout << "The linked list is a palindrome." << std::endl;
    } else {
        std::cout << "The linked list is not a palindrome." << std::endl;
    }

    // Clean up memory (free allocated nodes)
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}