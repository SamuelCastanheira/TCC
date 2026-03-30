function rect_in_main(layer_pos, rect)
    pico.set.layer(nil)

    local w = layer_pos.w
    local h = layer_pos.h

    local x0 = layer_pos.x - w/2
    local y0 = layer_pos.y - h/2

    local local_x = x0 + rect.x * w
    local local_y = y0 + rect.y * h
    local local_w = rect.w*w
    local local_h = rect.h*h

    return {
        '%',
        x = local_x,
        y = local_y,
        w = local_w,
        h = local_h
    }
end

local rect_layer = {'%', x=0.5, y=0.5, w=0.3, h=0.3}
local rect_main = {'%', x=0.35, y=0.25, w=0.1, h=0.1}

pico.init(true)
pico.layer.empty("ui", {w=100, h=100})
pico.set.layer("ui")
pico.output.draw.rect(rect_layer)

local rect_layer_in_main = rect_in_main({'%', x=0.4, y=0.3, w=0.3, h=0.3}, rect_layer)
if pico.vs.rect_rect(rect_main, rect_layer_in_main) then
    print("DETECTOU COLISAO")
else
    print("NAO DETECTOU COLISAO")
end

pico.set.layer(nil)
pico.set.color.draw('red')
pico.output.draw.rect(rect_main)
pico.output.draw.layer("ui", {'%', x=0.4, y=0.3, w=0.3, h=0.3})

pico.input.delay(3000)
pico.init(false)