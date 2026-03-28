require 'pico.check'
pico.init(true)

local qnt_cores = 15

local phy = {'!', w=1280, h=720}
pico.set.window{title="Personalização"}
pico.set.dim(phy)
pico.set.expert(true,60)

local background = {'%', x=0.5, y=0.5, w=1, h=1}
local pinguim = {'%', x=0.3, y=0.35, w=0.3, h=0}
local voltar = {'%', x=0.3, y=0.8, w=0.3, h=0}
local texto_escolha = {'%', x=0.60, y=0.20, w=0.38, h=0.1, anchor='NW'}

pico.output.clear()
pico.output.draw.image("../imgs/background_personalizar.png", background)
pico.output.draw.image("../imgs/personalizar/pinguim_amarelo.png", pinguim)
pico.output.draw.image("../imgs/botoes/b_voltar.png", voltar)
pico.output.draw.text("Escolha sua cor", texto_escolha)

pico.layer.empty("quadro_gelo", {w=600, h=400})
pico.set.layer("quadro_gelo")
pico.set.view{tile={w=85, h=80}}
local quadro_gelo = {'#', x=1, y=1, w=7, h=5, anchor='NW'}
pico.output.draw.image("../imgs/personalizar/quadro.png", quadro_gelo)

local base = "../imgs/personalizar"

for i = 0, qnt_cores - 1 do
    local path = string.format("%s/%d.png", base, i)

    pico.output.draw.image(
        path,
        {
            '#',
            x = (i % 5) + 2,
            y = 2 + math.floor(i / 5),
            w = 1,
            h = 1,
            anchor = 'NW'
        }
    )
end

pico.set.layer(nil)
pico.output.draw.layer("quadro_gelo", {'%', x=0.55, y=0.25,  anchor='NW'})

pico.output.present()
pico.input.delay(5000)

pico.init(false)