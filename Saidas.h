#ifndef SAIDAS_H
#define SAIDAS_H
#include "Game.h"
void metricas(cenario& cena, std::chrono::time_point<std::chrono::high_resolution_clock> start);

void mensagem_menu();
void mensagem_creditos();
void mensagem_instrucoes();
void mensagem_perdeu();
void mensagem_ganhou();

#endif