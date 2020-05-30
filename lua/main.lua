
-- specify that lua should look in the src directory as well as the default path
package.cpath = package.cpath .. ';./src/?.so'

require("src.core");   -- exposes armv7_m3 C++ object
--require("src.memory"); -- exposes memory_t C++ object

--[[
    CPU.new            : creates new armv7_m3 object from scratch. registers are initialized as expected
    CPU.deserialize    : creates new armv7_m3 object from string-representation of cpu state
    <cpuobj>.serialize : creates string-representation that can be used with CPU.deserialize to prefectly 
                         reconstruct cpu state. any cpu object can be serialized

    because the (de)serialization methods use the C++ (de)serialization methods directly, results can be 
    freely passed between C++ and Lua. same protocol is used

--]]

math.randomseed(os.time())

function rng_reg(cpu)
    -- generate random values for all of the registers
    for i=0,15 do
        cpu.set_register_u32(i, math.random(0, 1000000000))
    end

end

-- just testing the garbage collector and making sure my __gc metamethod works
--[[do
    for i=0,1000000 do
        local studidcpu = CPU.new()
        --studidcpu = nil
        --collectgarbage()
    end
end
]]

function run()

    local cpua = CPU.new();
    cpua.set_register_u32(15, 45); -- ( reg_idx, reg_val )
    print(cpua.get_register_u32(15))

    local cpub = CPU.new();
    cpub.set_register_u32(15, 63)

    print(cpub.get_register_u32(CPU.PC))
    print(cpua.get_register_u32(15))
    print(cpua)

    print("printing cpu flags for `cpua'")
    print(cpua.get_flags())

    -- it is trivial to create N separate cpu instances
    local cputable = {}
    for i=1,15 do
        table.insert(cputable, CPU.new( function(cpu) cpu.set_register_u32(15, 10000) end ))
        --print(cputable[i])
    end

    local serialized = {}
    for k, v in pairs(cputable) do
        table.insert(serialized, v.serialize())
    end
    return cputable, serialized
end

cpulist, cpuser = run()

file = io.open('serialized.txt', 'w') -- clear existing file

print('list of serialized CPU states')
for k,v in pairs(cpuser) do
    print((k-1) .. ': ' .. v)

    -- print serialized cpu states to file
    file:write(v .. '\n')
end
file:close()

-- deserialize all of these cpu states
newcpulist = {}
for k,v in pairs(cpuser) do
    newcpulist[k] = CPU.deserialize(v)
end

print(tostring(newcpulist[1]))

-- fetch pre- and post- serialization cpus and compare them
cpu_0 = cpulist[1]
cpu_1 = newcpulist[1]

-- if the serialization process worked, these should all equal 0
for i=0,15 do
    print(cpu_0.get_register_u32(i) - cpu_1.get_register_u32(i))
end

print(newcpulist)

