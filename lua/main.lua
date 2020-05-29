require("core");   -- exposes armv7_m3 C++ object
--require("memory"); -- exposes memory_t C++ object

function run()
    local cpua = CPU.new();
    cpua.set_register_u32(15, 45); -- ( reg_idx, reg_val )
    print(cpua.get_register_u32(15))
    --pc = cpu.get_register_u32(15);
    --print(pc);


    local cpub = CPU.new();
    cpub.set_register_u32(15, 63)
    print(cpub.get_register_u32(cpub.PC))

    print(cpua.get_register_u32(15))

end

run()

collectgarbage()
--collectgarbage()
