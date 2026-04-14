local Dojo = {}

-- Constantes
local NUM_CARTAS_PLAYER = 3
local NUM_CARTAS_POOL = 5

local cartaSelecionada = -1
local cartaNPCSelecionada = -1

local Elemento = {
    GELO = 1,
    AGUA = 2,
    FOGO = 3
}

local DuelState = {
    SELECIONANDO = 1,
    COMPARANDO = 2,
    FINALIZADO = 3
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

-- Sorteio
local function sortearCartas(pool)
    local destino = {}
    local usados = {}

    while #destino < NUM_CARTAS_PLAYER do
        local idx = math.random(1, NUM_CARTAS_POOL)
        if not usados[idx] then
            usados[idx] = true
            table.insert(destino, pool[idx])
        end
    end

    return destino
end

-- Comparação
local function compararCartas(jogador, npc)
    if jogador.elemento == npc.elemento then
        if jogador.pontuacao > npc.pontuacao then return 1
        elseif jogador.pontuacao < npc.pontuacao then return -1
        else return 0 end
    end

    if (jogador.elemento == Elemento.GELO and npc.elemento == Elemento.AGUA) or
       (jogador.elemento == Elemento.AGUA and npc.elemento == Elemento.FOGO) or
       (jogador.elemento == Elemento.FOGO and npc.elemento == Elemento.GELO) then
        return 1
    else
        return -1
    end
end

local function removerCarta(array, index)
    table.remove(array, index)
end

-- Estado
local dueloEstado = DuelState.SELECIONANDO
local cartasJogador = {}
local cartasNPC = {}

local jogadorScore = 0
local npcScore = 0

local running = true

-- UPDATE (lógica)
function update()
    if dueloEstado == DuelState.SELECIONANDO then
        if cartaSelecionada ~= -1 and cartaNPCSelecionada ~= -1 then
            dueloEstado = DuelState.COMPARANDO
        end

    elseif dueloEstado == DuelState.COMPARANDO then
        local cJog = cartasJogador[cartaSelecionada]
        local cNPC = cartasNPC[cartaNPCSelecionada]

        local resultado = compararCartas(cJog, cNPC)

        if resultado == 1 then jogadorScore = jogadorScore + 3
        elseif resultado == -1 then npcScore = npcScore + 3
        else
            jogadorScore = jogadorScore + 1
            npcScore = npcScore + 1
        end

        pico.input.delay(3000)

        removerCarta(cartasJogador, cartaSelecionada)
        removerCarta(cartasNPC, cartaNPCSelecionada)

        cartaSelecionada = -1
        cartaNPCSelecionada = -1

        if #cartasJogador > 0 and #cartasNPC > 0 then
            dueloEstado = DuelState.SELECIONANDO
        else
            dueloEstado = DuelState.FINALIZADO
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
end

function ordena_mao(mao, jogador)
    local x_inicial = 0.2
    if (jogador) then
        x_inicial = 0.6
    end
    print(#mao)
    for i, carta in ipairs(mao) do
        carta.rect = {
            '%', 
            x = x_inicial + (i - 1) * 0.1, 
            y=0.8, 
            w=0.3, 
            h=0.3}  
    end
end

function desenha_mao(mao)
    for _, carta in ipairs(mao) do
        pico.output.draw.image(carta.path, carta.rect)
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

    ordena_mao(cartasJogador, true)
    ordena_mao(cartasNPC,false)
    local carta_turno = {'%', x=0.2 , y=0.3 , w=0.7, h= 0.7}
    local carta_turno_NPC = {'%', x=0.8 , y=0.3 , w=0.7 , h= 0.7}

    while true do
        local e = pico.input.event()
        local mouse = pico.get.mouse('%')
        if e ~= nil then
            if e.tag=='quit' then
                break
            end
            if e.tag =='mouse.button.dn' then
                local mouse = pico.get.mouse('%')
                for _, carta in ipairs(cartasJogador) do
                    if pico.vs.pos_rect(mouse, carta.rect) then

                    end 
                end

            end 
        end

         pico.output.draw.image("../../../assets/imgs/dojo/background.png",background)
        desenha_mao(cartasNPC)
        desenha_mao(cartasJogador)
        pico.output.present()
    end
end

return Dojo