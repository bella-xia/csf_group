#include <iostream>
#include <istream>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <cmath>

using std::cout;
using std::endl;
using std::istream;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

struct Slot
{
    uint32_t tag = 0;
    bool valid = false;
    bool dirty = false;
    uint32_t load_ts = 0, access_ts = 0;
};

struct Set
{
    vector<Slot> slots = vector<Slot>();
    int occupancy = 0;
};

struct Cache
{
    vector<Set> sets = vector<Set>();
};

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
        cout << write_allo << endl;
        string write_tb = argv[5];
        cout << write_tb << endl;
        string eviction = argv[6];
        cout << eviction << endl;
        if (num_set_cache < 0 || block_set < 0 || byte_block < 4)
        {
            std::cerr << "Your arguments are invalid." << endl;
            return 2;
        }
        float log_2_num_set_cache = log2(num_set_cache);
        float log_2_block_set = log2(block_set);
        float log_2_byte_block = log2(byte_block);
        if (ceil(log_2_num_set_cache) != floor(log_2_num_set_cache) ||
            ceil(log_2_block_set) != floor(log_2_block_set) ||
            ceil(log_2_byte_block) != floor(log_2_byte_block))
        {
            std::cerr << "Your arguments are invalid." << endl;
            return 2;
        }
        if (write_allo != "no-write-allocate" && write_allo != "write-allocate")
        {
            std::cerr << "Invalid write-allocate parameter." << endl;
            return 3;
        }
        if (write_tb != "write-through" && write_tb != "write-back")
        {
            std::cerr << "Invalid write-through parameter." << endl;
            return 3;
        }
        if (eviction != "lru" && eviction != "fifo")
        {
            std::cerr << "Invalid eviction." << endl;
            return 3;
        }
        bool write_allocate = write_allo == "write-allocate";
        bool write_back = write_tb == "write-back";
        if (write_back && !write_allocate)
        {
            std::cerr << "Invalid combination." << endl;
            return 3;
        }
        vector<pair<char, long>> stored;
        string temp_long;
        char temp_ch;
        int temp_int;
        Cache cache;
        int total_loads = 0;
        int total_stores = 0;
        int load_hits = 0;
        int load_misses = 0;
        int store_hits = 0;
        int store_misses = 0;
        int total_cycles = 0;
        int num_byte_load = byte_block >> 2;
        cache.sets[num_set_cache];
        for (int j = 0; j < num_set_cache; ++j)
        {
            cache.sets.push_back(Set());
            for (int i = 0; i < block_set; ++i)
            {
                cache.sets[j].slots.push_back(Slot());
            }
        }
        int num_mark = 0;
        while (std::cin >> temp_ch >> temp_long >> temp_int)
        {
            num_mark++;
            bool store = temp_ch == 's';
            bool miss = true;
            char *str_ptr = &temp_long[0];
            long converted_long = std::strtoll(str_ptr, nullptr, 16);
            // long converted_long = std::strtoll(str_ptr, nullptr, 2);
            int log_num_set = (int)log_2_num_set_cache;
            int log_num_byte = (int)log_2_byte_block;
            long tag = converted_long >> (log_num_byte + log_num_set);
            long index = (converted_long >> log_num_byte) - (tag << log_num_set);
            // uint32_t index = converted_long & ((1 << (log_num_byte + log_num_set)) - 1) >> (log_num_byte);
            // cout << "tag: "  << tag << endl;
            // cout << "index" << index << endl;
            if (index == 220)
            {
                cout << "tag: " << tag << endl;
                cout << "num_Mark: " << num_mark << endl;
                cout << "occupancy: " << cache.sets[index].occupancy << endl;
            }
            int i = 0;
            for (; i < cache.sets[index].occupancy; ++i)
            {
                Slot &target = cache.sets[index].slots[i];
                if (target.valid && tag == target.tag)
                {
                    miss = false;
                    break;
                }
            }
            if (miss)
            {
                if (store)
                {
                    total_stores++;
                    store_misses++;
                }
                else
                {
                    total_loads++;
                    load_misses++;
                }
                total_cycles += 100;
                if (write_allocate)
                {
                    total_cycles++;
                    Slot curr;
                    curr.tag = tag;
                    curr.valid = true;
                    curr.load_ts = 0;
                    curr.access_ts = 0;
                    curr.dirty = store && write_back;
                    if (cache.sets[index].occupancy >= block_set)
                    {
                        if (index == 220)
                        {
                            cout << "replacing num_mark" << num_mark << endl;
                        }
                        for (int j = 0; j < cache.sets[index].occupancy; ++j)
                        {
                            Slot &target = cache.sets[index].slots[j];
                            if (target.valid && ((int)target.access_ts) >= cache.sets[index].occupancy - 1)
                            {
                                total_cycles += (target.dirty) ? 100 : 0;
                                target = curr;
                            }
                            else
                            {
                                target.access_ts++;
                            }
                        }
                    }
                    else
                    {
                        Slot &target = cache.sets[index].slots[cache.sets[index].occupancy++];
                        target = curr;
                    }
                }
            }
            else
            {
                if (store)
                {
                    total_stores++;
                    store_hits++;
                }
                else
                {
                    total_loads++;
                    load_hits++;
                }
                total_cycles += (write_back) ? 1 : 101;
                Slot &curr_ref = cache.sets[index].slots[i];
                curr_ref.load_ts = 0;
                int access_ts = curr_ref.access_ts;
                curr_ref.access_ts = 0;
                curr_ref.dirty = store && write_back;
                for (int j = 0; j < cache.sets[index].occupancy; ++j)
                {
                    Slot &target = cache.sets[index].slots[j];
                    if (target.valid && ((int)target.access_ts) < access_ts)
                    {
                        target.access_ts++;
                    }
                }
            }
        }
        total_cycles *= num_byte_load;
        cout << "Total loads: " << total_loads << endl;
        cout << "Total stores: " << total_stores << endl;
        cout << "Load hits: " << load_hits << endl;
        cout << "Load misses: " << load_misses << endl;
        cout << "Store hits: " << store_hits << endl;
        cout << "Store misses: " << store_misses << endl;
        cout << "Total cycles: " << total_cycles << endl;
    }
    else
    {
        std::cerr << "Do not hve sufficient arguments." << endl;
        // error 1: not having enough argument
        return 1;
    }
    return 0;
}