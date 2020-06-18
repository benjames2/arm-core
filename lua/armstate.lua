
-- wrap raw c functions in Lua object syntax
require("core")   -- CPU object
require("memory") -- MEMORY object

STATE = {}
STATE.new = function()

    local self = {}

    self.cpu = CPU.new()
    self.memory = MEMORY.new()

    -- userdata ptrs are:
    --   self.cpu.cpu
    --   self.memory.memory

    return self
end

-- returns a deep copy of the given STATE object
STATE.copy = function(state)

    local self = {}

    self.cpu = CPU.copy(state.cpu.cpu)
    self.memory = MEMORY.copy(state.memory.memory)

    return self;
end

