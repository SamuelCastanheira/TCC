require 'pico.check'

local BeanCounters = {}

local lim_esquerda = {'%',x=0.2, y=0.1} 
local lim_direita = {'%',x=0.8, y=0.1}

local jogando = true

local img_background = "../../imgs/bean_counters/background.png"
local img_caminhao = "../../imgs/bean_counters/trucker.png"
local img_neve = "../../imgs/bean_counters/neve.png"
local img_plataforma = "../../imgs/bean_counters/plataforma.png"

local img_cafe = "../../imgs/bean_counters/Coffe_bag.webp"
local img_ceramica = "../../imgs/bean_counters/Flower_pot.webp"
local img_bigorna = "../../imgs/bean_counters/Anvil.webp"
local img_peixe = "../../imgs/bean_counters/Fish_bean_counters.webp"

local rect_background = {'%', x=0.5, y=0.5, w=1, h=1}
local rect_caminhao = {'%', x=1, y=1, w=0.22, h=0, anchor='SE'}
local rect_neve = {'%', x=0.5, y=0.5, w=1, h=1}
local rect_plataforma = {'%', x=0.15, y=0.85, w=0.25, h=0.25}

local pinguim = {
    img = "../../imgs/bean_counters/penguin0.webp",
    rect = {'%', x=0.3, y=0.97, w=0.15, h=0, anchor='S'},
    speed = 0.08
}

function update()
    local e = pico.input.event()
    if e ~= nil then
        if e.tag == 'key.dn' then
            local next_step = pinguim.rect.x - pinguim.speed
            if e.key == 'Left' and not pico.vs.pos_rect(lim_esquerda, pinguim.rect) then 
                pinguim.rect.x = next_step
            elseif e.key == 'Right' and not pico.vs.pos_rect(lim_direita, pinguim.rect) then 
                local next_step = pinguim.rect.x + pinguim.speed
                pinguim.rect.x = next_step
        end
        elseif e.tag=='quit' then
            pico.quit()
            jogando = false
        end
    end
end

function draw()    
        pico.output.clear()
        pico.output.draw.image(img_background, rect_background)
        pico.output.draw.image(img_neve, rect_neve)
        pico.output.draw.image(img_caminhao, rect_caminhao)
        pico.output.draw.image(img_plataforma, rect_plataforma)
        pico.output.draw.image(pinguim.img, pinguim.rect)
        pico.output.present()
end


function BeanCounters.renderizar()
    pico.set.window{title="Bean Counters"}
    while jogando do
        update()
        draw()
    end
end 

return BeanCounters
