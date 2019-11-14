#include <memory_pool.h>

memory_t::memory_t(const int endianness) {
    this->endianness = endianness;
}

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

uint16_t memory_t::load_u16(address_t address) {
    uint16_t data, dl, dh;

    dl = this->load_u8(address);
    dh = this->load_u8(address+1);

    data = (dl << 0) | (dh << 8);

    if(this->endianness == this->big_endian)
        

}

uint32_t memory_t::load_u32(address_t address) {

}

uint64_t memory_t::load_u64(address_t address) {

}

int8_t memory_t::load_i8(address_t address) {

}

int16_t memory_t::load_i16(address_t address) {

}

int32_t memory_t::load_i32(address_t address) {

}

int64_t memory_t::load_i64(address_t address) {

}

void memory_t::store_u8(uint32_t address, uint8_t byte) {
    uint32_t page = address >> 8;
    auto iter = this->mem_lut.find(page);

    if(iter == this->mem_lut.end()) {
        if(byte) {
            // create a new page
            memory_t::memory_page_t pg;
            pg.bytes[address & 0xFF] = byte;
            pg.sz++;
            this->mem_lut.insert({ page, pg });
        }
    }
    else {
        uint8_t stored_byte = iter->second.bytes[address & 0xFF];
        iter->second.bytes[address & 0xFF] = byte;

        if(stored_byte && !byte) {
            iter->second.sz--;
        }
        else if(!stored_byte && byte) {
            iter->second.sz++;
        }

        if(!iter->second.sz) {
            this->mem_lut.erase(iter);
        }
    }
}
