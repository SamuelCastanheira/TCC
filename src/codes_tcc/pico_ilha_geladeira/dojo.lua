local Dojo = {}

-- Constantes
local NUM_CARTAS_PLAYER = 3
local NUM_CARTAS_POOL = 5

--Cartas do turno
local cartaSelecionada = nil
local cartaNPCSelecionada = nil


local Elemento = {
    GELO = 1,
    AGUA = 2,
    FOGO = 3
}

local DuelState = {
    SELECIONANDO = 1,
    COMPARANDO = 2,
    MOSTRANDO_RESULTADO = 3,
    FINALIZADO = 4
}

local ResultadoState = {
    EMPATE = 0,
    GANHOU = 1,
    PERDEU = -1
}

-- Carta
local function novaCarta(elemento, pontuacao, path)
    return {
        elemento = elemento,
        pontuacao = pontuacao,
        path = path,
        rect = {}
    }
end

local function deepCopy(orig)
    local copia = {}
    for k, v in pairs(orig) do
        if type(v) == "table" then
            copia[k] = deepCopy(v)
        else
            copia[k] = v
        end
    end
    return copia
end

-- Sorteio
local function sortearCartas(pool)
    local destino = {}
    local usados = {}
    local pool_copy = deepCopy(pool)
    while #destino < NUM_CARTAS_PLAYER do
        local idx = math.random(1,#pool_copy)
        if not usados[idx] then
            usados[idx] = true
            table.insert(destino, pool_copy[idx])
        end
    end

    return destino
end

-- Comparação
local function compararCartas(jogador, npc)
    if jogador.elemento == npc.elemento then
        if jogador.pontuacao > npc.pontuacao then return ResultadoState.GANHOU
        elseif jogador.pontuacao < npc.pontuacao then return ResultadoState.PERDEU
        else return ResultadoState.EMPATE end
    end

    if (jogador.elemento == Elemento.GELO and npc.elemento == Elemento.AGUA) or
       (jogador.elemento == Elemento.AGUA and npc.elemento == Elemento.FOGO) or
       (jogador.elemento == Elemento.FOGO and npc.elemento == Elemento.GELO) then
        return ResultadoState.GANHOU
    else
        return ResultadoState.PERDEU
    end
end

-- Remoção por referência
local function removerCarta(array, carta)
    for i, c in ipairs(array) do
        if c == carta then
            table.remove(array, i)
            return
        end
    end
end

-- Estado
local dueloEstado = DuelState.SELECIONANDO
local cartasJogador = {}
local cartasNPC = {}

local jogadorScore = 0
local npcScore = 0
local resultado = -1

local mostrandoResultado = false
local tempoResultado = 0
local DURACAO_RESULTADO = 2 -- milisegundos

local running = true

function ordena_mao(mao, jogador)
    local x_inicial = 0.1
    if (jogador) then
        x_inicial = 0.7
    end
    print(#mao)
    for i, carta in ipairs(mao) do
        carta.rect = {
            '%', 
            x = x_inicial + (i - 1) * 0.1, 
            y=0.88, 
            w=0.3, 
            h=0.3}  
    end
end

function desenha_mao(mao, jogador)
    local verso = "../../../assets/imgs/dojo/carta_azul.png"
    for _, carta in ipairs(mao) do
        if jogador then
            pico.output.draw.image(carta.path, carta.rect)
        else
             pico.output.draw.image(verso, carta.rect)
        end  
    end
end

function Dojo.renderizar()
    pico.set.window{title="Dojo"}
    math.randomseed(os.time())

    local background = {'%', x=0.5, y=0.5, w=1, h=1}

    local cartas_pool = {
        novaCarta(Elemento.AGUA, 3, "../../../assets/imgs/dojo/agua_3.png"),
        novaCarta(Elemento.FOGO, 4, "../../../assets/imgs/dojo/fogo_4.png"),
        novaCarta(Elemento.FOGO, 7, "../../../assets/imgs/dojo/fogo_7.png"),
        novaCarta(Elemento.GELO, 5, "../../../assets/imgs/dojo/gelo_5.png"),
        novaCarta(Elemento.GELO, 6, "../../../assets/imgs/dojo/gelo_6.png")
    }

    cartasJogador = sortearCartas(cartas_pool)
    cartasNPC = sortearCartas(cartas_pool)

    ordena_mao(cartasNPC,false)
    ordena_mao(cartasJogador, true)
    
    local rect_carta_jogador = {'%', x=0.2 , y=0.3 , w=0.7, h= 0.7}
    local rect_carta_NPC = {'%', x=0.8 , y=0.3 , w=0.7 , h= 0.7}

    while true do
        local e = pico.input.event()
        local mouse = pico.get.mouse('%')
        local agora = os.clock()
        print(agora)

        if e ~= nil then
            if e.tag=='quit' then
                break
            end
            if e.tag == 'mouse.button.dn' and dueloEstado == DuelState.SELECIONANDO then
                for _, carta in ipairs(cartasJogador) do
                    if pico.vs.pos_rect(mouse, carta.rect) then
                        cartaSelecionada = carta
                        break
                    end
                end
            end
        end

        if dueloEstado == DuelState.SELECIONANDO then
            if cartaSelecionada and not cartaNPCSelecionada then
                cartaNPCSelecionada = cartasNPC[math.random(1, #cartasNPC)]
            end

            if cartaSelecionada and cartaNPCSelecionada then
                dueloEstado = DuelState.COMPARANDO
            end

        elseif dueloEstado == DuelState.COMPARANDO then
            
            resultado = compararCartas(cartaSelecionada, cartaNPCSelecionada)

            if resultado == ResultadoState.GANHOU then 
                jogadorScore = jogadorScore + 3
            elseif resultado == ResultadoState.PERDEU then 
                npcScore = npcScore + 3
            else
                jogadorScore = jogadorScore + 1
                npcScore = npcScore + 1
            end

            tempoResultado = agora
            dueloEstado = DuelState.MOSTRANDO_RESULTADO
        
        elseif dueloEstado == DuelState.MOSTRANDO_RESULTADO then

            if agora - tempoResultado >= DURACAO_RESULTADO then

                removerCarta(cartasJogador, cartaSelecionada)
                removerCarta(cartasNPC, cartaNPCSelecionada)

                cartaSelecionada = nil
                cartaNPCSelecionada = nil

                if #cartasJogador > 0 and #cartasNPC > 0 then
                    dueloEstado = DuelState.SELECIONANDO
                else
                    dueloEstado = DuelState.FINALIZADO
                end
            end
   
        elseif dueloEstado == DuelState.FINALIZADO then
            print("Fim do duelo!")

            if jogadorScore > npcScore then
                print("Jogador venceu +100")
            elseif jogadorScore == npcScore then
                print("Empate +50")
            else
                print("NPC venceu")
            end

            running = false
        end

        pico.output.draw.image("../../../assets/imgs/dojo/background.png",background)
        desenha_mao(cartasNPC, false)
        desenha_mao(cartasJogador, true)
        if dueloEstado == DuelState.MOSTRANDO_RESULTADO and cartaSelecionada  and cartaNPCSelecionada then
            pico.output.draw.image(cartaSelecionada.path, rect_carta_jogador)
            pico.output.draw.image(cartaNPCSelecionada.path ,rect_carta_NPC)
            
            if resultado == ResultadoState.GANHOU then 
                pico.output.draw.image("../../../assets/imgs/dojo/ok.png", rect_carta_jogador)
                pico.output.draw.image("../../../assets/imgs/dojo/x.png", rect_carta_NPC)
            elseif resultado == ResultadoState.PERDEU then
                pico.output.draw.image("../../../assets/imgs/dojo/x.png", rect_carta_jogador)
                pico.output.draw.image("../../../assets/imgs/dojo/ok.png", rect_carta_NPC)
            else 
                pico.output.draw.image("../../../assets/imgs/dojo/ok.png", rect_carta_jogador)
                pico.output.draw.image("../../../assets/imgs/dojo/ok.png", rect_carta_NPC)
            end

        end
        pico.output.present()
    end
end

return Dojo