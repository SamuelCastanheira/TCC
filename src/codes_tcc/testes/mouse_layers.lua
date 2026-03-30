pico.init(true)

pico.layer.empty("ui", {w=100, h=100})
pico.set.layer("ui")
local rect_layer = {'%', x=0.5, y=0.5, w=0.3, h=0.3}
pico.output.draw.rect(rect_layer)
local i = 0

pico.set.layer(nil)
pico.set.expert(true,1)
while true do
     local e = pico.input.event()
        mouse = pico.get.mouse()
        if e ~= nil then
            if e.tag=='quit' then
                pico.quit()
                break
            end
        end
    if pico.vs.pos_rect(mouse, rect_layer) then
        print("DETECTOU COLISAO")
    else
        print("NAO DETECTOU COLISAO")
    end
    pico.output.draw.layer("ui", {'%', x=0.4, y=0.3, w=0.3, h=0.3})
    pico.output.present()
end
pico.init(false)