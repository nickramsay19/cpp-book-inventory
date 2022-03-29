#include "include/Book.h"

#include <iostream>
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <stdio.h> // for string formatting w/ sprintf

Book::Book(std::string bookTitle, std::string bookAuthor, std::vector<std::string> bookGenres) {

    // validate title length
    if (bookTitle.length() > MAX_BOOK_TEXT) {
        char *err_msg;
        sprintf(err_msg, "Parameter \"bookTitle\" of type std::string exceeds the maximum length of %d.", MAX_BOOK_TEXT);
        throw std::invalid_argument(err_msg);
    } else {
        title = bookTitle;
    }

    // validate author string length
    if (bookAuthor.length() > MAX_BOOK_TEXT) {
        char *err_msg;
        sprintf(err_msg, "Parameter \"bookAuthor\" of type std::string exceeds the maximum length of %d.", MAX_BOOK_TEXT);
        throw std::invalid_argument(err_msg);
    } else {
        author = bookAuthor;
    }
    
    // validate length of bookGenres
    if (bookGenres.size() > MAX_BOOK_GENRES) {
        char *err_msg;
        sprintf(err_msg, "Parameter \"bookGenres\" of type std::vector<std::string> must not have more than %d elements.", MAX_BOOK_GENRES);
        throw std::invalid_argument(err_msg);
    }

    // add each genre to genres while validating their length
    for (auto genre : bookGenres) {
        if (genre.length() > MAX_BOOK_TEXT) {
            char *err_msg;
            sprintf(err_msg, "Parameter \"bookGenres\" of type std::vector<std::string> contains an element exceeding the restricted length of %d.", MAX_BOOK_TEXT);
            throw std::invalid_argument(err_msg);
        } else {
            genres.push_back(genre);
        }
    }
}

Book::Book(std::string bookTitle, std::string bookAuthor, std::initializer_list<std::string> bookGenresInitializer) {
    title = bookTitle;
    author = bookAuthor;
    
    // validate length of bookGenres
    if (bookGenresInitializer.size() > MAX_BOOK_GENRES) {
        char *err_msg;
        sprintf(err_msg, "Parameter \"bookGenresInitializer\" of type std::initializer_list<std::string> must not have more than %d elements.", MAX_BOOK_GENRES);
        throw std::invalid_argument(err_msg);
    }

    // add each genre to genres while validating their length
    for (const auto *sp = bookGenresInitializer.begin(); sp < bookGenresInitializer.end(); sp++) {
        if (sp->length() > MAX_BOOK_TEXT) {
            char *err_msg;
            sprintf(err_msg, "Parameter \"bookGenresInitializer\" of type std::initializer_list<std::string> contains an element exceeding the restricted length of %d.", MAX_BOOK_TEXT);
            throw std::invalid_argument(err_msg);
        } else {
            genres.push_back(*sp);
        }
    }
}

void Book::Print() {
    printf("(\"%s\", \"%s\", [", title.c_str(), author.c_str());
    printGenres();
    std::cout << "])\n";
}

void Book::printGenres() {
    for (std::string g : genres) {
        std::cout << "\" << g << "\",";
    }
}

std::string Book::GetTitle() {
    return title;
}

std::string Book::GetAuthor() {
    return author;
}

std::vector<std::string> Book::GetGenres() {
    return genres;
}