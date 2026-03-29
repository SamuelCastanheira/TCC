local Menu = require("menu")
local Loading = require("loading")

pico.init(true)
local phy = {'!', w=800, h=600}
pico.set.view{grid=false}
pico.set.dim(phy)
pico.set.expert(true,10)
Menu.renderizar()
pico.init(false)