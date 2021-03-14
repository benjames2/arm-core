#include <iostream>
#include <inc/memory_pool.h>


memory_page_t::memory_page_t(void) {

    this->sz = 0;
    for(int i = 0; i < 256; i++)
        this->bytes[i] = 0x00; // may want to just count the number of non-zero entries
}

bool operator==(const memory_page_t& lhs, const memory_page_t& rhs) {
    return lhs.bytes == rhs.bytes;
}

memory_t::memory_t(const int endianness) {
    this->endianness = endianness;
}

const int memory_t::get_endianness(void) {
    return this->endianness;
}

// ==================================================================
// load functions
// ==================================================================

uint8_t memory_t::load_u8(uint32_t address) {

    uint32_t page = address >> 8;
    auto iter = this->mem_lut.find(page);

    if(iter == this->mem_lut.end()) {
        // this address does not exist in the memory pool
        return 0x00;
    }
    else {
        // index into the correct page to grab the byte
        return iter->second.bytes[address & 0xFF];
    }
}

uint16_t memory_t::load_u16(address32_t address) {
    uint16_t data, dl, dh;

    dl = this->load_u8(address);
    dh = this->load_u8(address+1);

    data = (dl << 0) | (dh << 8);

    if(this->endianness == this->big_endian)
        return byte_swap_16(data);
    else
        return data;
}

uint32_t memory_t::load_u32(address32_t address) {
    uint32_t data, d0, d1, d2, d3;

    d0 = this->load_u8(address + 0);
    d1 = this->load_u8(address + 1);
    d2 = this->load_u8(address + 2);
    d3 = this->load_u8(address + 3);

    data = (d3 << 24) | (d2 << 16) | (d1 << 8) | (d0 << 0);

    if(this->endianness == this->big_endian)
        return byte_swap_32(data);
    else
        return data;

}

uint64_t memory_t::load_u64(address32_t address) {
    uint64_t data, d0, d1, d2, d3, d4, d5, d6, d7;

    d0 = this->load_u8(address + 0);
    d1 = this->load_u8(address + 1);
    d2 = this->load_u8(address + 2);
    d3 = this->load_u8(address + 3);
    d4 = this->load_u8(address + 4);
    d5 = this->load_u8(address + 5);
    d6 = this->load_u8(address + 6);
    d7 = this->load_u8(address + 7);

    data = 
        (d7 << 56) | (d6 << 48) | (d5 << 40) | (d4 << 32) | 
        (d3 << 24) | (d2 << 16) | (d1 << 8) | (d0 << 0);

    if(this->endianness == this->big_endian)
        return byte_swap_64(data);
    else
        return data;
}

// load signed data
int8_t memory_t::load_i8(address32_t address) {
    union {
        int8_t i8;
        uint8_t u8;
    };

    u8 = this->load_u8(address);
    return i8;
}

int16_t memory_t::load_i16(address32_t address) {
    union {
        int16_t i16;
        uint16_t u16;
    };

    u16 = this->load_u16(address);
    return i16;
}

int32_t memory_t::load_i32(address32_t address) {
    union {
        int32_t  i32;
        uint32_t u32;
    };

    u32 = this->load_u32(address);
    return i32;
}

int64_t memory_t::load_i64(address32_t address) {
    union {
        int64_t  i64;
        uint64_t u64;
    };

    u64 = this->load_u64(address);
    return i64;
}

// ==================================================================
// store functions
// ==================================================================

void memory_t::store_u8(address32_t address, uint8_t byte) {
    uint32_t page = address >> 8;
    auto iter = this->mem_lut.find(page);

    if(iter == this->mem_lut.end()) {
        if(byte) {
            // create a new page
            memory_page_t pg;
            pg.bytes[address & 0xFF] = byte;
            pg.sz++;
            this->mem_lut.insert({ page, pg });
        }
    }
    else {
        uint8_t stored_byte = iter->second.bytes[address & 0xFF];
        iter->second.bytes[address & 0xFF] = byte;

        if(stored_byte && !byte) { // stored data is overwritten with zero
            iter->second.sz--;
        }
        else if(!stored_byte && byte) { // zero is being overwritten with non-zero
            iter->second.sz++;
        }

        if(!iter->second.sz) {
            this->mem_lut.erase(iter);
        }
    }
}

void memory_t::store_u16(address32_t address, uint16_t data) {

    if(this->endianness == this->big_endian)
        data = byte_swap_16(data);

    this->store_u8(address + 0, (data >> 0) & 0xFF);
    this->store_u8(address + 1, (data >> 8) & 0xFF);

}

void memory_t::store_u32(address32_t address, uint32_t data) {
    if(this->endianness == this->big_endian)
        data = byte_swap_32(data);

    this->store_u8(address + 0, (data >> 0)  & 0xFF);
    this->store_u8(address + 1, (data >> 8)  & 0xFF);
    this->store_u8(address + 2, (data >> 16) & 0xFF);
    this->store_u8(address + 3, (data >> 24) & 0xFF);

}

void memory_t::store_u64(address32_t address, uint64_t data) {
    if(this->endianness == this->big_endian)
        data = byte_swap_64(data);

    this->store_u8(address + 0, (data >> 0)  & 0xFF);
    this->store_u8(address + 1, (data >> 8)  & 0xFF);
    this->store_u8(address + 2, (data >> 16) & 0xFF);
    this->store_u8(address + 3, (data >> 24) & 0xFF);
    this->store_u8(address + 4, (data >> 32) & 0xFF);
    this->store_u8(address + 5, (data >> 40) & 0xFF);
    this->store_u8(address + 6, (data >> 48) & 0xFF);
    this->store_u8(address + 7, (data >> 56) & 0xFF);

}

// store signed data
void memory_t::store_i8(address32_t address,  int8_t data) {
    union {
        int8_t i8;
        uint8_t u8;
    };

    i8 = data;
    this->store_u8(address, u8);
}

void memory_t::store_i16(address32_t address, int16_t data) {
    union {
        int16_t i16;
        uint16_t u16;
    };

    i16 = data;
    this->store_u16(address, u16);
}

void memory_t::store_i32(address32_t address, int32_t data) {
    union {
        int32_t  i32;
        uint32_t u32;
    };

    i32 = data;
    this->store_u32(address, u32);
}

void memory_t::store_i64(address32_t address, int64_t data) {
    union {
        int64_t i64;
        uint64_t u64;
    };

    i64 = data;
    this->store_u64(address, u64);
}

size_t memory_t::debug_num_pages(void) {
    return this->mem_lut.size();
}

void memory_t::debug_clear_pages(void) {
    this->mem_lut.clear();
}

std::ostream& operator<<(std::ostream& os, memory_t& mem) {

    auto check_chunk = [](uint8_t* begin, uint8_t* end) -> bool {
        while(begin != end) {
            if(*begin)
                return true;
            begin++;
        }
        return false;
    };

    auto base_address = [](address32_t addr) -> std::string {
        std::string s = "0x";
        for(int i = 7; i >= 0; i--) {
            s.push_back("0123456789ABCDEF"[(addr >> (i*4)) & 0x0F]);
        }
        return s;
    };

    // iterate through all pages
    for(auto& p : mem.mem_lut) {
        // p.second.bytes is the actual memory array

        for(int i = 0; i < 256; i += 16) {
            if(check_chunk(p.second.bytes.data() + i, p.second.bytes.data() + i + 16)) {
                // there is data that needs to be printed
                os << base_address((p.first << 8) + i) << " : ";

                for(int j = 0; j < 16; j++) {
                    os << ' ';
                    os << "0123456789ABCDEF"[p.second.bytes[i+j] >> 4];
                    os << "0123456789ABCDEF"[p.second.bytes[i+j] & 0x0F];
                }

                os << std::endl;

            }
        }

    }

    return os;
}

// print 16-byte chunk
static void print_memory_chunk(address32_t addr, uint8_t* data, std::ostream& os) {

    //std::ostream& os = std::cout;

    auto base_address = [](address32_t addr) -> std::string {
        std::string s = "0x";
        for(int i = 7; i >= 0; i--) {
            s.push_back("0123456789ABCDEF"[(addr >> (i*4)) & 0x0F]);
        }
        return s;
    };

    os << base_address(addr) << " : ";   //needs to double check on this

    for(int j = 0; j < 16; j++) {
        os << ' ';
        os << "0123456789ABCDEF"[data[j] >> 4];
        os << "0123456789ABCDEF"[data[j] & 0x0F];
    }

    os << std::endl;

}

// print 256 byte page
static void print_memory_page(address32_t page_addr, memory_page_t& mp, std::ostream& os) {

    //std::ostream& os = std::cout;
    
    auto check_chunk = [](uint8_t* begin, uint8_t* end) -> bool {
        while(begin != end) {
            if(*begin)
                return true;
            begin++;
        }
        return false;
    };

    for(int i = 0; i < 256; i += 16) {
        if(check_chunk(mp.bytes.data() + i, mp.bytes.data() + i + 16)) {

            print_memory_chunk( page_addr + i, mp.bytes.data() + i, os);
            os << std::endl;

        }
    }

}

//
// NOTE: minimum non-zero set can cause issues here. Passing an empty memory(an instance of memory_t with no operation on it) does not work
//This following example does not work. It makes sense because storing zero on empty memory does not affect memory
/*memory_t mem1(memory_t::little_endian);
    mem1.store_u32(0x0, 0x10);
    memory_t mem2(memory_t::little_endian);
    mem2.store_u32(0x0, 0x0);
    print_diff(mem1, mem2);
*/
void print_memory_diff(memory_t& memory_lhs, memory_t& memory_rhs, std::ostream& os) {

    auto lhs_iter = memory_lhs.begin();
    auto rhs_iter = memory_rhs.begin();

    while( lhs_iter != memory_lhs.end() && rhs_iter != memory_rhs.end() ) {

        if(lhs_iter->first < rhs_iter->first) {
            print_memory_page( lhs_iter->first << 8, lhs_iter->second, os );
            lhs_iter++;
        }
        else if(rhs_iter->first < lhs_iter->first) {
            print_memory_page( rhs_iter->first << 8, rhs_iter->second, os );
            rhs_iter++;
        }
        else {

            memory_page_t& lhs_mp = lhs_iter->second;
            memory_page_t& rhs_mp = rhs_iter->second;

            for(int i = 0; i < 256; i += 16) {
                for(int j = 0; j < 16; j++) {

                    if(lhs_mp.bytes[i + j] != rhs_mp.bytes[i + j]) {
                        print_memory_chunk((rhs_iter->first << 8) | i, rhs_mp.bytes.data() + i, os);
                        break;
                    }
        
                }
            }

            lhs_iter++;
            rhs_iter++;
        }

        //std::cout << " >>\n";
    }
    os << " >>\n\n";

}

auto memory_t::begin() -> std::map<int, memory_page_t>::iterator {
    return this->mem_lut.begin();
}

auto memory_t::end() -> std::map<int, memory_page_t>::iterator {
    return this->mem_lut.end();
}

bool operator==(memory_t& lhs, memory_t& rhs){

    auto lhs_iter = lhs.begin();
    auto rhs_iter = rhs.begin();

    while(lhs_iter != lhs.end() && rhs_iter != rhs.end()) {

        if((*lhs_iter) != (*rhs_iter))
            return false;

        lhs_iter++;
        rhs_iter++;
    }

    return true;
}

bool operator!=(memory_t& memory_lhs, memory_t& memory_rhs){
    return !(memory_lhs == memory_rhs);
}