local PegaPuffle = {}
local Puffle = require("util.puffle")
local Vetor = require("util.vetor")

-- ==================== UTILITÁRIOS ====================
--criar um lista.lua
local function removerPuffle(lista, alvo)
    for i, v in ipairs(lista) do
        if v == alvo then
            table.remove(lista, i)
            return 1
        end
    end
end

local function verificaColisaoComParede(puffle, hitboxes)
    for nome, parede in pairs(hitboxes) do
        if pico.vs.rect_rect(puffle.rect, parede) then
            return true, nome
            
        end
    end
    return false, nil
end

local function calculaDirecaoFuga(puffle, mouse, v_direcao)
    -- Direção contrária ao mouse
    local dx = puffle.rect.x - mouse.x
    local dy = puffle.rect.y - mouse.y
    local v_mouse = Vetor:new({x= dx, y= dy})
    v_mouse:normalize()
    
    -- Combina com direção do obstáculo
    local v_final = v_mouse:add(v_direcao)
    v_final:normalize()
    
    return v_final
end

-- ==================== INICIALIZAÇÃO ====================

function PegaPuffle.init(state)
    math.randomseed(pico.get.now())
    
    state.pegaPuffleData = {
        puffles = Puffle.lista(),
        hitboxes = {
            esquerda = {'%', x=0.43, y=0.65, w=0.02, h=0.3, anchor='C'},
            direita =  {'%', x=0.58, y=0.65, w=0.02, h=0.3, anchor='C'},
            baixo =    {'%', x=0.5, y=0.8, w=0.20, h=0.02, anchor='C'}
        },
        distancia_max = 0.1,
        velocidade_esc = 0.05,
        duracao_fuga = 0.4,
        cercado = {'%', x=0.5, y=0.65, w=0.15, h=0.25},
    }
end

function PegaPuffle.update(state, event)
    local data = state.pegaPuffleData
    local mouse = pico.get.mouse('%')
    local dt = 0.05
    
    -- Processa evento de mouse
    if event and event.tag == 'mouse.motion' then
        for _, puffle in ipairs(data.puffles) do
            if puffle:pode_receber_comando() then
                local dx = mouse.x - puffle.rect.x
                local dy = mouse.y - puffle.rect.y
                local dist = math.sqrt(dx * dx + dy * dy)
                
                if dist > 0.001 and dist <= data.distancia_max then
                    -- Calcula reflexão e move para lá
                    local destino_x = mouse.x - (2 * dx)
                    local destino_y = mouse.y - (2 * dy)
                    puffle:ir_para(destino_x, destino_y, data.velocidade_esc)
                end
            end
        end
    end
    
    -- Atualiza movimento de cada puffle
    for _, puffle in ipairs(data.puffles) do
        --puffle.dentro_cercado = puffle:esta_dentro(data.cercado)
        
        if puffle.dentro_cercado then
            --puffle:limpar_movimento()
        elseif puffle.em_fuga then
            
            puffle:atualizar_posicao(dt)
            puffle:atualizar_fuga(dt)

        elseif puffle.destino then
            -- Movimento normal em direção ao destino
            puffle.rect.x = puffle.rect.x + puffle.velocidade.x * dt
            puffle.rect.y = puffle.rect.y + puffle.velocidade.y * dt  

            local dx = puffle.rect.x- mouse.x
            local dy = puffle.rect.y - mouse.y

            local nova_dist = math.sqrt(dx * dx + dy * dy)
            puffle.distancia_destino = nova_dist
            
            -- Chegou ao destino
            if nova_dist > data.distancia_max then
                puffle:limpar_movimento()    
            end    
        end
        
        -- Limites da tela - mesma lógica de fuga
        local limites = {
            {puffle.rect.x < 0.1, 0.1, {x=1, y=0}},    -- esquerda -> foge direita
            {puffle.rect.x > 0.9, 0.9, {x=-1, y=0}},   -- direita -> foge esquerda
            {puffle.rect.y < 0.1, 0.1, {x=0, y=1}},    -- cima -> foge baixo
            {puffle.rect.y > 0.95, 0.95, {x=0, y=-1}}    -- baixo -> foge cima
        }
        
        for i=1,4 do
            local limite = limites[i]
            if limite[1] then
                if i <= 2 then puffle.rect.x = limite[2] else puffle.rect.y = limite[2] end
                local v_dir = calculaDirecaoFuga(puffle, mouse, limite[3])
                puffle:ativar_fuga(v_dir.x, v_dir.y, data.velocidade_esc, data.duracao_fuga)
            end
        end

        local colidiu, parede_nome = verificaColisaoComParede(puffle, data.hitboxes)
            if colidiu then
                print(parede_nome)
                -- Calcula direção de fuga
                local cerca_outside = {esquerda = {x=-1, y=0}, 
                                    direita = {x=1, y=0}, 
                                    baixo = {x=0, y=1}}

                local cerca_inside =  {esquerda = {x=1, y=0}, 
                                        direita = {x=-1, y=0}, 
                                        baixo = {x=0, y=-1}} 

                local direcoes_fuga = puffle:esta_dentro(data.cercado) and cerca_inside or cerca_outside
                local direcao = direcoes_fuga[parede_nome] or {0, 0}
                local v_dir = calculaDirecaoFuga(puffle, mouse, direcao)
                
                -- Ativa fuga
                puffle:ativar_fuga(v_dir.x, v_dir.y, data.velocidade_esc, data.duracao_fuga)
            end
    end
    
    -- Verifica vitória
    local todos_dentro = true
    for _, puffle in ipairs(data.puffles) do
        if not puffle:esta_dentro(data.cercado) then
            todos_dentro = false
            break
        end
    end
    
    if todos_dentro then
        state.nextScreen = "centro"
    end
end

function PegaPuffle.draw(state)
    local data = state.pegaPuffleData
    local background = {'%', x=0.5, y=0.5, w=1, h=1}
    local arvores = {'%', x=0.5, y=0.5, w=1, h=1}
    
    pico.output.draw.image("../../../assets/imgs/puffle_roundup/background.png", background)
    
    for _, puffle in ipairs(data.puffles) do
        pico.output.draw.image(puffle.img, puffle.rect)
    end
    
    -- Debug
    --pico.output.draw.rect(data.cercado, {r=100, g=200, b=100, a=50})
    --pico.output.draw.rect(data.hitboxes.esquerda, {r=100, g=200, b=100, a=50})
    --pico.output.draw.rect(data.hitboxes.direita, {r=100, g=200, b=06, a=50})
    --pico.output.draw.rect(data.hitboxes.baixo, {r=10, g=200, b=100, a=50})
    pico.output.draw.image("../../../assets/imgs/puffle_roundup/arvores.png", arvores)
end

function PegaPuffle.finish(state)
    state.pegaPuffleData = nil
    state.money = 100
end

return PegaPuffle