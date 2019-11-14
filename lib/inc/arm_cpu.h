#ifndef arm_cpu_h
#define arm_cpu_h

#include <array>
#include <iostream>
#include <register_names.h>

class arm_cpu {
private:

    union register_t {
        uint32_t u32;
        int32_t  i32;
        float    f32;
    };

    std::array<register_t, 32> register_file;

    uint32_t APSR; // Application Program Status Register

public:

    arm_cpu(void);

    int32_t  get_register_int(  const int reg);
    uint32_t get_register_uint( const int reg);
    float    get_register_fp(   const int reg);

    void set_register_int(  const int reg, int32_t val);
    void set_register_uint( const int reg, uint32_t val);
    void set_register_fp(   const int reg, float val);

    uint32_t get_program_status_register(void);

    bool get_flag_negative(void);
    bool get_flag_zero(void);
    bool get_flag_carry(void);
    bool get_flag_overflow(void);
    bool get_flag_saturate(void);

    void set_flag_negative( bool val );
    void set_flag_zero(     bool val );
    void set_flag_carry(    bool val );
    void set_flag_overflow( bool val );
    void set_flag_saturate( bool val );

};

#endif // arm_cpu_h