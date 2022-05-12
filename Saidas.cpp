#include "Saidas.h"
void metricas(
    cenario &cena,
    std::chrono::time_point<std::chrono::high_resolution_clock> start) {
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  
  std::cout << "\033[0;31m" << std::endl;
  std::cout << "Abertas: " << cena.reveladas << "     " << std::endl;
  std::cout << "Jogadas: " << cena.jogadas << "           "
            << std::endl;
  std::cout << "Marcações: " << cena.marcacoes
            << "                                  " << std::endl;
  std::cout << "Minas: " << cena.dimensoes.minas - cena.marcacoes
            << "           " << std::endl;
  std::cout << "Tempo: " << elapsed.count() << "s"
            << "           " << std::endl;
  std::cout <<"\033[0m"<< std::endl << std::endl;
}

void mensagem_menu(){
		std::cout << "" << std::endl;
		std::cout << "|-----------------------------------------------------------------------------|" << std::endl;
		std::cout << "|                                 Menu                                        |" << std::endl;
		std::cout << "|                           (1) Iniciar Jogo                                  |" << std::endl;
		std::cout << "|                           (2) Créditos                                      |" << std::endl;
		std::cout << "|                           (3) Instruções do Jogo                            |" << std::endl;
		std::cout << "|                           (4) Sair do Jogo                                  |" << std::endl;
		std::cout << "|-----------------------------------------------------------------------------|" << std::endl;
		std::cout << std::endl << std::endl;
}

// Tela de créditos
void mensagem_creditos(){
		std::cout << "" << std::endl;
		std::cout << "|-----------------------------------------------------------------------------|" << std::endl;
		std::cout << "|                                 LP1 2022.1                                  |" << std::endl;
		std::cout << "|                         ANTONINO ALVES FEITOSA NETO                         |" << std::endl;
		std::cout << "|                                                                             |" << std::endl;
		std::cout << "|                                  Grupo:                                     |" << std::endl;
		std::cout << "|                        ALICE.                      |" << std::endl;
		std::cout << "|                     HENRIQUE                   |" << std::endl;
		std::cout << "|-----------------------------------------------------------------------------|" << std::endl;
}

void mensagem_instrucoes(){
		std::cout << "" << std::endl;
		std::cout << "|-----------------------------------------------------------------------------|" << std::endl;
		std::cout << "|                          Instruçoes do Jogo                                 |" << std::endl;
		std::cout << "|                                                                             |" << std::endl;
		std::cout << "|              R - Revela, exemplo: R 1 2 - Revela posição 12.                |" << std::endl;
		std::cout << "|              M - Flag, exemplo: M 1 2 - Insere uma flag na posição 12.      |" << std::endl;
		std::cout << "|              ? - Interrogação: ? 1 2 - Insere uma ? na posição 12.          |" << std::endl;
		std::cout << "|-----------------------------------------------------------------------------|" << std::endl;
}

void mensagem_perdeu(){
    std::cout << std::endl;
    std::cout << "VOCE PERDEU!!!!!";
	  std::cout << std::endl << std::endl;
}

// Ganhou o jogo
void mensagem_ganhou(){
    std::cout << std::endl;
    std::cout << "VOCE VENCEU";
    std::cout << std::endl;
}