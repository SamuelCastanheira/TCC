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
    self.destino = nil
    self.distancia_destino = 0
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
    self.velocidade = Vetor.new({x=0, y=0})
    self.destino = nil
    self.distancia_destino = 0
    self.em_fuga = false
    self.tempo_fuga = 0
    return self
end

-- ==================== DESTINO ====================

function Movable:ir_para(destino_x, destino_y, velocidade_escalar)
    velocidade_escalar = velocidade_escalar or 0.1
    
    self.destino = {x = destino_x, y = destino_y}
    
    local dx = self.destino.x - self.rect.x
    local dy = self.destino.y - self.rect.y
    local dist = math.sqrt(dx * dx + dy * dy)
    
    self.distancia_destino = dist
    
    if dist > 0 then
        self.velocidade.x = (dx / dist) * velocidade_escalar
        self.velocidade.y = (dy / dist) * velocidade_escalar
    end
    
    return self
end

function Movable:atualizar_destino()
    if not self.destino then return false end
    
    local dx = self.destino.x - self.rect.x
    local dy = self.destino.y - self.rect.y
    local nova_dist = math.sqrt(dx * dx + dy * dy)
    
    self.distancia_destino = nova_dist
    
    -- Chegou ao destino
    if nova_dist < 0.01 then
        self.rect.x = self.destino.x
        self.rect.y = self.destino.y
        self:limpar_movimento()
        return true
    end
    
    return false
end

-- ==================== FUGA ====================

function Movable:ativar_fuga(dir_x, dir_y, velocidade_escalar, duracao)
    velocidade_escalar = velocidade_escalar or 0.1
    duracao = duracao or 0.4
    
    self.velocidade.x = dir_x * velocidade_escalar
    self.velocidade.y = dir_y * velocidade_escalar
    self.em_fuga = true
    self.tempo_fuga = duracao
    self.destino = nil
    self.distancia_destino = 0
    
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

function Movable:esta_dentro(cercado_rect)
    return pico.vs.pos_rect(self.rect, cercado_rect)
end

function Movable:distancia_para(outro)
    local pos1 = self:get_posicao_vetor()
    local pos2 = outro:get_posicao_vetor()
    return pos1:distance(pos2)
end

function Movable:esta_em_movimento()
    return self.destino ~= nil or self.em_fuga
end

function Movable:pode_receber_comando()
    return not self.dentro_cercado and not self.em_fuga
end

return Movable
