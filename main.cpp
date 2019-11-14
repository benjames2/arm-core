#include <iostream>
#include <fstream>
#include <memory_pool.h>
#include <assert.h>

using namespace std;

int main(void) {

    memory_t mem(memory_t::little_endian);

    // start with empty memory area
    assert(mem.debug_num_pages() == 0);

    {
        mem.store_u8(address_t(0), 0x00);
        assert(mem.debug_num_pages() == 0); // storing zeroes shouldnt change number of pages

        mem.store_u16(address_t(0), 0);
        assert(mem.debug_num_pages() == 0); // ... ever

        mem.store_u32(address_t(0), 0);
        assert(mem.debug_num_pages() == 0); // ... ever

        mem.store_u64(address_t(0), 0);
        assert(mem.debug_num_pages() == 0); // ... ever

        mem.store_i8(address_t(0), 0x00);
        assert(mem.debug_num_pages() == 0); // ... ever

        mem.store_i16(address_t(0), 0);
        assert(mem.debug_num_pages() == 0); // ... ever

        mem.store_i32(address_t(0), 0);
        assert(mem.debug_num_pages() == 0); // ... ever

        mem.store_i64(address_t(0), 0);
        assert(mem.debug_num_pages() == 0); // ... ever
    }

    {
        // adding a non-zero value to empty memory should create anew page
        mem.store_u8(address_t(0), 0xFF);
        assert(mem.debug_num_pages() == 1);

        // adding data in a new page should increase the number of pages again
        mem.store_u8(address_t(256), 0xFF);
        assert(mem.debug_num_pages() == 2);

        // clearing all memory should result in zero pages
        mem.debug_clear_pages();
        assert(mem.debug_num_pages() == 0);

        // adding multibyte data that crosses page boundaries should create 2 pages
        mem.store_u16(address_t(255), 0xFFFF);
        assert(mem.debug_num_pages() == 2);

        // crossing page boundaries again creates new page
        mem.store_u32(address_t(510), 0xFFFFFFFF);
        assert(mem.debug_num_pages() == 3);

        // zeroing the last non-zero byte in a page should delete that page
        //mem.store_u8(address_t(255), 0x00);
        //assert(mem.debug_num_pages() == 2);

    }

    {
        mem.debug_clear_pages();

        mem.store_i64(address_t(256*10-4), -32); // storing and loading from the same location should yield the same information
        assert(mem.load_i64(address_t(256*10-4)) == -32);
        assert(mem.debug_num_pages() == 2);

    }

    {
        mem.debug_clear_pages();
        ifstream is("./txt/memory.txt");

        address_t addr;
        uint16_t  data;

        while(is >> std::hex >> addr) {
            is >> data;
            mem.store_u16(addr, data);
        }

    }

    cout << "Success!\n";
    cout << endl << mem << endl;
    
    return 0;
}
