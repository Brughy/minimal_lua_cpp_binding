map = {
    pos = {
    x = 666, 
    y = 2550
    },
    command = "merge"
}

array = {0, 1, 2, 3, 55, 10, 20}
names = {"ciccio", "pasticcio"}
  
a = 0
function add (a)
  local sum = 10
  sum = sum + a
  return sum, 50
end

function addN (a,b)
  local sum = 10
  sum = sum + a + b 
  return sum, 50
end

do
	function double (a)
		local lib = require("libmylib")
		r = lib.double(a)
		return r
	end
end
