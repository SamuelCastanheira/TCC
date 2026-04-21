local Objeto = require("util.objeto")
local Vetor = require("util.vetor")

local Movable = {}
Movable.__index = Movable

-- Herança: Movable herda de Objeto
setmetatable(Movable, {__index = Objeto})

function Movable.create(opts)
    opts = opts or {}
    local self = Objeto.create(opts)
    
    self.velocidade = opts.velocidade or Vetor.new{ x = 0, y = 0 }
    self.em_fuga = false
    self.tempo_fuga = 0
    self.dentro_cercado = false
    
    setmetatable(self, Movable)
    return self
end

-- ==================== MOVIMENTO ====================

function Movable:atualizar_posicao(dt)
    self.rect.x = self.rect.x + self.velocidade.x * dt
    self.rect.y = self.rect.y + self.velocidade.y * dt
    return self
end

function Movable:set_velocidade(vx, vy)
    self.velocidade = Vetor.new({x=vx, y=vy})
    return self
end

function Movable:parar()
    self.velocidade = Vetor.new({x=0, y=0})
    return self
end

function Movable:limpar_movimento()
    self:parar()
    self.em_fuga = false
    self.fugindo_jogador = false
    self.tempo_fuga = 0
    return self
end

-- ==================== DESTINO ====================

function Movable:calcula_velocidade_vetorial(destino, velocidade_escalar)
    velocidade_escalar = velocidade_escalar or 0.1
    
    
    local delta = Vetor.new({x=destino.x - self.rect.x, y= destino.y - self.rect.y})

    if delta:magnitude() > 0 then
        self.velocidade = delta:normalize(velocidade_escalar):multiply(velocidade_escalar)
        self.fugindo_jogador = true
    else
        self:parar()
    end
    return self
end

function Movable:ativar_fuga(destino, velocidade_escalar, duracao)
    velocidade_escalar = velocidade_escalar or 0.1
    duracao = duracao or 0.4
    self.em_fuga = true
    self.tempo_fuga = duracao
    self:calcula_velocidade_vetorial(destino,velocidade_escalar)
    
    return self
end

function Movable:atualizar_fuga(dt)
    if not self.em_fuga then return false end
    
    self.tempo_fuga = self.tempo_fuga - dt
    
    if self.tempo_fuga <= 0 then
        self.em_fuga = false
        self:parar()
        return true
    end
    
    return false
end

-- ==================== POSIÇÃO ====================

function Movable:get_velocidade_magnitude()
    return self.velocidade:magnitude()
end

function Movable:get_posicao_vetor()
    return Vetor.new({x=self.rect.x, y=self.rect.y})
end

function Movable:set_posicao_vetor(pos)
    self.rect.x = pos.x
    self.rect.y = pos.y
    return self
end

function Movable:revert_posicao(pos_anterior)
    self.rect.x = pos_anterior.x
    self.rect.y = pos_anterior.y
    return self
end

-- ==================== VERIFICAÇÕES ====================

function Movable:distancia_para(outro)
    local pos1 = self:get_posicao_vetor()
    local pos2 = outro:get_posicao_vetor()
    return pos1:distance(pos2)
end



return Movable
