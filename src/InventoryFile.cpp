#include "include/InventoryFile.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>

#include "include/Book.h"

// copies the char arr from base[head] to base[len] to a str while incrementing head to the position at len
std::string charPtrLen2Str(char *base, size_t *head, size_t len) {
    std::string str("");

    // get the start pos relative to the base
    char *ptr = &base[*head];

    // now loop normally, appending each char to the string
    for (char *p = ptr; p < (ptr + len); p++) {
        str += *p;
    }

    // now adjust the position of head
    *head += len;

    // return the string
    return str;
}

void writeStr(std::fstream *outfile, std::string str, size_t len) {
    // check that str length is less than "len"
    if (str.length() > len) {
        char *err_msg;
        sprintf(err_msg, "Error in \'writeStr\': Parameter \'str\' of value \"%s\" exceeds the proved length \'len' of %zu.", str.c_str(), len);
        throw std::invalid_argument(err_msg);
    }

    // write the string provided to the output file
    *outfile << str;

    // ensure we have written exactly "len" chars
    // write empty chars up till len
    for (int i = str.length(); i < len; i++) {
        *outfile << '\0';
    }
}

InventoryFile::InventoryFile(std::string inventoryFilename) {
    filename = inventoryFilename;
}

void InventoryFile::Read() {
    // first check if file already exists
    std::fstream inBooksFile;
    inBooksFile.open(filename, std::ios_base::in | std::ios::binary);

    // if the file exists, populate the books vector
    if (inBooksFile.is_open()) {

        // create a char buffer of the entire file
        size_t file_size = std::filesystem::file_size(filename); // first use filesystem to get the file_size
        char *data = reinterpret_cast<char *>(new int[file_size]); // create the buffer of size file_size
        inBooksFile.read(data, file_size); // read the file contents into the buffer

        for (int i = 0; i < file_size; i++) {
            std::cout << data[i];
        }
        std::cout << '\n';

        // file not needed now that we have a buffer copy if its contents: close the file
        inBooksFile.close();

        // extract each book one by one until all bytes read
        size_t b = 0; // the position of the current buffer byte we're at
        while (b < file_size) {

            // copy the title, and author
            std::string title = charPtrLen2Str(data, &b, MAX_BOOK_TEXT);
            

            std::cout << "--------------- READING A BOOK ---------------\n";
            std::cout << "data=" << data << ", data[0]=" << data[0] << '\n';
            std::cout << "&b=" << &b << ", b=" << b << '\n';
            std::cout << "title.length()=" << title.length() << '\n';
            std::cout << "title=" << title << '\n';
            std::cout << "--------\n";

            std::string author = charPtrLen2Str(data, &b, MAX_BOOK_TEXT);

            // copy the genres
            // create the final genres vector
            std::vector<std::string> genres;
            std::string curr_genre; // create a buffer for the current genre
            for (int i = 0; i < MAX_BOOK_GENRES; i++) {
                curr_genre = charPtrLen2Str(data, &b, MAX_BOOK_TEXT);
                genres.push_back(curr_genre); // add the genre to the vector
            }

            // finally create the book object
            Book book(title, author, genres);
            inventory.push_back(book); // add the book to the inventory buffer
        }
    }
    
}

void InventoryFile::Write() {
    std::fstream outBooksFile;
    outBooksFile.open(filename, std::ios_base::out | std::ios::binary);

    // if the file exists, populate the books vector
    if (outBooksFile.is_open()) {

        // loop through the inventory buffer
        for (Book book : inventory) {

            // write title and author
            //outBooksFile << book.GetTitle();
            //outBooksFile << book.GetAuthor();
            //outBooksFile.write(book.GetTitle().c_str(), MAX_BOOK_TEXT);
            writeStr(&outBooksFile, book.GetTitle(), MAX_BOOK_TEXT);
            writeStr(&outBooksFile, book.GetAuthor(), MAX_BOOK_TEXT);
            //outBooksFile.write((char*) book.GetAuthor().c_str(), MAX_BOOK_TEXT * sizeof(char));

            // write each genre
            int genres_written = 0; // keep track of how many books we have written
            for (std::string genre : book.GetGenres()) {
                // write the genre text
                 writeStr(&outBooksFile, genre, MAX_BOOK_TEXT);

                genres_written++;
            }

            // we need to ensure we have written exactly MAX_BOOK_GENRES genre strings, or the binary file will be unreadable
            // write an empty string for every genre unwritten below MAX_BOOK_GENRES
            std::string empty(MAX_BOOK_TEXT, '\0');
            for (int i = genres_written; i < MAX_BOOK_GENRES; i++) {
                outBooksFile << empty;
            }
        }

       
        // close the file
        // check that the file is still "good"
        outBooksFile.close();
        if(!outBooksFile.good()) {
            char *err_msg;
            sprintf(err_msg, "An error occurred after writing to file \"%s\".", filename.c_str());
            throw std::invalid_argument(err_msg);
        }
            
    } else {
        // failed to open file: throw an error
        char *err_msg;
        sprintf(err_msg, "Failed to write to file \"%s\".", filename.c_str());
        throw std::invalid_argument(err_msg);
    }
}

void InventoryFile::AddBook(Book book) {
    inventory.push_back(book);
}

void InventoryFile::PrintInventory() {
    for (Book book : inventory) {
        book.Print();
    }
}