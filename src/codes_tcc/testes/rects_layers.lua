pico.init(true)
pico.layer.empty("ui", {w=100, h=100})
pico.set.layer("ui")
local rect_layer = {'%', x=0.5, y=0.5, w=0.3, h=0.3}
pico.output.draw.rect(rect_layer)

pico.set.layer(nil)
pico.set.color.draw('red')
local rect_main = {'%', x=0.35, y=0.25, w=0.1, h=0.1}
pico.output.draw.rect(rect_main)
pico.output.draw.layer("ui", {'%', x=0.4, y=0.3, w=0.3, h=0})

if pico.vs.rect_rect(rect_main, rect_layer) then
    print("DETECTOU COLISAO")
else
    print("NAO DETECTOU COLISAO")
end

pico.input.delay(3000)
pico.init(false)