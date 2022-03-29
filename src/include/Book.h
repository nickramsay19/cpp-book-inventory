#pragma once

#include <iostream>
#include <vector>

#define MAX_BOOK_GENRES 8
#define MAX_BOOK_TEXT 32

class Book {
public:
    Book(std::string bookTitle, std::string bookAuthor, std::vector<std::string> bookGenres);

    Book(std::string bookTitle, std::string bookAuthor, std::initializer_list<std::string> bookGenresInitializer);

    void Print();

    std::string GetTitle();
    std::string GetAuthor();
    std::vector<std::string> GetGenres();

private:
    void printGenres();

    std::string title;
    std::string author;
    std::vector<std::string> genres;
};