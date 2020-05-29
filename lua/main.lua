
-- specify that lua should look in the src directory as well as the defaults
package.cpath = package.cpath .. ';./src/?.so'

require("src.core");   -- exposes armv7_m3 C++ object
--require("memory"); -- exposes memory_t C++ object

function run()

    local cpua = CPU.new();
    cpua.set_register_u32(15, 45); -- ( reg_idx, reg_val )
    print(cpua.get_register_u32(15))

    local cpub = CPU.new();
    cpub.set_register_u32(15, 63)

    print(cpub.get_register_u32(CPU.PC))
    
    print(cpua.get_register_u32(15))

    print(cpua)

end

run()

cpuc = CPU.new()

for i = 0, 15 do
    cpuc.set_register_u32(i, i*4 + 1)
end

for i = 0, 15 do
    print(i .. ' ' .. cpuc[i])
end
--print(cpuc[16]) -- <<< produces program crash via C++ exception

f = cpuc.get_flags()
print(f)

cpuc = nil

print('At the end of the program...')


