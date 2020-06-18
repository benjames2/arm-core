
-- specify that lua should look in the src directory as well as the default path
package.cpath = package.cpath .. ';./src/?.so'
--package.path = package.path .. ';./src/?.lua'

require("includes")

math.randomseed(os.time())

start_state = STATE.new()

-- load instructions and data into STATE object
-- ...


-- keep in mind that this operation copies both the cpu state AND all used memory
new_state = STEP(start_state)


