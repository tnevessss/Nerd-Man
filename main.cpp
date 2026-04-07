#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
using namespace sf;

// Mapa do jogo, onde '1' representa parede e '0' caminho livre
const char mapa[48][49] = {
    "111111111111111111111111111111111111111111111111",
    "111111111111111111111111111111111111111111111111",
    "111111111111110001111111111111100011111111111111",
    "111111111111110001111111111111100011111111111111",
    "111111111111110001111111111111100011111111111111",
    "111111111111110001111111111111100011111111111111",
    "111111111111110001111111111111100011111111111111",
    "111111111111110001111111111111100011111111111111",
    "111111111111110001111111111111100011111111111111",
    "111111111111110001111111111111100011111111111111",
    "110000000000000000000000000000000000000000000011",
    "110000000000000000000000000000000000000000000011",
    "111111111100011110000000000000011110001111111111",
    "111111111100011110000111111000011110001111111111",
    "111111111100011110000111111000011110001111111111",
    "111111111100011110000111111000011110001111111111",
    "111111111100011110000111111000011110001111111111",
    "111111111100011110000111111000011110001111111111",
    "110000000000000000000000000000000000000000000011",
    "110000000000000000000000000000000000000000000011",
    "110000000000000000000000000000000000000000000011",
    "111111111100011111111000000111111110001111111111",
    "111111111100011111111000000111111110001111111111",
    "111111111100011111111000000111111110001111111111",
    "111111111100011111111000000111111110001111111111",
    "111111111100011111111000000111111110001111111111",
    "111111111100011111111000000111111110001111111111",
    "110000000000000000000000000000000000000000000011",
    "110000000000000000000000000000000000000000000011",
    "110000000000000000000000000000000000000000000011",
    "111111111000000000000000000000000000000111111111",
    "111111111000000000011111111100000000000111111111",
    "111111111000000000011111111100000000000111111111",
    "111111111000000000011111111100000000000111111111",
    "111110000000000000011111111100000000000000011111",
    "111110000000000000011111111100000000000000011111",
    "111110000000000000011111111100000000000000011111",
    "111110000000000000011111111100000000000000011111",
    "111110000011111100011111111100000011111000011111",
    "111110000011111100011111111100000011111000011111",                             
    "111110000011111100011111111100000011111000011111",
    "111110000011111100011111111100000011111000011111",
    "111110000000000000011111111100000011111000011111",
    "111110000000000000011111111100000000000000011111",
    "111110000000000000011111111100000000000000011111",
    "111110000000000000011111111100000000000000011111",
    "111111111111111111111111111111111111111111111111",
    "111111111111111111111111111111111111111111111111"
};

// Enumeração para as direções possíveis
enum Direcao { CIMA, DIREITA, BAIXO, ESQUERDA };

// Estrutura para coordenadas e caminho do BFS
struct Coordenadas {
    int x, y;
    vector<vector<int>> caminho;
};

// Estrutura para itens coletáveis
struct Coletavel {
    Sprite *sprite;
    FloatRect area;
    bool coletado;
};

// Posição inicial do personagem
int POSX_PERSONAGEM_INICAL = 24;
int POSY_PERSONAGEM_INICAL = 24;

// Declaração da função principal do jogo
void PlayPacMan(RenderWindow &window);

// Função para mostrar imagem final (tela de vitória ou derrota)
void mostrarImagemFinal(RenderWindow &window, const string &imagem) {
    Texture finalTexture;
    if (!finalTexture.loadFromFile(imagem)) {
        return;
    }
    Sprite finalSprite(finalTexture);
    Vector2u size = finalTexture.getSize();
    finalSprite.setScale(960.f / size.x, 960.f / size.y);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed ||
                (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left))
                return;
        }
        window.clear();
        window.draw(finalSprite);
        window.display();
    }
}

// Função para mostrar tela de vitória
void mostrarTelaWin(RenderWindow &window) {
    Texture winTexture;
    if (!winTexture.loadFromFile("images/WIN.png")) {
        cerr << "Erro ao carregar WIN.png\n";
        return;
    }

    Sprite winSprite(winTexture);
    Vector2u imgSize = winTexture.getSize();
    winSprite.setScale(960.f / imgSize.x, 960.f / imgSize.y);

    IntRect retornar_ao_menu_win(202, 790, 555, 151);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }
            // Retorna ao menu ao clicar no botão
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (retornar_ao_menu_win.contains(mousePos)) {
                    return;
                }
            }
        }

        window.clear();
        window.draw(winSprite);
        window.display();
    }
}

// Função para mostrar tela de game over
void mostrarGameOver(RenderWindow &window) {
    Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("images/game_over.png")) {
        return;
    }
    Sprite gameOverSprite(gameOverTexture);
    Vector2u imgSize = gameOverTexture.getSize();
    gameOverSprite.setScale(960.f / imgSize.x, 960.f / imgSize.y);

    IntRect retornar_ao_menu(30, 811, 327, 118);
    IntRect jogar_novamente(607, 812, 316, 123);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }
            // Retorna ao menu ou reinicia o jogo
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (retornar_ao_menu.contains(mousePos)) {
                    return;
                }
                if (jogar_novamente.contains(mousePos)) {
                    PlayPacMan(window);
                    return;
                }
            }
        }
        window.clear();
        window.draw(gameOverSprite);
        window.display();
    }
}

// Função principal do jogo
void PlayPacMan(RenderWindow &window){
    // Posições iniciais dos vilões
    int VILOES_POSICAO[4][2] = {{8,10},{28,10},{8,28},{28,28}};
    int posx_personagem = POSX_PERSONAGEM_INICAL;
    int posy_personagem = POSY_PERSONAGEM_INICAL;
    int indexAtual = 0;

    // Direções do personagem e dos vilões
    bool direcoes_personagem[4] = {false};
    bool direcoes_viloes[4][4];
    bool viloes_perseguindo[4] = {false};

    // Inicializa direções dos vilões
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            direcoes_viloes[i][j] = false;
        }
    }

    bool olha_direita = true;
    bool jogar = false;
    bool perdeu = false;

    int ultimo_aleatorios_viloes[4] = {-1,-1,-1,-1};

    // Lambda para verificar se uma posição está ocupada por outro vilão
    auto pos_ocupada = [&](int x, int y, int vilao_atual) {
        for (int i = 0; i < 4; i++) {
            if (i != vilao_atual && VILOES_POSICAO[i][0] == x && VILOES_POSICAO[i][1] == y)
                return true;
        }
        return false;
    };

    // Lambda para mover vilão usando BFS (perseguição)
    auto mover_vilao_bfs = [&](int &vilao_x, int &vilao_y, int posx_personagem, int posy_personagem) {
        int dx[] = {0, 1, 0, -1};
        int dy[] = {-1, 0, 1, 0};

        bool visitado[48][49] = {false};
        queue<Coordenadas> fila;

        fila.push({vilao_x, vilao_y, {}});
        visitado[vilao_y][vilao_x] = true;

        while (!fila.empty()) {
            Coordenadas atual = fila.front();
            fila.pop();

            // Se encontrou o personagem, move o vilão na direção correta
            if (atual.x == posx_personagem && atual.y == posy_personagem) {
                if (!atual.caminho.empty()) {
                    int nx = atual.caminho[0][0];
                    int ny = atual.caminho[0][1];

                    int direcao = -1;
                    if (nx == vilao_x && ny == vilao_y - 1) direcao = CIMA;
                    else if (nx == vilao_x + 1 && ny == vilao_y) direcao = DIREITA;
                    else if (nx == vilao_x && ny == vilao_y + 1) direcao = BAIXO;
                    else if (nx == vilao_x - 1 && ny == vilao_y) direcao = ESQUERDA;

                    int idx = -1;
                    for (int i = 0; i < 4; ++i) {
                        if (vilao_x == VILOES_POSICAO[i][0] && vilao_y == VILOES_POSICAO[i][1]) {
                            idx = i;
                            break;
                        }
                    }
                    if (idx != -1 && direcao != -1) {
                        for (int d = 0; d < 4; ++d) direcoes_viloes[idx][d] = false;
                        direcoes_viloes[idx][direcao] = true;
                    }

                    if (!pos_ocupada(nx, ny, idx)) {
                        vilao_x = nx;
                        vilao_y = ny;
                    }
                }
                return;
            }

            // Explora vizinhos
            for (int i = 0; i < 4; i++) {
                int nx = atual.x + dx[i];
                int ny = atual.y + dy[i];

                if (nx >= 0 && nx < 48 && ny >= 0 && ny < 48 &&
                    !visitado[ny][nx] && mapa[ny][nx] != '1') {
                    visitado[ny][nx] = true;
                    vector<vector<int>> novo_caminho = atual.caminho;
                    novo_caminho.push_back({nx, ny});
                    fila.push({nx, ny, novo_caminho});
                }
            }
        }
    };

    // Lambda para mover vilão aleatoriamente
    auto mover_vilao_aleatorio = [&](int &posx_vilao, int &posy_vilao, int &ultimo_vilao, int vilao){
        bool direcoes_vilao[4];
        direcoes_vilao[CIMA] = mapa[posy_vilao-1][posx_vilao] != '1';
        direcoes_vilao[DIREITA] = mapa[posy_vilao][posx_vilao+1] != '1';
        direcoes_vilao[BAIXO] = mapa[posy_vilao+1][posx_vilao] != '1';
        direcoes_vilao[ESQUERDA] = mapa[posy_vilao][posx_vilao-1] != '1';

        int temp, seguranca = 0;
        do{
            temp = rand() % 4;
            seguranca++;
            if (seguranca >= 50){
                switch (ultimo_aleatorios_viloes[vilao]) {
                    case CIMA:
                    case DIREITA:
                        temp = ultimo_aleatorios_viloes[vilao]+2;
                        break;
                    case BAIXO:
                    case ESQUERDA: 
                        temp = ultimo_aleatorios_viloes[vilao]-2;
                        break;
                }
                break;
            }
        } while (abs(temp-ultimo_aleatorios_viloes[vilao]) == 2 || !(direcoes_vilao[temp]));

        ultimo_aleatorios_viloes[vilao] = temp;

        // Move o vilão na direção sorteada, se possível
        if (temp == CIMA && !pos_ocupada(posx_vilao,posy_vilao-1, vilao)) {
                posy_vilao--;
                direcoes_viloes[vilao][CIMA] = true;
                direcoes_viloes[vilao][DIREITA] = direcoes_viloes[vilao][BAIXO] = direcoes_viloes[vilao][ESQUERDA] = false;
        } else if (temp == DIREITA && !pos_ocupada(posx_vilao+1,posy_vilao, vilao)){
                posx_vilao++;
                direcoes_viloes[vilao][DIREITA] = true;
                direcoes_viloes[vilao][CIMA] = direcoes_viloes[vilao][BAIXO] = direcoes_viloes[vilao][ESQUERDA] = false;
        } else if (temp == BAIXO && !pos_ocupada(posx_vilao,posy_vilao+1, vilao)){
                posy_vilao++;
                direcoes_viloes[vilao][BAIXO] = true;
                direcoes_viloes[vilao][DIREITA] = direcoes_viloes[vilao][CIMA] = direcoes_viloes[vilao][ESQUERDA] = false;
        } else if (temp == ESQUERDA && !pos_ocupada(posx_vilao-1,posy_vilao, vilao)){
                posx_vilao--;
                direcoes_viloes[vilao][ESQUERDA] = true;
                direcoes_viloes[vilao][DIREITA] = direcoes_viloes[vilao][BAIXO] = direcoes_viloes[vilao][CIMA] = false;
        }
    };

    srand(time(0));

    // Áreas dos portais e coletáveis
    FloatRect Portal_roxo_1(110.f, 692.f,18.f, 13.f);
    FloatRect Portal_roxo_2(890.f, 211.f, 18.f, 13.f);
    FloatRect Portal_vermelho_1(51, 209, 18, 13);
    FloatRect Portal_vermelho_2(825, 895, 18, 13);
    FloatRect Portal_branco_1(300, 53, 18, 13);
    FloatRect Portal_branco_2(880, 556, 18, 13);

    FloatRect areaLivro(878.f, 373.f, 23.f, 18.f);
    FloatRect areaLapis(585.f, 874.f, 23.f, 18.f);
    FloatRect areaBorracha(121.f, 882.f, 23.f, 18.f);
    FloatRect areaRegua(50.f, 376.f, 23.f, 18.f);
    FloatRect areaOculos(629.f, 52.f, 23.f, 18.f);

    // Carrega texturas dos coletáveis
    Texture texLivro, texLapis, texBorracha, texRegua, texOculos;
    texLivro.loadFromFile("images/livro.png");
    texLapis.loadFromFile("images/lapis.png");
    texBorracha.loadFromFile("images/borracha.png");
    texRegua.loadFromFile("images/regua.png");
    texOculos.loadFromFile("images/oculos.png");

    // Carrega músicas e efeitos sonoros
    Music musicaFundo;
    if (!musicaFundo.openFromFile("sounds/musica_de_fundo.ogg")) {
        cerr << "Erro ao carregar musica_de_fundo.ogg" << endl;
    }
    musicaFundo.setLoop(true);
    musicaFundo.play();

    SoundBuffer bufferBeijo;
    if (!bufferBeijo.loadFromFile("sounds/beijo.ogg")) {
        cerr << "Erro ao carregar beijo.ogg" << endl;
    }
    Sound somBeijo;
    somBeijo.setBuffer(bufferBeijo);

    SoundBuffer bufferColetavel;
    if (!bufferColetavel.loadFromFile("sounds/coletavel.ogg")) {
        cerr << "Erro ao carregar coletavel.ogg" << endl;
    }
    Sound somColetavel;
    somColetavel.setBuffer(bufferColetavel);

    SoundBuffer bufferVitoria;
    if (!bufferVitoria.loadFromFile("sounds/vitoria.ogg")) {
        cerr << "Erro ao carregar vitoria.ogg" << endl;
    }
    Sound somVitoria;
    somVitoria.setBuffer(bufferVitoria);

    // Cria sprites dos coletáveis
    Sprite sprLivro(texLivro);
    Sprite sprLapis(texLapis);
    Sprite sprBorracha(texBorracha);
    Sprite sprRegua(texRegua);
    Sprite sprOculos(texOculos);

    sprLivro.setScale(0.08f, 0.08f);
    sprLapis.setScale(0.08f, 0.08f);
    sprBorracha.setScale(0.08f, 0.08f);
    sprRegua.setScale(0.08f, 0.08f);
    sprOculos.setScale(0.08f, 0.08f);

    sprLivro.setPosition(878.f, 373.f);
    sprLapis.setPosition(575.f, 874.f);
    sprBorracha.setPosition(110.f, 882.f);
    sprRegua.setPosition(50.f, 376.f);
    sprOculos.setPosition(629.f, 52.f);

    // Sprites miniatura dos coletáveis (HUD)
    Sprite sprLivroMini(sprLivro);
    Sprite sprLapisMini(sprLapis);
    Sprite sprBorrachaMini(sprBorracha);
    Sprite sprReguaMini(sprRegua); 
    Sprite sprOculosMini(sprOculos); 

    sprLivroMini.setScale(0.05f, 0.05f);   
    sprLapisMini.setScale(0.04f, 0.04f);   
    sprBorrachaMini.setScale(0.04f, 0.04f); 
    sprReguaMini.setScale(0.045f, 0.045f);   
    sprOculosMini.setScale(0.08f, 0.08f); 

    sprLivroMini.setPosition(30, 0);
    sprLapisMini.setPosition(60, 0);
    sprBorrachaMini.setPosition(80, 2);
    sprReguaMini.setPosition(100, 0);
    sprOculosMini.setPosition(120, -5);

    // Vetor de coletáveis
    vector<Coletavel> coletaveis = {
    { &sprLivro,    areaLivro,    false },
    { &sprLapis,    areaLapis,    false },
    { &sprBorracha, areaBorracha, false },
    { &sprRegua,    areaRegua,    false },
    { &sprOculos,   areaOculos,   false }
    };

    // Ordem aleatória dos coletáveis
    vector<int> ordemColetaveis = {0, 1, 2, 3, 4};
    random_shuffle(ordemColetaveis.begin(), ordemColetaveis.end());

    int coletavelAtual = ordemColetaveis[indexAtual];

    // Carrega texturas do personagem principal
    Texture pacman_costa, pacman_direita, pacman_frente, pacman_esquerda;
    if (!pacman_frente.loadFromFile("images/pacman_frente.png") || 
        !pacman_esquerda.loadFromFile("images/pacman_esquerda.png") || 
        !pacman_direita.loadFromFile("images/pacman_direita.png") ||
        !pacman_costa.loadFromFile("images/pacman_costa.png")) {
        cerr << "Erro ao carregar texturas do Pac-Man" << endl;
        return;
    }

    // Carrega texturas dos vilões
    Texture ruiva_frente, ruiva_costa, ruiva_direita, ruiva_esquerda;
    if (!ruiva_frente.loadFromFile("images/ruivo/frente.png") || 
    !ruiva_costa.loadFromFile("images/ruivo/costa.png") || 
    !ruiva_esquerda.loadFromFile("images/ruivo/esquerda.png") || 
    !ruiva_direita.loadFromFile("images/ruivo/direita.png")) {
        cerr << "Erro ao carregar texturas da Ruiva" << endl;
        return;
    }

    Texture preto_frente, preto_costa, preto_direita, preto_esquerda;
    if (!preto_frente.loadFromFile("images/preto/frente.png") || 
    !preto_costa.loadFromFile("images/preto/costa.png") || 
    !preto_esquerda.loadFromFile("images/preto/esquerda.png") || 
    !preto_direita.loadFromFile("images/preto/direita.png")) {
        cerr << "Erro ao carregar texturas do Preto" << endl;
        return;
    }

    Texture loiro_frente, loiro_costa, loiro_direita, loiro_esquerda;
    if (!loiro_frente.loadFromFile("images/loiro/frente.png") || 
    !loiro_costa.loadFromFile("images/loiro/costa.png") || 
    !loiro_esquerda.loadFromFile("images/loiro/esquerda.png") || 
    !loiro_direita.loadFromFile("images/loiro/direita.png")) {
        cerr << "Erro ao carregar texturas da Loira" << endl;
        return;
    }

    Texture castanho_frente, castanho_costa, castanho_direita, castanho_esquerda;
    if (!castanho_frente.loadFromFile("images/castanho/frente.png") || 
    !castanho_costa.loadFromFile("images/castanho/costa.png") || 
    !castanho_esquerda.loadFromFile("images/castanho/esquerda.png") || 
    !castanho_direita.loadFromFile("images/castanho/direita.png")) {
        cerr << "Erro ao carregar texturas do Castanho" << endl;
        return;
    }

    // Cria sprites do personagem e vilões
    Sprite pacman;
    pacman.setTexture(pacman_frente);
    pacman.setScale(30.0/pacman_frente.getSize().x,30.0/pacman_frente.getSize().y);

    Sprite vilao1;
    vilao1.setTexture(ruiva_frente);
    vilao1.scale(0.025,0.025);

    Sprite vilao2;
    vilao2.setTexture(preto_frente);
    vilao2.scale(0.06,0.06);

    Sprite vilao3;
    vilao3.setTexture(loiro_frente);
    vilao3.scale(0.025,0.025);

    Sprite vilao4;
    vilao4.setTexture(castanho_frente);
    vilao4.scale(0.06,0.06);

    Sprite viloes[4] = {vilao1, vilao2, vilao3, vilao4};
    Texture texture_viloes[4][4] = {{ruiva_costa, ruiva_direita, ruiva_frente, ruiva_esquerda}, {preto_costa, preto_direita, preto_frente, preto_esquerda}, {loiro_costa, loiro_direita, loiro_frente, loiro_esquerda}, {castanho_costa, castanho_direita, castanho_frente, castanho_esquerda}};

    // Carrega o mapa do jogo
    Sprite spritemap;
    Texture texturemap;
    if (!texturemap.loadFromFile("images/image.png")) {
        cout << "Erro lendo imagem image.png\n";
        return;
    }

    // Carrega imagem de game over
    Sprite spritegameover;
    Texture texturegameover;
    if (!texturegameover.loadFromFile("images/game_over.png")) {
        cout << "Erro lendo imagem image.png\n";
        return;
    }
    spritegameover.setTexture(texturegameover);
    spritegameover.setPosition(0, 0);

    spritemap.setTexture(texturemap);
    spritemap.setPosition(0, 0);

    // Clocks para controlar tempo de movimento e teleporte
    Clock clock;
    Clock clock2;
    Clock clock3;
    Clock clock4;
    Clock teleportClock;
    float teleportCooldown = 0.4;
    float velocidade = 0.1;

    // Loop principal do jogo
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            // Controle de movimento do personagem
            if (event.type == Event::KeyPressed) {
                jogar = true;
                if (event.key.code == Keyboard::Left) {
                    direcoes_personagem[ESQUERDA] = true;
                    direcoes_personagem[CIMA] = direcoes_personagem[DIREITA] = direcoes_personagem[BAIXO] = false;
                    olha_direita = false;
                    pacman.setTexture(pacman_esquerda);
                } else if (event.key.code == Keyboard::Right) {
                    direcoes_personagem[DIREITA] = true;
                    direcoes_personagem[CIMA] = direcoes_personagem[ESQUERDA] = direcoes_personagem[BAIXO] = false;
                    olha_direita = true;
                    pacman.setTexture(pacman_direita);
                } else if (event.key.code == Keyboard::Up) {
                    direcoes_personagem[CIMA] = true;
                    direcoes_personagem[DIREITA] = direcoes_personagem[ESQUERDA] = direcoes_personagem[BAIXO] = false;
                    pacman.setTexture(pacman_costa);
                } else if (event.key.code == Keyboard::Down) {
                    direcoes_personagem[BAIXO] = true;
                    direcoes_personagem[CIMA] = direcoes_personagem[ESQUERDA] = direcoes_personagem[DIREITA] = false;
                    pacman.setTexture(pacman_frente);
                }
            }
        }

        window.clear(Color::Black);

        // Verifica colisão com vilões (game over)
        for (int i = 0; i < 4; i++){
            if (VILOES_POSICAO[i][0] == posx_personagem && VILOES_POSICAO[i][1] == posy_personagem){
                perdeu = true;
                musicaFundo.stop();
                somBeijo.play();
                while (somBeijo.getStatus() == sf::Sound::Playing) {
                    
                }
                mostrarGameOver(window);
                return;
            }
        }

        if (!perdeu){
            window.draw(spritemap);

            // Lógica dos portais (teleporte)
            if (teleportClock.getElapsedTime().asSeconds() >= teleportCooldown && Portal_roxo_1.intersects(pacman.getGlobalBounds())) {
                pacman.setPosition(Portal_roxo_2.left, Portal_roxo_2.top);
                posx_personagem = (Portal_roxo_2.left + 2) / 20;
                posy_personagem = Portal_roxo_2.top / 20;
                teleportClock.restart();
                direcoes_personagem[ESQUERDA] = true;
                direcoes_personagem[DIREITA] = direcoes_personagem[CIMA] = direcoes_personagem[BAIXO] = false;
                pacman.setTexture(pacman_esquerda);
            } else if (teleportClock.getElapsedTime().asSeconds() >= teleportCooldown && Portal_roxo_2.intersects(pacman.getGlobalBounds())) {
                pacman.setPosition(Portal_roxo_1.left, Portal_roxo_1.top);
                posx_personagem = (Portal_roxo_1.left+2) / 20;
                posy_personagem = Portal_roxo_1.top / 20;
                teleportClock.restart();
            }

            if (teleportClock.getElapsedTime().asSeconds() >= teleportCooldown && Portal_vermelho_1.intersects(pacman.getGlobalBounds())) {
                pacman.setPosition(Portal_vermelho_2.left, Portal_vermelho_2.top);
                posx_personagem = (Portal_vermelho_2.left+2) / 20;
                posy_personagem = Portal_vermelho_2.top / 20;
                teleportClock.restart();

            } else if (teleportClock.getElapsedTime().asSeconds() >= teleportCooldown && Portal_vermelho_2.intersects(pacman.getGlobalBounds())) {
                pacman.setPosition(Portal_vermelho_1.left, Portal_vermelho_1.top);
                posx_personagem = (Portal_vermelho_1.left+2) / 20;
                posy_personagem = Portal_vermelho_1.top / 20;
                teleportClock.restart();
                direcoes_personagem[DIREITA] = true;
                direcoes_personagem[ESQUERDA] = direcoes_personagem[CIMA] = direcoes_personagem[BAIXO] = false;
                pacman.setTexture(pacman_direita);
            }

            if (teleportClock.getElapsedTime().asSeconds() >= teleportCooldown && Portal_branco_1.intersects(pacman.getGlobalBounds())) {
                pacman.setPosition(Portal_branco_2.left, Portal_branco_2.top);
                posx_personagem = (Portal_branco_2.left+2) / 20;
                posy_personagem = Portal_branco_2.top / 20;
                teleportClock.restart();
                direcoes_personagem[ESQUERDA] = true; 
                direcoes_personagem[DIREITA] = direcoes_personagem[CIMA] = direcoes_personagem[BAIXO] = false;
                pacman.setTexture(pacman_esquerda);
            } else if (teleportClock.getElapsedTime().asSeconds() >= teleportCooldown && Portal_branco_2.intersects(pacman.getGlobalBounds())) {
                pacman.setPosition(Portal_branco_1.left, Portal_branco_1.top);
                posx_personagem =( Portal_branco_1.left+2) / 20;
                posy_personagem = Portal_branco_1.top / 20;
                teleportClock.restart();
                direcoes_personagem[BAIXO] = true;
                direcoes_personagem[ESQUERDA] = direcoes_personagem[DIREITA] = direcoes_personagem[CIMA] = false;
                pacman.setTexture(pacman_frente);
            }

            // Coleta de itens
            if (!coletaveis[coletavelAtual].coletado &&
                pacman.getGlobalBounds().intersects(coletaveis[coletavelAtual].area)) {

                coletaveis[coletavelAtual].coletado = true;
                somColetavel.play();

                if (indexAtual < 4){
                    viloes_perseguindo[indexAtual] = true;
                }
                indexAtual++;
                velocidade = 0.1 - indexAtual/100.0;

                if (indexAtual < ordemColetaveis.size()) {
                    coletavelAtual = ordemColetaveis[indexAtual];
                } else {
                    musicaFundo.stop();
                    somVitoria.play();
                    mostrarTelaWin(window);
                    return;
                }
            }

            // Desenha o coletável atual
            if (!coletaveis[coletavelAtual].coletado) {
                window.draw(*coletaveis[coletavelAtual].sprite);
            }

            // Desenha miniaturas dos coletáveis já coletados
            vector<Sprite*> miniSprites = {
            &sprLivroMini, &sprLapisMini, &sprBorrachaMini, &sprReguaMini, &sprOculosMini
            };
        
            if (coletaveis[0].coletado) window.draw(sprLivroMini);
            if (coletaveis[1].coletado) window.draw(sprLapisMini);
            if (coletaveis[2].coletado) window.draw(sprBorrachaMini);
            if (coletaveis[3].coletado) window.draw(sprReguaMini);
            if (coletaveis[4].coletado) window.draw(sprOculosMini);

            // Atualiza posição do personagem e desenha
            pacman.setPosition(posx_personagem * 20 - 5, posy_personagem * 20 - 10);
            window.draw(pacman);

            // Atualiza e desenha vilões
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (direcoes_viloes[i][j]) {
                        if (i == 0 && j == 0){
                            viloes[i].setScale(0.06, 0.06);
                        } else if(i == 0 && j != 0){
                            viloes[i].setScale(0.025, 0.025);
                        }
                        viloes[i].setTexture(texture_viloes[i][j]);
                        break;
                    }
                }
                viloes[i].setPosition(VILOES_POSICAO[i][0] * 20 - 5, VILOES_POSICAO[i][1] * 20 - 10);
                window.draw(viloes[i]);
            }
            
            window.display();

            if (!jogar){
                continue;
            }

            // Movimento do personagem (tempo)
            if (clock.getElapsedTime() > seconds(0.04)) {
                clock.restart();
                if (direcoes_personagem[CIMA] && mapa[posy_personagem-1][posx_personagem] != '1') posy_personagem--;
                if (direcoes_personagem[BAIXO] && mapa[posy_personagem+1][posx_personagem] != '1') posy_personagem++;
                if (direcoes_personagem[ESQUERDA] && mapa[posy_personagem][posx_personagem-1] != '1') posx_personagem--;
                if (direcoes_personagem[DIREITA] && mapa[posy_personagem][posx_personagem+1] != '1') posx_personagem++;
            }

            // Movimento dos vilões (tempo)
            if (clock2.getElapsedTime() > seconds(velocidade)) {
                clock2.restart();
                for (int i = 0; i < 4; i++){
                    if (viloes_perseguindo[i]){
                        mover_vilao_bfs(VILOES_POSICAO[i][0], VILOES_POSICAO[i][1], posx_personagem, posy_personagem);
                    } else{
                        mover_vilao_aleatorio(VILOES_POSICAO[i][0], VILOES_POSICAO[i][1], ultimo_aleatorios_viloes[i], i);
                    }
                }
            }
        } else{
            mostrarGameOver(window);
            return;
        }
    }
}

// Função principal do programa (menu)
int main() {
    RenderWindow window(VideoMode(960, 960), "NERDMAN", Style::Titlebar | Style::Close);
    window.setFramerateLimit(180);

    // Carrega imagem de fundo do menu
    Texture bgTexture;
    if (!bgTexture.loadFromFile("images/MENU_FINAL.png")) {
        return -1;
    }
    Sprite bgSprite(bgTexture);
    Vector2u sizeBG = bgTexture.getSize();
    bgSprite.setScale(960.f / sizeBG.x, 960.f / sizeBG.y);

    // Carrega tela de créditos
    Texture creditosTexture;
    if (!creditosTexture.loadFromFile("images/CREDITOS.png")) {
        return -1;
    }
    Sprite creditosSprite(creditosTexture);
    Vector2u sizeCred = creditosTexture.getSize();
    creditosSprite.setScale(960.f / sizeCred.x, 960.f / sizeCred.y);

    // Define ícone da janela
    Image icon;
    if (icon.loadFromFile("images/icone_janela.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    // Áreas dos botões do menu
    IntRect botaoJogar(338, 310, 274, 82);
    IntRect botaoCreditos(267, 414, 407, 98);
    IntRect botaoSair(380, 553, 201, 79);

    int selecionado = 0;
    bool mostrandoCreditos = false;

    // Loop principal do menu
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (!mostrandoCreditos) {
                // Navegação pelo teclado
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Up)
                        selecionado = (selecionado - 1 + 3) % 3;
                    else if (event.key.code == Keyboard::Down)
                        selecionado = (selecionado + 1) % 3;
                    else if (event.key.code == Keyboard::Enter) {
                        switch (selecionado) {
                            case 0:
                                PlayPacMan(window);
                                break;
                            case 1:
                                mostrandoCreditos = true;
                                break;
                            case 2:
                                window.close();
                                break;
                        }
                    }
                }
                // Navegação pelo mouse
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (botaoJogar.contains(mousePos)) {
                        PlayPacMan(window);
                    } else if (botaoCreditos.contains(mousePos)) {
                        mostrandoCreditos = true;
                    } else if (botaoSair.contains(mousePos)) {
                        window.close();
                    }
                }
            } else {
                // Sai dos créditos ao pressionar tecla ou mouse
                if (event.type == Event::KeyPressed ||
                    (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)) {
                    mostrandoCreditos = false;
                }
            }
        }

        window.clear();
        if (mostrandoCreditos) {
            window.draw(creditosSprite);
        } else {
            window.draw(bgSprite);
        }
        window.display();
    }

    return 0;
}