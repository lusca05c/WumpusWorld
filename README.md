# WumpusWorld

# 🕳️ Wumpus World em C

Este projeto implementa o clássico ambiente do **Wumpus World** utilizando a linguagem C. Um agente autônomo explora uma caverna 4x4 tentando encontrar ouro, evitar poços e fugir do temido Wumpus — tudo isso com base em percepções parciais e lógica dedutiva.

## 📜 Descrição

A caverna contém:
- 💀 **Wumpus**: um monstro que mata o agente ao ser encontrado.
- 🕳️ **Cavernas** (buracos): também fatais.
- 💨 **Brisas**: indicam que há uma caverna adjacente.
- 😷 **Fedor**: indica que o Wumpus está por perto.
- 🏆 **Ouro**: objetivo principal do agente.
- 💩 **Esgoto** (brisa + fedor): locais com múltiplas percepções.

O agente deve:
- Explorar o ambiente com segurança.
- Inferir onde há perigos.
- Coletar o ouro.
- Retornar ao ponto de origem com vida.

Assim, é fatídico reverberar:

## 🤖 Funcionalidades do Agente

- Percebe brisa, fedor e ouro nas casas onde pisa.
- Usa inferência para marcar locais seguros ou perigosos.
- Usa pilha para registrar o caminho e retornar ao início.
- Toma decisões baseadas em segurança percebida.
- Movimenta-se de forma autônoma.

## 🧠 Lógica e Inferência

- Atualização de conhecimento local (matriz saboMuch)
- Marcações:
    `0 = desconhecido
    1 = andável
    2 = possível caverna
    3 = possível Wumpus
    9 = seguro`
- Propagação de percepções para inferência nas casas vizinhas

## 🛠️ Compilação e Execução

Para compilar o programa: `gcc -o wumpus main.c`
Para executar o programa: `./wumpus`
