function mouse_in_layer(layer_pos)
    pico.set.layer(layer)
    local layer_view = pico.get.view()
    pico.set.layer(nil)

    local mouse = pico.get.mouse('%')

    local w = layer_pos.w
    local h = layer_pos.h

    local x0 = layer_pos.x - w/2
    local y0 = layer_pos.y - h/2

    local local_x = (mouse.x - x0) / w
    local local_y = (mouse.y - y0) / h

    return {
        '%',
        x = local_x,
        y = local_y,
    }
end


pico.init(true)

pico.layer.empty("ui", {w=100, h=100})
pico.set.layer("ui")
local rect_layer = {'%', x=0.5, y=0.5, w=0.3, h=0.3}
pico.output.draw.rect(rect_layer)
local i = 0

pico.set.layer(nil)
pico.set.expert(true,20)
while true do
    local e = pico.input.event()
    mouse = mouse_in_layer({'%', x=0.4, y=0.3, w=0.3, h=0.3})
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