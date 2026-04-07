# 🎮 NerdMan  

Um jogo desenvolvido em **C++** utilizando a biblioteca **SFML**, inspirado no clássico Pac-Man, mas com uma proposta divertida: um menino nerd precisa fugir de quatro meninas enquanto coleta itens espalhados pelo mapa.  

---

## 📌 Funcionalidades  

- **Telas de interação**
  - Menu inicial com opções de **Jogar**, **Créditos** e **Sair**  
  - Tela de **Game Over** com opção de jogar novamente ou voltar ao menu  
  - Tela de **Vitória** exibida após coletar todos os itens  

- **Sprites**
  - Personagem principal: um menino nerd de óculos  
  - Inimigos: quatro meninas com cores de cabelo distintas  
  - Ícone próprio do jogo exibido na janela e na barra de tarefas  

- **Portais**
  - Portais coloridos que transportam o jogador para diferentes partes do mapa  
  - Implementação com **cooldown de 0.4 segundos** para evitar loops de teletransporte  

- **Coletáveis**
  - Itens: lápis, borracha, livro, régua e óculos  
  - Ordem aleatória em cada partida  
  - Barra de score exibindo os itens coletados  
  - Quanto mais itens coletados, maior a dificuldade (inimigos passam a perseguir o jogador e ficam mais rápidos)  

- **Música e efeitos sonoros**
  - Música de fundo durante a partida  
  - Efeitos sonoros para coleta de itens, vitória e captura pelo inimigo  

- **Movimentação dos inimigos**
  - Movimentação **aleatória** com a função `mover_vilao_aleatorio()`  
  - Movimentação **inteligente** (perseguição) com algoritmo de **busca em largura (BFS)**  
  - Prevenção de sobreposição de inimigos com a função `pos_ocupada()`  

---

## 🚀 Tecnologias Utilizadas  

- **Linguagem:** C++  
- **Biblioteca gráfica:** [SFML](https://www.sfml-dev.org/)  
- **Algoritmos:**  
  - Busca em largura (BFS) para perseguição  
  - Randomização para posicionamento e ordem dos coletáveis  

---

## 🎯 Como Jogar  

1. Inicie o jogo pelo menu inicial.  
2. Mova o personagem para coletar os 5 itens espalhados pelo mapa.  
3. Use os **portais** estrategicamente para fugir dos inimigos.  
4. Se todos os itens forem coletados → 🎉 Você vence!  
5. Se for capturado → 💔 Tela de Game Over.  

---

## ⚙️ Compilação e Execução  

Clone o repositório:  
```bash
git clone https://github.com/seu-usuario/nerd-escape.git
cd nerd-escape

👥 Créditos

Projeto desenvolvido para a disciplina INF 110, com colaboração de:

Luiz Filipe Santos Oliveira
Paulo Henrique de Souza Castro
Thales Rodrigues Neves
