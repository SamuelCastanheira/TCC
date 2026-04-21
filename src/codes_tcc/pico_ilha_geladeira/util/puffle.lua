local Movable = require("util.movable")
local Vetor = require("util.vetor")

local Puffle = {}
Puffle.__index = Puffle

-- Herança: Puffle herda de Movable
setmetatable(Puffle, {__index = Movable})

function Puffle.create(opts)
    opts = opts or {}

    local self = Movable.create(opts)

    setmetatable(self, Puffle)
    return self
end

function Puffle.lista()
    local puffles = {}
    local puffle_paths = {
        "../../../assets/imgs/puffle_roundup/puffle_amarelo.png",
        "../../../assets/imgs/puffle_roundup/puffle_azul.png",
        "../../../assets/imgs/puffle_roundup/puffle_branco.png",
        "../../../assets/imgs/puffle_roundup/puffle_vermelho.png",
        "../../../assets/imgs/puffle_roundup/puffle_verde.png",
        "../../../assets/imgs/puffle_roundup/puffle_laranja.png",
        "../../../assets/imgs/puffle_roundup/puffle_rosa.png",
        "../../../assets/imgs/puffle_roundup/puffle_roxo.png",
        "../../../assets/imgs/puffle_roundup/puffle_preto.png",
        "../../../assets/imgs/puffle_roundup/puffle_marrom.png"
    }

    for i = 1, #puffle_paths do
        puffles[i] = Puffle.create({
            img = puffle_paths[i],
            rect = {   
                '%', 
                x= math.random(30, 70) / 100,
                y= math.random(10, 40) / 100,
                w=0.05, h=0.05
            }
        })
    end
    return puffles
end

return Puffle