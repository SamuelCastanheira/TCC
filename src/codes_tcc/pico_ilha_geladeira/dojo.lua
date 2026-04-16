local Dojo = {}

local NUM_CARTAS_PLAYER = 3

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

local function novaCarta(elemento, pontuacao, path)
    return {
        elemento = elemento,
        pontuacao = pontuacao,
        path = path,
        rect = {}
    }
end

local function sortearCartas(pool)
    local destino = {}
    local usados = {}

    while #destino < NUM_CARTAS_PLAYER do
        local idx = math.random(1, #pool)

        if not usados[idx] then
            usados[idx] = true
            local base = pool[idx]
            table.insert(destino, novaCarta(base.elemento, base.pontuacao, base.path))
        end
    end

    return destino
end

local function compararCartas(a, b)
    if a.elemento == b.elemento then
        if a.pontuacao > b.pontuacao then return ResultadoState.GANHOU end
        if a.pontuacao < b.pontuacao then return ResultadoState.PERDEU end
        return ResultadoState.EMPATE
    end

    if (a.elemento == Elemento.GELO and b.elemento == Elemento.AGUA) or
       (a.elemento == Elemento.AGUA and b.elemento == Elemento.FOGO) or
       (a.elemento == Elemento.FOGO and b.elemento == Elemento.GELO) then
        return ResultadoState.GANHOU
    end

    return ResultadoState.PERDEU
end

local function removerCarta(lista, alvo)
    for i = #lista, 1, -1 do
        if lista[i] == alvo then
            table.remove(lista, i)
            return
        end
    end
end

local dueloEstado = DuelState.SELECIONANDO
local cartasJogador = {}
local cartasNPC = {}

local jogadorScore = 0
local npcScore = 0
local resultado = -1

local tempoResultado = 0
local DURACAO_RESULTADO = 1.5

local function ordena_mao(mao, jogador)
    if jogador then
        local x = 0.9
        for i, carta in ipairs(mao) do
            carta.rect = {
                '%',
                x = x - (i - 1) * 0.12,
                y = 0.88,
                w = 0.2,
                h = 0.3
            }
        end

    else
        local x = 0.1
        for i, carta in ipairs(mao) do
            carta.rect = {
                '%',
                x = x + (i - 1) * 0.12,
                y = 0.88,
                w = 0.2,
                h = 0.3
            }
        end
    end
end

local function desenha_mao(mao, jogador)
    local verso = "../../../assets/imgs/dojo/carta_azul.png"

    for _, carta in ipairs(mao) do
        -- 🔥 NUNCA desenha carta selecionada na mão
        if carta ~= cartaSelecionada then
            if jogador then
                pico.layer.image(carta.path)
                pico.set.layer(carta.path)
                pico.set.view{source={'%', x=0.5, y=0.5, w=0.6, h=0.8}}
                pico.set.layer()
                pico.output.draw.layer(carta.path, carta.rect)
            else
                pico.layer.image(verso)
                pico.set.layer(verso)
                pico.set.view{source={'%', x=0.5, y=0.5, w=0.6, h=0.8}}
                pico.set.layer()
                pico.output.draw.layer(verso, carta.rect)
            end
        end
    end
end

function Dojo.renderizar()
    pico.set.window{title="Dojo"}
    math.randomseed(os.time())

    local background = {'%', x=0.5, y=0.5, w=1, h=1}

    local pool = {
        novaCarta(Elemento.AGUA, 3, "../../../assets/imgs/dojo/agua_3.png"),
        novaCarta(Elemento.FOGO, 4, "../../../assets/imgs/dojo/fogo_4.png"),
        novaCarta(Elemento.FOGO, 7, "../../../assets/imgs/dojo/fogo_7.png"),
        novaCarta(Elemento.GELO, 5, "../../../assets/imgs/dojo/gelo_5.png"),
        novaCarta(Elemento.GELO, 6, "../../../assets/imgs/dojo/gelo_6.png")
    }

    cartasJogador = sortearCartas(pool)
    cartasNPC = sortearCartas(pool)

    ordena_mao(cartasJogador, true)
    ordena_mao(cartasNPC, false)

    local rect_jogador = {'%', x=0.8, y=0.3, w=0.5, h=0}
    local rect_npc = {'%', x=0.2, y=0.3, w=0.5, h=0}

    while true do
        local e = pico.input.event()
        local mouse = pico.get.mouse('%')
        local agora = os.time()

        if e and e.tag == 'quit' then break end

        if e and e.tag == 'mouse.button.dn' and dueloEstado == DuelState.SELECIONANDO then
            for _, carta in ipairs(cartasJogador) do
                if pico.vs.pos_rect(mouse, carta.rect) then
                    cartaSelecionada = carta
                    break
                end
            end
        end

        -- STATE MACHINE
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

                ordena_mao(cartasJogador, true)
                ordena_mao(cartasNPC, false)

                if #cartasJogador == 0 or #cartasNPC == 0 then
                    dueloEstado = DuelState.FINALIZADO
                else
                    dueloEstado = DuelState.SELECIONANDO
                end
            end

        elseif dueloEstado == DuelState.FINALIZADO then
            print("Fim do duelo!")
            break
        end

        -- RENDER LIMPO
        pico.output.draw.image("../../../assets/imgs/dojo/background.png", background)

        desenha_mao(cartasNPC, false)
        desenha_mao(cartasJogador, true)

        if dueloEstado == DuelState.MOSTRANDO_RESULTADO and cartaSelecionada and cartaNPCSelecionada then

            pico.output.draw.image(cartaSelecionada.path, rect_jogador)
            pico.output.draw.image(cartaNPCSelecionada.path, rect_npc)

            -- resultado visual (centralizado no conceito, não na carta)
            if resultado == ResultadoState.GANHOU then
                pico.output.draw.image("../../../assets/imgs/dojo/ok.png", rect_jogador)
                pico.output.draw.image("../../../assets/imgs/dojo/x.png", rect_npc)

            elseif resultado == ResultadoState.PERDEU then
                pico.output.draw.image("../../../assets/imgs/dojo/x.png", rect_jogador)
                pico.output.draw.image("../../../assets/imgs/dojo/ok.png", rect_npc)

            else
                pico.output.draw.image("../../../assets/imgs/dojo/ok.png", rect_jogador)
                pico.output.draw.image("../../../assets/imgs/dojo/ok.png", rect_npc)
            end
        end
        pico.output.present()
    end
end

return Dojo