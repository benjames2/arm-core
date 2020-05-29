
-- wrap raw c functions in Lua object syntax
armcore = require("core_wrapper");

CPU = {}
--CPU.__index = CPU

CPU.new = function()
    local self = {}
    self.__index = armcore

    self.PC = 15
    self.LR = 14
    self.SP = 13

    self.cpu = armcore.new()

    --function self.__gc(obj)
    --    print("cpu is being collected...")
    --    armcore.delete(self.cpu)
    --end

    -- specify garbage collection method
    setmetatable(self, { __gc = function(obj) armcore.delete(self.cpu) end} )

    function self.set_register_u32(reg_idx, reg_val) 
        armcore.set_register_u32(self.cpu, reg_idx, reg_val) 
    end

    function self.get_register_u32(reg_idx) 
        return armcore.get_register_u32(self.cpu, reg_idx) 
    end

    return self;
end


--[[
-- we can write the method access stuff in Lua
function CPU.new()

    local thisobj = { }
    --setmetatable( thisobj, armcore )

    thisobj.cpu = armcore.new()

    return thisobj;

--]]
    --thisobj.get_register_u32 = function(idx) return armcore.getregu32(self.cpu, idx) end
    --get_register_i32 = function(idx) return armcore.getregi32(self.cpu, idx) end
    --thisobj.set_register_u32 = function(idx, val) armcore.setregu32(self.cpu, idx, val) end
    --set_register_i32 = function(idx, val) armcore.setregi32(self.cpu, idx, val) end

    --get_PC = function() return armcore.getregu32(self.cpu, CPU.PC) end
    --get_LR = function() return armcore.getregu32(self.cpu, CPU.LR) end
    --get_SP = function() return armcore.getregu32(self.cpu, CPU.SP) end

    --__gc = function(cpu) armcore.delete(cpu) end

    --[[
    this.get_register_u32 = function(idx) return armcore.getregu32(this.cpu, idx) end
    this.get_register_i32 = function(idx) return armcore.getregi32(this.cpu, idx) end
    this.set_register_u32 = function(idx, val) armcore.setregu32(this.cpu, idx, val) end
    this.set_register_i32 = function(idx, val) armcore.setregi32(this.cpu, idx, val) end

    this.get_PC = function() return armcore.getregu32(this.cpu, CPU.PC) end
    this.get_LR = function() return armcore.getregu32(this.cpu, CPU.LR) end
    this.get_SP = function() return armcore.getregu32(this.cpu, CPU.SP) end

    this.__gc = function() print("GC'd cpu") end
    --]]

--end



