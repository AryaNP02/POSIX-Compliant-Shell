#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include <string>
// Global variable  -> for  handlin my path
struct Node {
    std::string command;
    Node* next;
    Node* prev;

   Node(const std::string& cmd) : command(cmd), next(nullptr), prev(nullptr) {}
};



extern Node* head;
extern Node* tail;
extern std::string filename; //history.txt
extern int count;


void addCommand(const std::string& cmd);
void deletelist();
void loadFromFile();
void saveToFile();
void displayHistory(long long num);
std::string getprevious(int x);

#endif // DOUBLELINKEDLIST_H
