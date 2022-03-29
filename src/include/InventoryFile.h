#pragma once

#include <vector>
#include <string>

#include "Book.h"

class InventoryFile {
public:
    InventoryFile(std::string inventoryFilename);

    void Read();

    void Write();

    void AddBook(Book book);

    void PrintInventory();

private:
    std::vector<Book> inventory;
    std::string filename;
};