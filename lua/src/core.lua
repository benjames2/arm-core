
-- wrap raw c functions in Lua object syntax
armcore = require("core_wrapper");

CPU = {
    PC = 15,
    LR = 14,
    SP = 13,
}

CPU.new = function(initcallback)
    local self = {}
    --self.__index = self

    self.cpu = armcore.new() -- returns a userdata

    -- lua supports a lot of metamethods placed via metatable
    setmetatable(
        self, 
        { -- how bout them dunder functions boys
            __gc = function(obj) -- implicit garbage collection method
                armcore.delete(obj.cpu) 
            end,
            
            __tostring = function(obj) -- implicit string conversion method
                return armcore.to_string(obj.cpu)
            end,

            __index = function(obj, key) -- fetch the correct register value, error checking is done in the C++ 
                                         -- implementation and may throw an uncaught exception which will crash 
                                         -- the entire program
                return armcore.get_register_u32(obj.cpu, key)
            end,

        })

    -- get various flags. returns table of flag values
    function self.get_flags()
        N, Z, C, V, Q = armcore.get_all_flags(self.cpu)

        flags = {
            N = N,
            Z = Z,
            C = C,
            V = V,
            Q = Q
        }

        -- it is useful to be able to print the values of the flags
        setmetatable(
            flags,
            {
                __tostring = function(obj)
                    return "N Z C V Q\n" .. string.format("%d %d %d %d %d", obj.N, obj.Z, obj.C, obj.V, obj.Q)
                end,
            })
        
        return flags;

    end

    -- 'methods' are really just wrappers over functions in the armcore table
    function self.set_register_u32(reg_idx, reg_val) armcore.set_register_u32(self.cpu, reg_idx, reg_val) end
    function self.set_register_i32(reg_idx, reg_val) armcore.set_register_i32(self.cpu, reg_idx, reg_val) end
    function self.get_register_u32(reg_idx) return armcore.get_register_u32(self.cpu, reg_idx) end
    function self.get_register_i32(reg_idx) return armcore.get_register_i32(self.cpu, reg_idx) end

    -- convenience functions for fetching specific registers
    function self.get_PC() return armcore.get_register_u32(self.cpu, CPU.PC) end
    function self.get_LR() return armcore.get_register_u32(self.cpu, CPU.LR) end
    function self.get_SP() return armcore.get_register_u32(self.cpu, CPU.SP) end

    -- generate ASCII string suitable for file storage
    function self.serialize() return armcore.serialize(self.cpu) end

    -- optionally pass an initialization function to CPU.new
    if initcallback ~= nil then
        initcallback(self)        
    end

    return self;
end

CPU.deserialize = function(s)
    local self = CPU.new()

    -- the existing cpu object is simply modified by the deserialize function
    armcore.deserialize(s, self.cpu)
    return self
end


