#include "doubleLinkList.h"


#include <fstream>
#include <string>

using namespace std;



Node* head = NULL;
Node* tail = NULL;
string filename = "history.txt";
int count = 0;
/////////////////////////// functions for add ,delete , get , and save  commands to history /////////////////////

void addCommand(const string& cmd) {
    Node* newNode = new Node(cmd);
    if (!head) {  // If the list is empty
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    count++;

    if (count > 20) {
        Node* temp = head;
        head = head->next;
        if (head) {
            head->prev = NULL;
        }
        delete temp;
        count--;
    }
}


void deletelist() {
    Node* current = head;
    while (current) {
        Node* to_delete = current;
        current = current->next;
        delete to_delete;
    }
    head = tail = NULL;
    count = 0;
}

void loadFromFile() { /// first to run when initiated.


    ifstream infile(filename);
    if (!infile) 
    {
        printf ("Error : The file 'history.txt' can't be found  \n");
        printf ("creating one \n");

        ofstream outfile(filename);
        if (!outfile) 
        printf ("Fail  to create : history.txt \n");

        printf("Success \n");
    
        
        return;
    
    }

    string line;
    while (getline(infile, line)) {
        addCommand(line);
    }
}


void saveToFile() {
    ofstream outfile(filename);
    if (!outfile) {
      printf ("Fail  to save the content into file : history.txt \n");
        return;
    }

    Node* current = head;
    while (current) {
        outfile << current->command << endl;
        current = current->next;
    }
}

void displayHistory(long long num) {
    Node* current = tail;
    while (current && num > 0) {
        printf ("%s \n" ,( current->command.c_str()));
        current = current->prev;
        num--;
    }
}

///////////////////////////////////////// for arrow up /////////////////////////////////
string getprevious(int x) {
    Node* current = tail;
    while (current && x > 0) {
        current = current->prev;
        x--;
    }
    return (current) ? current->command : ""; // Return command if found, else empty string
}