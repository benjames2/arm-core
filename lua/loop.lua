

fde_loop = require("execution_loop_wrapper")

-- step function returns new cpu state after executing a single instruction
function STEP(a)

    local newa = STATE.copy(a)

    fde_loop.step(newa.cpu.cpu, newa.memory.memory)
    
    return newa
end
