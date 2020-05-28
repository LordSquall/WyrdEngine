printMessage("You can call C++ functions from Lua!")

sumNumbers = function(a,b)
    printMessage("You can still call C++ functions from Lua functions!")
    return a + b
end
