#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <string to search in pi>" << std::endl;
        return 1;
    }

    std::string searchString = argv[1];
    std::ifstream piFile("pi.txt");
    std::string piString;
    piFile >> piString;

    std::size_t found = piString.find(searchString);
    if (found != std::string::npos) {
        std::cout << "Found at position: " << found+1 << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    return 0;
}
