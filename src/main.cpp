#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>

#include "include/Book.h"
#include "include/InventoryFile.h"

int main(int argc, char** argv) {

    // check for valid args
    if (argc < 2) {
        std::cout << "Error: Please specify a file to store book inventory data.\n";
        return -1;
    }

    // get the filename from args
    char* filename = argv[1];

    // create inventory file object
    InventoryFile invFile(filename);

    // TEST
    bool readOrWriteTest = false;
    if (readOrWriteTest) {
        invFile.Read();
        invFile.PrintInventory();
    } else {
        Book book1("Animal Farm", "George Orwell", {"Fiction", "Satire", "Politics"});
        Book book2("Dracula", "Bram Stoker", {"Fiction", "Gothic", "Horror", "Fantasy", "Thriller"});
        Book book3("The Wealth of Nations", "Adam Smith", {"Non-Fiction", "Economics", "Philosophy", "Treatise", "Resource-allocation Theory"});
        Book book4("The Great Gatsby", "F. Scott Fitzgerald", {"Fiction", "Literature"});
        Book book5("1984", "George Orwell", {"Fiction", "Satire", "Politics"});
        Book book6("War and Peace", "Leo Tolstoy", {"Fiction", "War", "Commentary", "Realist-Fiction"});
        Book book7("Dummy Book", "Nicholas Ramsay", {"Genre1", "Genre2", "Genre3", "Genre4", "Genre5", "Genre6", "Genre7", "Genre8"});
        invFile.AddBook(book1);
        invFile.AddBook(book2);
        invFile.AddBook(book3);
        invFile.AddBook(book4);
        invFile.AddBook(book5);
        invFile.AddBook(book6);
        for (int i = 0; i < 100; i++) {
            invFile.AddBook(book7);
        }
        
        invFile.Write();
    }
    
    // end here, for now
    return 0;

    // enter the cli
    std::string command = "";
    while (true) {
        // print prompt
        std::cout << " ~ ";

        // get user command
        command.clear(); // clear first
        std::cin >> command;

        if (command == "quit" || command == "exit") {
            return 0;
        } else if (command == "help") {
            std::cout << "Help info.\n";
        }
    }


    //Book book("Animal Farm", "George Orwell", {"Satire", "Fiction", "Politics"});

    //book.printGenres();

}