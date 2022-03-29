# Book Inventory CLI | coo-book-inventory
> Developed by Nicholas Ramsay
Just a mock tool made whilst learning C++.

## Usage
```
make
dist/main <YOUR INVENTORY FILE> # e.g. "dist/main books.bin"
```

## Binary size
Storing one book "Animal Farm" in a book inventory binary. The book's data: ("Animal Farm", "George Orwell", ["Satire","Fiction","Politics",]).

At first, entire strings of exact length 32 were written to the binary. These strings contained the contents of the string plus any trailing '\0'. This was done to prevent ambiguous string lengths, preventing data in, say, the book's title and author from being distinguished. Additionally exactly 16 genres were stored, each of which, a string also of exactly 32 bytes. This was a waste of storage space. To store the Animal Book as above would take X bytes or storage.

In the next version, a single byte would be written to the binary indicating the length of the following string. Then only that exact amount indicated would be stored. The same would be true for the genres list, a byte indicating the genres count. The same data in this format takes 4Kb.

## Todo
- replace each instance of `char *err_msg; sprintf(err_msg.....` with a function call to a function to return a char pointer
- add read book function