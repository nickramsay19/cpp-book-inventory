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

std::string charPtrLen2StrCompressed(char *base, size_t *head) {
    std::string str("");

    printf("CURRENT BYTE POS: %zu\n", *head);

    // get the start pos relative to the base
    char *ptr = &base[*head];

    // first byte should be a byte indicating the length of the string
    size_t len = (size_t) *ptr; // reads one byte, casts to size_t ulong
    printf("STRING LENGTH: %zu\n", len);

    // we read a byte, so move forware one bye
    ptr++;

    // now loop normally, appending each char to the string
    for (char *p = ptr; p < (ptr + len); p++) {
        str += *p;
    }

    // now adjust the position of head
    *head += len + 1; // move by the length of the string plus the length of the one byte str length we read

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

void writeStrCompressed(std::fstream *outfile, std::string str) {
    // check that str length is less than "len"
    if (str.length() > MAX_BOOK_TEXT) {
        char *err_msg;
        sprintf(err_msg, "Error in \'writeStr\': Parameter \'str\' of value \"%s\" exceeds the length restriction of %d.", str.c_str(), MAX_BOOK_TEXT);
        throw std::invalid_argument(err_msg);
    }

    // write the string provided to the output file
    *outfile << (char) str.length(); // write the length but only as one byte to save space, size shouldn't exceed one byte anyway
    *outfile << str; // write the string contents
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

        // file not needed now that we have a buffer copy if its contents: close the file
        inBooksFile.close();

        // extract each book one by one until all bytes read
        size_t b = 0; // the position of the current buffer byte we're at
        while (b < file_size) {

            // copy the title, and author
            std::string title = charPtrLen2StrCompressed(data, &b);
            

            std::cout << "--------------- READING A BOOK ---------------\n";
            std::cout << "data=" << data << ", data[0]=" << data[0] << '\n';
            std::cout << "&b=" << &b << ", b=" << b << '\n';
            std::cout << "title.length()=" << title.length() << '\n';
            std::cout << "title=" << title << '\n';
            std::cout << "--------\n";

            std::string author = charPtrLen2StrCompressed(data, &b);

            // read the byte indicating the amount of genres
            int genresCount = (int) data[b++]; // reads one byte, casts to int

            // copy the genres
            // create the final genres vector
            std::vector<std::string> genres;
            std::string curr_genre; // create a buffer for the current genre
            for (int i = 0; i < genresCount; i++) {
                curr_genre = charPtrLen2StrCompressed(data, &b);
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
            writeStrCompressed(&outBooksFile, book.GetTitle());
            writeStrCompressed(&outBooksFile, book.GetAuthor());
            //outBooksFile.write((char*) book.GetAuthor().c_str(), MAX_BOOK_TEXT * sizeof(char));

            // write the amount of genres first
            outBooksFile << (char) book.GetGenres().size(); // write the length but only as one byte to save space, size shouldn't exceed one byte anyway

            // write each genre
            int genres_written = 0; // keep track of how many books we have written
            for (std::string genre : book.GetGenres()) {
                // write the genre text
                 writeStrCompressed(&outBooksFile, genre);

                genres_written++;
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