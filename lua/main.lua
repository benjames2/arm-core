
-- specify that lua should look in the src directory as well as the default path
package.cpath = package.cpath .. ';./src/?.so'
--package.path = package.path .. ';./src/?.lua'

require("core")
require("memory")
require("armstate")
require("loop")

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
        table.insert(cputable, CPU.new( rng_reg ))
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

collectgarbage("count")

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

-- spawn a bunch of memory_t objects
for i=0,100 do
    --print("Lua memory usage : " .. tostring(collectgarbage("count")) .. " kB")
    local memobj = MEMORY.new(MEMORY.little_endian)

    if i == 100 then

        for j=0,100 do
            memobj.store_u8(j, math.random(1, 255))
        end

        print(tostring(memobj))

        for j=0,9 do
            print(string.format("0x%08d : %02x ", j, memobj.load_u8(j)))
        end
        print() -- newline

    end

end

--print("Lua memory usage : " .. tostring(collectgarbage("count")) .. " kB")
print(string.format("CPU objects    : %d", CPU.debugNumObjects()))
print(string.format("MEMORY objects : %d", MEMORY.debugNumObjects()))

print("collecting garbage...")
collectgarbage()
collectgarbage()

--print("Lua memory usage : " .. tostring(collectgarbage("count")) .. " kB")
print(string.format("CPU objects    : %d", CPU.debugNumObjects()))
print(string.format("MEMORY objects : %d", MEMORY.debugNumObjects()))

