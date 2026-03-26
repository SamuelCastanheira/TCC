require 'pico.check'
pico.init(true)

local qnt_cores = 15

pico.set.window{title="Personalização", fullscreen = true}
local win = pico.get.window()
print(win.dim.w, win.dim.h)


local all = pico.get.view()
print(all.dim.w, all.dim.h)

local background = {'%', x=0.5, y=0.5, w=1, h=1}
local pinguim = {'%', x=0.3, y=0.35, w=0.3, h=0}
local voltar = {'%', x=0.3, y=0.8, w=0.3, h=0}

pico.output.draw.image("imgs/background_personalizar.png", background);

pico.input.delay(5000)

pico.init(false)