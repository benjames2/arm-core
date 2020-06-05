
-- cpp memory_t object
local memwrap = require("memory_wrapper")

MEMORY = {
    little_endian = 0,
    big_endian = 1
}

MEMORY.new = function(endianness)
    if endianness == nil then
        endianness = MEMORY.little_endian
    end

    local self = {}
    self.memory = memwrap.new(endianness)

    setmetatable(
        self,
        {
            __gc = function(obj)
                memwrap.delete(obj.memory)          
            end,

            __tostring = function(obj)
                return memwrap.to_string(obj.memory)
            end,

        })

    self.store_u8  = function(addr, data) memwrap.store_u8(self.memory, addr, data) end
    self.store_u16 = function(addr, data) memwrap.store_u16(self.memory, addr, data) end
    self.store_u32 = function(addr, data) memwrap.store_u32(self.memory, addr, data) end
    self.store_i8  = function(addr, data) memwrap.store_i8(self.memory, addr, data) end
    self.store_i16 = function(addr, data) memwrap.store_i16(self.memory, addr, data) end
    self.store_i32 = function(addr, data) memwrap.store_i32(self.memory, addr, data) end

    return self;
end

MEMORY.debugNumObjects = function()
    return memwrap.debugNumObjects()
end


