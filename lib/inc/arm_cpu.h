#ifndef arm_cpu_h
#define arm_cpu_h

#include <array>
#include <iostream>

class arm_cpu {
private:

    union register_t {
        uint32_t u32;
        int32_t  i32;
        float    f32;
    };

    std::array<register_t, 32> i_register_file;

public:

    arm_cpu(void);

};

#endif // arm_cpu_h