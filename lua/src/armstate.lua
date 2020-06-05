
-- wrap raw c functions in Lua object syntax
require("core")  -- CPU object
require("memory") -- MEMORY object

STATE = {}
STATE.new = function() {

    local self = {}

    self.cpu = CPU.new()
    self.memory = MEMORY.new()

    self.next_states = {}

    return self
}

STATE_TABLE = {}
STATE_TABLE.new = function() {

}

state_table = {}
for i=0,15 do
    state_table:insert(i, STATE.new())
end


state_table = {}

st = STATE.new()
state_table:insert(st)

-- initialize memory for st (load code and data)

-- loop

-- fetch, decode, execute
newst = FDE(st)

state_table:insert(newst)


