#ifndef memory_pool
#define memory_pool

#include <map>
#include <array>
#include <iostream>
#include <cstddef>

typedef uint32_t   address_t;
typedef uint64_t   address64_t;

typedef uint8_t    half_word_t;
typedef uint16_t   word_t;
typedef uint32_t   double_word_t;
typedef uint64_t   quad_word_t;
typedef int8_t     signed_half_word_t;
typedef int16_t    signed_word_t;
typedef int32_t    signed_double_word_t;
typedef int64_t    signed_quad_word_t;

// these routines are written in assembly
extern "C" {
    uint64_t byte_swap_64(uint64_t b);
    uint32_t byte_swap_32(uint32_t b);
    uint16_t byte_swap_16(uint16_t b);
}

class memory_t {
private:

    struct memory_page_t {
        
        std::array<uint8_t, 256> bytes;

        int sz; // number of non-zero entries. used to
                // determine when pages should be removed

        memory_page_t(void) {

            this->sz = 0;
            for(int i = 0; i < 256; i++)
                this->bytes[i] = 0x00; // may want to just count the number of non-zero entries
        }
    };

    // every entry : { 24-bit page number, 256 byte chunk }
    std::map<int, memory_page_t> mem_lut;

    int endianness;

public:

    // endianness of memory is important
    static const int little_endian = 0;
    static const int big_endian = 1;

    memory_t(const int endianness);
    memory_t(const memory_t&);

    int get_endianness(void) const;

    size_t debug_num_pages(void);
    void debug_clear_pages(void);

    auto begin(void) -> std::map<int, memory_page_t>::iterator;
    auto end(void) -> std::map<int, memory_page_t>::iterator;
    auto cbegin(void) const noexcept -> std::map<int, memory_page_t>::const_iterator;
    auto cend(void) const noexcept -> std::map<int, memory_page_t>::const_iterator;

    // get a specific byte from memory. can request data from
    // anywhere in the 32-bit memory space
    uint8_t  load_u8(address_t address);
    uint16_t load_u16(address_t address);
    uint32_t load_u32(address_t address);
    uint64_t load_u64(address_t address);

    // load signed data
    int8_t  load_i8(address_t address);
    int16_t load_i16(address_t address);
    int32_t load_i32(address_t address);
    int64_t load_i64(address_t address);

    // store data anywhere in the address space.
    // storing zeros does not actually increase the
    // amount of memory used by memory_t
    void store_u8(address_t address,  uint8_t data);
    void store_u16(address_t address, uint16_t data);
    void store_u32(address_t address, uint32_t data);
    void store_u64(address_t address, uint64_t data);

    // store signed data
    void store_i8(address_t address,  int8_t data);
    void store_i16(address_t address, int16_t data);
    void store_i32(address_t address, int32_t data);
    void store_i64(address_t address, int64_t data);

    friend std::ostream& operator<<(std::ostream& os, memory_t& mem);

};

#endif // memory_pool
