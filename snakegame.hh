#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H
#include <iostream>  // time e rand
#include <ncurses.h> // para as funções da ncurses
#include <vector>    // lógica das coordenadas
#include <unistd.h>  // usleep()
// lógica para armazenar as coordenadas.
// struct para criar as coordenadas da cobra, utilizadas para a movimentação.
struct SnakeType
{
    int s_x, s_y; // s_ demonstra que faz parte da struct
    SnakeType(int, int);
    SnakeType();
};

class SnakeGame
{
protected:
    int m_maxwidth;  // tamanho da largura da tela
    int m_maxheight; // m_ indica membro
    std::vector<SnakeType> snake;
    char m_snake_char; // desenha a snake
    SnakeType v_food;  // coordenadas da comida
    char m_food_char;
    int m_delay;      //
    char m_direction; // escutar as direções
    bool m_tail_stop; // evita que ao aumentar a cauda, esta fique estática. OOOOOOO, sem movimento.
    int m_score;      // Pontuação

public:
    SnakeGame();
    ~SnakeGame();
    void m_insert_food();
    void movesnake();
    bool _collide();
    void start();
};

#endif
