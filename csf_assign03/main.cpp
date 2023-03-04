#include <iostream>
#include <istream>
#include <string>
#include <utility>
#include <vector>
#include <sstream>


using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::istream;
using std::stringstream;

int main(int argc, char *argv[])
{
    if (argc == 7)
    {
        int num_set_cache = std::stoi(argv[1]);
        cout << "number of sets in cache is " << num_set_cache << endl;
        int block_set = std::stoi(argv[2]);
        cout << "number of blocks in each set is " << block_set << endl;
        int byte_block = std::stoi(argv[3]);
        cout << "number of bytes in each block is " << byte_block << endl;
        string write_allo = argv[4];
        cout << write_allo  << endl;
        string write_tb = argv[5];
        cout << write_tb  << endl;
        string eviction = argv[6];
        cout << eviction << endl;

        vector<pair<char, long>> store;
        string temp_long;
        char temp_ch;
        int temp_int;
        while (std::cin >> temp_ch >> temp_long >> temp_int) {
            char * str_ptr = &temp_long[0];
            long converted_long = std::strtoll(str_ptr, nullptr, 16);
            store.push_back(std::make_pair(temp_ch, converted_long));
        }
    }
    return 0;
}