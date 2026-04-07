# 🎮 Nerdman

Jogo desenvolvido em **C++** utilizando a biblioteca **SFML**, inspirado no clássico *Pac-Man*.  
A proposta traz uma abordagem divertida: um estudante nerd deve coletar itens pelo mapa enquanto foge de inimigas que se tornam cada vez mais desafiadoras.

---

## 📌 Funcionalidades

### 🖥️ Telas do jogo
- Menu inicial com opções de **Jogar, Créditos e Sair**
- Tela de **Game Over** com opção de reiniciar ou voltar ao menu
- Tela de **Vitória** exibida ao coletar todos os itens

---

### 🎨 Elementos visuais
- Personagem principal: estudante nerd
- Inimigos: quatro personagens com diferenciação visual
- Ícone personalizado exibido na janela do jogo

---

### 🌀 Sistema de portais
- Portais que transportam o jogador entre regiões do mapa
- Implementação com **controle de cooldown** para evitar teletransporte contínuo

---

### 🎯 Sistema de coletáveis
- Itens: lápis, borracha, livro, régua e óculos
- Ordem de coleta **aleatória a cada partida**
- Sistema de pontuação baseado nos itens coletados
- **Escalonamento de dificuldade**:
  - Inimigos passam a perseguir o jogador
  - Aumento progressivo de velocidade

---

### 🔊 Áudio
- Música de fundo durante a gameplay
- Efeitos sonoros para:
  - Coleta de itens
  - Vitória
  - Captura pelo inimigo

---

### 🤖 Inteligência dos inimigos
- Movimentação aleatória inicial
- Sistema de perseguição utilizando **Busca em Largura (BFS)**
- Controle de colisão para evitar sobreposição entre inimigos

---

## 🚀 Tecnologias Utilizadas

- **C++**
- **SFML (Simple and Fast Multimedia Library)**

### 🧠 Conceitos aplicados:
- Algoritmos de busca (**BFS**)
- Estruturas de dados
- Controle de estado do jogo
- Randomização
- Lógica de colisão

---

## 🎮 Como Jogar

1. Inicie o jogo pelo menu principal  
2. Explore o mapa coletando os itens  
3. Utilize os portais estrategicamente  
4. Evite os inimigos  

- ✔ Coletou todos os itens → **Vitória**
- ❌ Foi capturado → **Game Over**

---

## ⚙️ Execução

```bash
git clone https://github.com/seu-usuario/nerdman
cd nerdman

👥 Créditos

Projeto desenvolvido para a disciplina INF 110, com colaboração de:

Thales Rodrigues Neves
Luiz Filipe Santos Oliveira
Paulo Henrique de Souza Castro
