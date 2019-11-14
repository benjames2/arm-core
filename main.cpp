#include <iostream>
#include <fstream>
#include <memory_pool.h>
#include <assert.h>

using namespace std;

int main(void) {

    memory_t mem(memory_t::little_endian);

    // start with empty memory area
    assert(mem.debug_num_pages() == 0);

    // writing memory data from file
    {
        mem.debug_clear_pages(); // start with clean slate
        ifstream is("./txt/memory.txt");

        address_t addr;
        uint16_t  data;

        while(is >> std::hex >> addr) {
            is >> data;
            mem.store_u16(addr, data);
        }

        cout << "memory test file\n";
        cout << endl << mem << endl;

    }
    
    // writing executable data from file
    {
        //mem.debug_clear_pages();
        ifstream is("./txt/binary.txt");

        address_t addr;
        uint32_t  data;

        while(is >> hex >> addr) {

            is >> data;

            if(data > 0xFFFF) {
                mem.store_u32(addr, data);
            }
            else {
                mem.store_u16(addr, data & 0xFFFF);
            }

        }

        cout << "instruction test file\n";
        cout << endl << mem << endl;

    }

    return 0;
}
