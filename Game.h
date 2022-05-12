#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <array>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

enum Difficulty { beginner, intermediary, advanced };
enum Opcoes { iniciar_jogo = 1, creditos_jogo, instrucoes_jogo, sair_jogo};

struct cenario {
  std::vector<std::vector<char>> mapa;
  std::vector<std::vector<int>> bombas;
  std::vector<std::vector<int>> vizinhos; 
  int reveladas;
  int marcacoes; 
  int jogadas;
  std::vector<std::vector<int>> revelados;

  struct {
    int x;
    int y;
    int minas;
  } dimensoes;

};
void opcoes_uso(void);
void guardar_nivel(const std::string config_file, Difficulty level);
Difficulty carregar_nivel(const std::string config_file);
void mostrar_mapa(cenario cena);


// CRIAR UM .H separado para esses cabeçalhos se quiser
std::vector<std::vector<int>> adjacentes(cenario cena, std::vector<int> celula);
void revelar(cenario &cena, int l, int c);
bool celula_valida(cenario cena, std::vector<int> celula);
void abrir_celulas_adjacentes(cenario& cena, std::vector<int> celula);
void marcar_celula(cenario &cena, std::vector<int> celula);
int total_bombas(cenario cena, std::vector<int> celula);
bool foi_marcada(cenario &cena, std::vector<int> celula);
bool foi_revelada(cenario &cena, std::vector<int> celula);

std::vector<int> gerar_indices(cenario cena);
void preencher_bombas(cenario & cena);
void preencher_bombas(cenario & cena, std::vector<std::vector<int>> invalidos);
void preencher_bombas(cenario & cena, std::vector<int> celula);
bool verifica_bomba(cenario cena, std::vector<int> celula);

#endif