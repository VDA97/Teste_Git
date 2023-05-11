#include "snakegame.hh"
// lógica para armazenar as coordenadas.
// struct para criar as coordenadas da cobra, utilizadas para a movimentação.

SnakeType::SnakeType(int s_col, int s_row)
{ // Toda vez que atualizar o x atualiza aqui
    s_x = s_col;
    s_y = s_row;
}

SnakeType::SnakeType()
{ // Inicia tudo com 0.
    s_x = 0;
    s_y = 0;
}

SnakeGame::SnakeGame()
{
    initscr();             // Possibiltia o uso de todas as funções da ncurses.
    nodelay(stdscr, true); // para a snake não parar
    keypad(stdscr, true);  // KEY_...,
    noecho();              // para não exibir teclas na tela
    curs_set(0);           // desliga o cursor.

    m_maxwidth = getmaxx(stdscr) / 2; // dividi a tela por 2
    m_maxheight = getmaxy(stdscr) / 2;
    m_snake_char = 'o'; // desenha a snake

    m_food_char = 'V';
    srand(time(NULL)); // para toda vez que iniciar o game a posição da comida ser randômica/aleatória
    m_insert_food();   // gera apenas, depois com o move ele vai realmente printar a comida.

    m_delay = 100000;  // microssegundos.
    m_direction = 'L'; // começa o movimento à esqueda
    m_tail_stop = false;
    m_score = 0;

    // criar uma linha horizontal na parte superior da janela cmd
    for (int i = 0; i < m_maxwidth - 1; i++)
    {
        move(0, i); // Função utilizada para printar no cmd
        if (i == 0)
        {
            addch('+'); // Add + no incio da linha
        }
        else if (i == (m_maxwidth - 2))
        {
            addch('+'); // Add + no final da linha
        }
        else
        {
            addch('-'); // Constroi o segmento de linha
        }
    }
    // criar uma linha vertical na parte esquerda da janela cmd
    for (int i = 1; i < m_maxheight - 1; i++)
    {
        move(i, 0); // Função utilizada para printar no cmd
        if (i == (m_maxheight - 2))
        {
            addch('+'); // Add + no incio da linha
        }
        else
        {
            addch('|'); // Constroi o segmento de linha
        }
    }

    // criar uma linha horizontal na parte inferior da janela cmd
    for (int i = 0; i < m_maxwidth - 1; i++)
    {
        move(m_maxheight - 2, i); // Função utilizada para printar no cmd
        if (i == 0)
        {
            addch('+'); // Add + no incio da linha
        }
        else if (i == (m_maxwidth - 2))
        {
            addch('+'); // Add + no final da linha
        }
        else
        {
            addch('-'); // Constroi o segmento de linha
        }
    }

    // criar uma linha vertical na parte direita da janela cmd
    for (int i = 1; i < m_maxheight - 1; i++)
    {
        move(i, m_maxwidth - 2); // Função utilizada para printar no cmd
        if (i == (m_maxheight - 2))
        {
            addch('+'); // Add + no incio da linha
        }
        else if (i == 0)
        {
            addch('+');
        }
        else
        {
            addch('|'); // Constroi o segmento de linha
        }
    }
    // desenha o tamanho inicial da snake
    for (int i = 0; i < 5; ++i)
    {
        snake.push_back(SnakeType(20 + i, 5));
    }
    // Lógica para posicionar a cobra
    for (int i = 0; i < snake.size(); ++i)
    {
        move(snake[i].s_y, snake[i].s_x);
        addch(m_snake_char);
    }

    // score no inicio
    move(m_maxheight - 1, 0);
    printw("%d", m_score);

    move(v_food.s_y, v_food.s_x);
    addch(m_food_char);
}

SnakeGame::~SnakeGame()
{
    nodelay(stdscr, false); // elimina comportamentos estranhos no terminal
    getch();                // desliga a captura de comandos do teclado.
    endwin();
}

void SnakeGame::m_insert_food()
{
    while (true)
    {
        int tmpx = rand() % m_maxwidth + 1; // Posições pra comida
        int tmpy = rand() % m_maxheight + 1;

        for (int i = 0; i < snake.size(); ++i)
        {
            // condições para evitar que a comida surja no meio da snake
            if (snake[i].s_x == tmpx && snake[i].s_y == tmpy)
            {
                continue;
            }
        }

        // Condições para evitar que a comida surja muito próximo das bordas.
        if (tmpx >= m_maxwidth - 2 || tmpy >= m_maxheight - 3)
        {
            continue;
        }

        v_food.s_x = tmpx;
        v_food.s_y = tmpy;
        break;
    }
    move(v_food.s_y, v_food.s_x);
    addch(m_food_char);
    refresh(); // para atualizar a tela
}
// movimento da cobra
void SnakeGame::movesnake()
{
    int tmp = getch();
    switch (tmp)
    {
    case KEY_LEFT:
        if (m_direction != 'R')
        {
            m_direction = 'L';
        }
        break;
    case KEY_UP:
        if (m_direction != 'D')
        {
            m_direction = 'U';
        }
        break;
    case KEY_DOWN:
        if (m_direction != 'U')
        {
            m_direction = 'D';
        }
        break;
    case KEY_RIGHT:
        if (m_direction != 'L')
        {
            m_direction = 'R';
        }
        break;
    case 'q':
        m_direction = 'Q';
        break;
    }

    if (!m_tail_stop)
    {
        move(snake[snake.size() - 1].s_y, snake[snake.size() - 1].s_x);
        printw(" ");
        refresh();
        snake.pop_back();
    }

    if (m_direction == 'L')
    {
        snake.insert(snake.begin(), SnakeType(snake[0].s_x - 1, snake[0].s_y));
    }
    else if (m_direction == 'R')
    {
        snake.insert(snake.begin(), SnakeType(snake[0].s_x + 1, snake[0].s_y));
    }
    else if (m_direction == 'U')
    {
        snake.insert(snake.begin(), SnakeType(snake[0].s_x, snake[0].s_y - 1));
    }
    else if (m_direction == 'D')
    {
        snake.insert(snake.begin(), SnakeType(snake[0].s_x, snake[0].s_y + 1));
    }

    move(snake[0].s_y, snake[0].s_x);
    addch(m_snake_char);
    refresh();
}

// funções de colisão
bool SnakeGame::_collide()
{
    // colisão com as bordas
    if (snake[0].s_x == 0 || snake[0].s_x == m_maxwidth - 1 || snake[0].s_y == 0 || snake[0].s_y == m_maxheight - 2)
    {
        return true;
    }
    // colisão com a própria cauda snake
    for (int i = 2; i < snake.size(); ++i)
    {
        if (snake[0].s_x == snake[i].s_x && snake[0].s_y == snake[i].s_y)
        {
            return true;
        }
    }

    // colisão de ponto pra quando a cobrar pegar a comidinha
    if (snake[0].s_x == v_food.s_x && snake[0].s_y == v_food.s_y)
    {
        m_tail_stop = true; // limpa a tela com novo tamanho de snake
        m_insert_food();    // cria uma nova comida
        m_score += 10;
        move(m_maxheight - 1, 0); // printa o score na posição abaixo
        printw("%d", m_score);
        if ((m_score % 50) == 0)
        {
            // Pra cada 50 pontos, vamos aumentar a velocidade, no caso diminuindo o delay da cobra
            m_delay -= 10000;
        }
    }
    else
    {
        m_tail_stop = false;
    }
    return false; // Para continuar o movimento
}

void SnakeGame::start()
{
    while (true)
    {

        if (_collide())
        {
            move(m_maxheight / 2, (m_maxwidth / 2) - 4); //-4 para descontar a palavra "over"
            printw("GAME OVER");
            break; // Para a execução da função start.
        }
        movesnake();

        if (m_direction == 'Q')
        {
            break;
        }

        usleep(m_delay);
    }
}
