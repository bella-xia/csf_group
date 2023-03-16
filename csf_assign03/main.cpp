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
        /* Store the parameters */

        int num_set_cache = std::stoi(argv[1]);
        int block_set = std::stoi(argv[2]);
        int byte_block = std::stoi(argv[3]);
        string write_allo = argv[4];
        string write_tb = argv[5];
        string eviction = argv[6];

        /* Error checking */
        
        if (num_set_cache < 0 || block_set < 0 || byte_block < 4)
        { // check negative nums
            std::cerr << "Your arguments are invalid." << endl;
            return 2;
        }
        float log_2_num_set_cache = log2(num_set_cache);
        float log_2_block_set = log2(block_set);
        float log_2_byte_block = log2(byte_block);

        if (ceil(log_2_num_set_cache) != floor(log_2_num_set_cache) ||
            ceil(log_2_block_set) != floor(log_2_block_set) ||
            ceil(log_2_byte_block) != floor(log_2_byte_block))
        { // check all nums are power of 2
            std::cerr << "Your arguments are invalid." << endl;
            return 2;
        }
        if (write_allo != "no-write-allocate" && write_allo != "write-allocate")
        { // check write-allocate parameter
            std::cerr << "Invalid write-allocate parameter." << endl;
            return 3;
        }
        if (write_tb != "write-through" && write_tb != "write-back")
        { // check write-through parameter
            std::cerr << "Invalid write-through parameter." << endl;
            return 3;
        }
        if (eviction != "lru" && eviction != "fifo")
        { // check eviction parameter
            std::cerr << "Invalid eviction." << endl;
            return 3;
        }
        bool write_allocate = write_allo == "write-allocate";
        bool write_back = write_tb == "write-back";
        bool lru = eviction == "lru";
        if (write_back && !write_allocate)
        { // check valid write-through & write_allocate parameter combination
            std::cerr << "Invalid combination." << endl;
            return 3;
        }

        /* Initialize the counts */

        int total_loads = 0;
        int total_stores = 0;
        int load_hits = 0;
        int load_misses = 0;
        int store_hits = 0;
        int store_misses = 0;
        int total_cycles = 0;
        int num_byte_load = byte_block >> 2; // the time needed for loading/storing a whole block from/to memory
        
        /* Initialize the cache*/

        Cache cache;
        for (int j = 0; j < num_set_cache; ++j)
        {
            cache.sets.push_back(Set());
            for (int i = 0; i < block_set; ++i)
            {
                cache.sets[j].slots.push_back(Slot());
            }
        }

        /* Get input */

        char temp_ch; // 'l' or 'r'
        string temp_long; // address
        int temp_int; // trash
        while (std::cin >> temp_ch >> temp_long >> temp_int)
        {
            bool store = temp_ch == 's';
            bool miss = true;
            char *str_ptr = &temp_long[0];
            long converted_long = std::strtoll(str_ptr, nullptr, 16); // address 
            
            int log_num_set = (int)log_2_num_set_cache;
            int log_num_byte = (int)log_2_byte_block;
            long tag = converted_long >> (log_num_byte + log_num_set); // get the tag from the address
            long index = (converted_long >> log_num_byte) - (tag << log_num_set); // get the index from the address
            int i = 0; // the hit slot's index
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

                // while write_allcate or non-write allocate and load, get the block from memory
                if (write_allocate || (!write_allocate && !store))
                {
                    Slot curr;
                    curr.tag = tag;
                    curr.valid = true;
                    curr.load_ts = 0;
                    curr.access_ts = 0;
                    curr.dirty = store && write_back;
                    // if the set is full
                    if (cache.sets[index].occupancy >= block_set)
                    {
                        for (int j = 0; j < cache.sets[index].occupancy; ++j)
                        {
                            Slot &target = cache.sets[index].slots[j];
                            if (lru){
                                // replace the least recently accessed slot
                                if (target.valid && ((int)target.access_ts) >= cache.sets[index].occupancy - 1)
                                {
                                    total_cycles += (target.dirty) ? 100 * num_byte_load : 0;
                                    target = curr;
                                }
                                else if (target.valid)
                                {
                                    // increment the access time of all the other slot
                                    target.access_ts++;
                                }
                            } else {
                                // replace the least recently accessed slot
                                if (target.valid && ((int)target.load_ts) >= cache.sets[index].occupancy - 1)
                                {
                                    total_cycles += (target.dirty) ? 100 * num_byte_load : 0;
                                    target = curr;
                                }
                                else if (target.valid)
                                {
                                    // increment the access time of all the other slot
                                    target.load_ts++;
                                }
                            }
                        }
                    }
                    else
                    {
                        // increment the access time of all the other slots
                        for (int j = 0; j < cache.sets[index].occupancy; ++j)
                        {
                            cache.sets[index].slots[j].access_ts++;
                            cache.sets[index].slots[j].load_ts++;
                        }
                        Slot &target = cache.sets[index].slots[cache.sets[index].occupancy++];
                        target = curr;
                    }
                }
            }
            else // in hit
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

                Slot &curr_ref = cache.sets[index].slots[i];
                curr_ref.load_ts = 0;
                int access_ts = curr_ref.access_ts;
                curr_ref.access_ts = 0;
                curr_ref.dirty = store && write_back;
                // increment the access time for all the slot less than the replaced value
                for (int j = 0; j < cache.sets[index].occupancy; ++j)
                {
                    Slot &target = cache.sets[index].slots[j];
                    if (target.valid && ((int)target.access_ts) < access_ts && j != i)
                    {
                        target.access_ts++;
                    }
                }
            }

            /* counting the cycles*/
            if (write_back && write_allocate)
            {
                if (!miss)
                {
                    total_cycles++;
                }
                else
                {
                    total_cycles += 1 + 100 * num_byte_load;
                }
            }
            else if (!write_back && write_allocate)
            {
                if (!miss && !store)
                {
                    total_cycles++;
                }
                else if (!miss && store)
                {
                    total_cycles += 101;
                }
                else if (miss && !store)
                {
                    total_cycles += 1 + 100 * num_byte_load;
                }
                else
                {
                    total_cycles += 101 + 100 * num_byte_load;
                }
            }
            else
            {
                if (!miss && !store)
                {
                    total_cycles++;
                }
                else if (!miss && store)
                {
                    total_cycles += 101;
                }
                else if (miss && !store)
                {
                    total_cycles += 1 + 100 * num_byte_load;
                }
                else
                {
                    total_cycles += 100;
                }
            }
        }
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