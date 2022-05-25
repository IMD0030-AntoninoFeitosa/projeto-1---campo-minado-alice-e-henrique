#include "Game.h"
#include "Record.hpp"
#include "Saidas.h"

const std::string CONFIG_FILE = "config.cfg";
std::string file_name = "records.txt";

void opcoes_uso(void) {
  std::cout << "Usage: game [option]" << std::endl;
  std::cout << "Option:" << std::endl;
  std::cout << " -h or --help                  Display this information."
            << std::endl;
  std::cout << " -d or --difficulty <option>   Change the game difficulty for "
               "<option>"
            << std::endl;
  std::cout << "                               -b or --beginner" << std::endl;
  std::cout << "                               -i or --intermediary"
            << std::endl;
  std::cout << "                               -a or --advanced" << std::endl;
}

cenario criar_map(Difficulty level) {
  cenario cena;
  cena.marcacoes = 0;
  cena.jogadas = 0;
  cena.reveladas = 0;

  if (level == 0) {
    cena.dimensoes.minas = 10;
    cena.dimensoes.x = 10;
    cena.dimensoes.y = 10;
    // 10 x 10 + 10 minas
  } else if (level == 1) {
    cena.dimensoes.minas = 40;
    cena.dimensoes.x = 15;
    cena.dimensoes.y = 15;
    // 15 x 15 + 40 minas
  } else if (level == 2) {
    cena.dimensoes.minas = 100;
    cena.dimensoes.x = 15;
    cena.dimensoes.y = 30;
    // 15 x 30 + 100 minas
  } else {
    guardar_nivel(CONFIG_FILE, level); // nível de dificuldade
    opcoes_uso();
  }
  int l = cena.dimensoes.x;
  int c = cena.dimensoes.y;
  for (int i = 0; i < l; i++) {
    std::vector<char> temp;
    for (int j = 0; j < c; j++) {
      temp.push_back('X');
    }
    cena.mapa.push_back(temp);
  }

  return cena;
}

void mostrar_mapa(cenario cena) {
  for (int i = 0; i < cena.mapa.size(); i++) {

    if (i < 10 && cena.dimensoes.x > 10) {
      std::cout << " ";
    }

    /*
    8   1 2 3
    9   4 5 6
    10  7 8 9

    */

    std::cout << i << " - ";

    for (int j = 0; j < cena.mapa[i].size(); j++) {
      std::vector<int> celula;
      celula.push_back(i);
      celula.push_back(j);
      if (foi_revelada(cena, celula)) {

        int bombas = total_bombas(cena, celula);

        if (verifica_bomba(cena, celula)) {
total_bombas(cena, celula);
          if (foi_marcada(cena, celula)) {
            std::cout << " ? ";
          } else {
            std::cout << " X ";
          }

        } else {
          if (foi_marcada(cena, celula)) {
            std::cout << " ? ";
          } else {
            std::cout << " " << bombas << " ";
          }
        }
      } else {
        if (verifica_bomba(cena, celula)) {
          if (foi_marcada(cena, celula)) {
            std::cout << " ? ";
          } else {
            std::cout << " X ";
          }
        } else {
          if (foi_marcada(cena, celula)) {
            std::cout << " ? ";
          } else {
            std::cout << " " << cena.mapa[i][j] << " ";
          }
        }
      }
    }
    std::cout << std::endl;

    if (i == cena.dimensoes.x - 1) {
      if (cena.dimensoes.y > 10)
        std::cout << " ";
      std::cout << "     ";
      for (int j = 0; j < cena.mapa[i].size(); j++) {
        std::cout << "|  ";
      }
      std::cout << std::endl;
    }
  }
  std::cout << "     ";
  if (cena.dimensoes.y > 10)
    std::cout << " ";
  for (int j = 0; j < cena.mapa[0].size(); j++) {
    if (j > 9) {
      std::cout << j << " ";
    } else {
      std::cout << j << "  ";
    }
  }
}

void start_game(Difficulty level) {
  Record usuario;
  std::vector<Record> vec;
  std::cout << "Digite seu nome: ";
  std::cin >> usuario.name;
  usuario.nivel = level;
  
  bool preencheu = false;
  
  std::vector<std::vector<char>> mapa;
  cenario cena = criar_map(level);

  if (level == 0) {
    preencher_bombas(cena);
    preencheu = true;
  }
  std::cout << std::endl;
  mensagem_menu();

  bool sair = false;

  do {
    int resposta_usuario;
    std::cin >> resposta_usuario;
    switch (resposta_usuario) {
    case Opcoes::iniciar_jogo: {
      auto start = std::chrono::high_resolution_clock::now();
      do {
        mostrar_mapa(cena);
        metricas(cena, start);
        if (cena.reveladas ==
            ((cena.dimensoes.x * cena.dimensoes.y) - cena.dimensoes.minas)) {

          auto finish = std::chrono::high_resolution_clock::now();
          std::chrono::duration<double> elapsed = finish - start;
          usuario.milliseconds = elapsed.count();

          vec.push_back(usuario);

          Record **records{new Record *[vec.size()]};

          for (int i = 0; i < vec.size(); i++) {
            records[i] = new Record{vec[i]};
          }

          char output[] = "records.txt";

          write(output, vec.size(), records); // colocar

          for (int i = 0; i < vec.size(); i++) {
            delete records[i];
          }
          delete[] records;

          metricas(cena, start);
          mensagem_ganhou();
          abort();

        } else {
          // std::cout << std::endl;
          int l, c;
          char action;
          std::cout << "DIGITE UMA AÇÃO: R - Revelar ou B - Bandeira"
                    << std::endl;
          std::cout << "A: ";
          std::cin >> action;
          std::cout << "DIGITE UMA LINHA e UMA COLUNA:" << std::endl;
          std::cout << "L: ";
          std::cin >> l;
          std::cout << "C: ";
          std::cin >> c;
          std::vector<int> celula;

          celula.push_back(l);
          celula.push_back(c);

          if (action == 'R' || action == 'r') {
            bool eh_valida = celula_valida(cena, celula);
            bool eh_bomba = verifica_bomba(cena, celula);

            // comencando com 0
            if (level == 1 && !preencheu && eh_valida) { 
              // segundo argumento n pode ser sorteado como bombas
              preencher_bombas(cena, adjacentes(cena, celula));
            }

            // um dos lados adjacentes tem que ser uma bomba
            if (level == 2 && !preencheu && eh_valida) {
              preencher_bombas(cena, celula);
            }

            mostrar_mapa(cena);
            metricas(cena, start);
            preencheu = true;

            if (eh_bomba) {
              mensagem_perdeu();
              abort();
            }

            if (eh_valida) {
              cena.jogadas++;
              revelar(cena, l, c);
              std::vector<int> celula;
              celula.push_back(l);
              celula.push_back(c);

              if(total_bombas(cena, celula) == 0){
                abrir_celulas_adjacentes(cena, celula); 
              }
            } else {
              std::cout << "<< OPCAO INVÁLIDA >> " << std::endl;
            }
          } else if (action == 'B' || action == 'b') {
            mostrar_mapa(cena);
            metricas(cena, start);
            if (celula_valida(cena, celula)) {
              marcar_celula(cena, celula);
            } else {
              std::cout << "<< OPCAO INVÁLIDA >> " << std::endl;
            }

          } else {
            std::cout << "<< OPCAO INVÁLIDA >> " << std::endl;
          }
        }
      } while (true);
      break;
    }
    case Opcoes::creditos_jogo: {
      mensagem_creditos();
      break;
    }

    case Opcoes::instrucoes_jogo: {
      mensagem_instrucoes();
      break;
    }

    case Opcoes::sair_jogo: {
      sair = true;
      break;
    }
    }
  } while (!sair);
}

void guardar_nivel(const std::string config_file, Difficulty level) {
  std::ofstream file;
  file.open(config_file.c_str(), std::ifstream::out);
  if (file.is_open()) {
    switch (level) {
    case Difficulty::beginner:
      file << 'b';
      break;
    case Difficulty::intermediary:
      file << 'i';
      break;
    case Difficulty::advanced:
      file << 'a';
      break;
    }
    file.close();
  }
}

Difficulty carregar_nivel(const std::string config_file) {
  Difficulty level;
  std::ifstream file;
  file.open(config_file.c_str(), std::ifstream::in);
  if (file.is_open()) {
    char c;
    file >> c;
    switch (c) {
    case 'b':
      level = Difficulty::beginner;
      break;
    case 'i':
      level = Difficulty::intermediary;
      break;
    case 'a':
      level = Difficulty::advanced;
      break;
    }
    file.close();
  } else {
    guardar_nivel(config_file, Difficulty::beginner);
    level = Difficulty::beginner;
  }
  return level;
}

int main(int argc, char **argv) {
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "-h" || arg == "--help") {
      opcoes_uso();
    } else if (arg == "-d" || arg == "--difficulty") {

      if (argc > 2) {
        std::string newlevel = argv[2];
        if (newlevel == "-b" || newlevel == "--beginner") {
          guardar_nivel(CONFIG_FILE, Difficulty::beginner);
        } else if (newlevel == "-i" || newlevel == "--intermediary") {
          guardar_nivel(CONFIG_FILE, Difficulty::intermediary);
        } else if (newlevel == "-a" || newlevel == "--advanced") {
          guardar_nivel(CONFIG_FILE, Difficulty::advanced);
        } else {
          std::cout << "Unknown difficulty argument: " << newlevel << std::endl;
          opcoes_uso();
        }
      } else {
        std::cout << "It was expected a difficulty for: " << argv[1]
                  << std::endl;
        opcoes_uso();
      }
    } else {
      std::cout << "Unknown argument: " << argv[1] << std::endl;
      opcoes_uso();
    }
  } else {
    Difficulty level = carregar_nivel(CONFIG_FILE);
    start_game(level);
  }
  return 0;
}


int total_bombas(cenario cena, std::vector<int> celula) {
  int bombas = 0;

  int l = celula[0];
  int c = celula[1];

  std::vector<std::vector<int>> lados = adjacentes(cena, celula);

  for (int i = 0; i < lados.size(); i++) {
    if (verifica_bomba(cena, lados[i])) {
      bombas++;
    }
  }

  return bombas;
}

std::vector<std::vector<int>> adjacentes(cenario cena,
                                         std::vector<int> celula) {
  int l = celula[0], c = celula[1];
  std::vector<std::vector<int>> vetor_saida;

  // pegando celula de cima
  if (l - 1 >= 0) {
    std::vector<int> temp;
    temp.push_back(l - 1);
    temp.push_back(c);
    vetor_saida.push_back(temp);

    //  c
    /*  0 1 2 3 4
    l 0 x x x x x
      1 x x x x x
      2 x x x x x
      3 x x x x x
      4 x x x x x
    */

    if (c + 1 < cena.dimensoes.y) {
      std::vector<int> temp;
      temp.push_back(l - 1);
      temp.push_back(c + 1);
      vetor_saida.push_back(temp);
    }

    // pegando celula da esquerda acima
    if (c - 1 >= 0) {
      std::vector<int> temp;
      temp.push_back(l - 1);
      temp.push_back(c - 1);
      vetor_saida.push_back(temp);
    }
  }

  // pegando celula de baixo
  if (l + 1 < cena.dimensoes.x) {
    std::vector<int> temp;
    temp.push_back(l + 1);
    temp.push_back(c);
    vetor_saida.push_back(temp);

    // pegando celula da direita acima
    if (c + 1 < cena.dimensoes.y) {
      std::vector<int> temp;
      temp.push_back(l + 1);
      temp.push_back(c + 1);
      vetor_saida.push_back(temp);
    }

    // pegando celula da esquerda acima
    if (c - 1 >= 0) {
      std::vector<int> temp;
      temp.push_back(l + 1);
      temp.push_back(c - 1);
      vetor_saida.push_back(temp);
    }
  }

  // pegando celula da esquerda
  if (c - 1 >= 0) {
    std::vector<int> temp;
    temp.push_back(l);
    temp.push_back(c - 1);
    vetor_saida.push_back(temp);
  }

  // pegando celula da direita
  if (c + 1 < cena.dimensoes.y) {
    std::vector<int> temp;
    temp.push_back(l);
    temp.push_back(c + 1);
    vetor_saida.push_back(temp);
  }

  return vetor_saida;
}

bool celula_valida(cenario cena, std::vector<int> celula) {
  int l = celula[0];
  int c = celula[1];

  if (l >= 0 && l < cena.dimensoes.x && c >= 0 && c < cena.dimensoes.y) {
    return true;
  }

  return false;
}

void revelar(cenario &cena, int l, int c) {
  std::vector<int> celula;

  celula.push_back(l);
  celula.push_back(c);

  if (celula_valida(cena, celula)) {
    if (!foi_revelada(cena, celula)) {
      if (!verifica_bomba(cena, celula)) {
        if (!foi_marcada(cena, celula)) {
          cena.revelados.push_back(celula);
          cena.reveladas++;
        }
      }
    }
  }
}

void abrir_celulas_adjacentes(cenario &cena, std::vector<int> celula) {
  std::vector<std::vector<int>> lados = adjacentes(cena, celula);
  for (int i = 0; i < lados.size(); i++) {
    if (!verifica_bomba(cena, lados[i]) && !foi_revelada(cena, lados[i])) {
      revelar(cena, lados[i][0], lados[i][1]);
    }
  }
}

void marcar_celula(cenario &cena, std::vector<int> celula) {
  int l = celula[0];
  int c = celula[1];

  // ? marcado em posicao que n é bomba
  // & marcado em posicao que é bomba

  char v = cena.mapa[l][c];

  bool revelada = foi_revelada(cena, celula);

  if (!revelada) {
    if (v == '@') {
      cena.mapa[l][c] = '&';//bomba marcada
      cena.marcacoes++;
    } else if (v == '&') {
      cena.mapa[l][c] = '@';
      cena.marcacoes--;
    } else if (v == '?') {
      cena.mapa[l][c] = 'X';
      cena.marcacoes--;
    } else {
      cena.mapa[l][c] = '?';
      cena.marcacoes++;
    }
  }
}

bool foi_marcada(cenario &cena, std::vector<int> celula) {
  int l = celula[0];
  int c = celula[1];

  char v = cena.mapa[l][c];

  if (v == '&' || v == '?') {
    return true;
  }

  return false;
}

bool foi_revelada(cenario &cena, std::vector<int> celula) {
  int l = celula[0];
  int c = celula[1];
  for (int i = 0; i < cena.revelados.size(); i++) {
    if (l == cena.revelados[i][0] && c == cena.revelados[i][1]) {
      return true;
    }
  }
  return false;
}

/*
 *
 *Metodos das minas
 *
 */
std::vector<int> gerar_indices(cenario cena) {
  srand((unsigned)time(0));
  std::vector<int> indices;

  int linha = rand() % cena.dimensoes.x;
  int coluna = rand() % cena.dimensoes.y;

  indices.push_back(linha);
  indices.push_back(coluna);

  return indices;
}

void preencher_bombas(cenario &cena) {
  //caso iniciante
  int count_bombas = 0;
  do {
    std::vector<int> indices = gerar_indices(cena);
    int l = indices[0];
    int c = indices[1];
    //@ & ==  bomba
    if (cena.mapa[l][c] != '@') {
      cena.mapa[l][c] = '@';
      count_bombas++;
    }
  } while (count_bombas < cena.dimensoes.minas);
}
void preencher_bombas(cenario &cena, std::vector<std::vector<int>> invalidos) {
  int count_bombas = 0;

  //intermediario não pode escolher os lados adjacentes para ser bombas
  do {
    std::vector<int> indices = gerar_indices(cena);
    int l = indices[0];
    int c = indices[1];

    bool esta_ao_lado = false;


    //verifica se o indice gerado n esta ao lado
    for (int i = 0; i < invalidos.size(); i++) {
      if (l == invalidos[i][0] && c == invalidos[i][1]) {
        esta_ao_lado = true;
        break;
      }
    }

    if (cena.mapa[l][c] != '@' && !esta_ao_lado) {
      cena.mapa[l][c] = '@';
      count_bombas++;
    }
  } while (count_bombas < cena.dimensoes.minas);
}


void preencher_bombas(cenario &cena, std::vector<int> celula) {
  srand((unsigned)time(0));

  std::vector<std::vector<int>> lados = adjacentes(cena, celula);
  int size = lados.size();

  if (size > 4) {
    size = size - 4;
  }

  int bombas = (rand() % size);

  if(bombas == 0) bombas = 1;

  
  for (int i = 0; i < bombas; i++) {
    int l = lados[i][0];
    int c = lados[i][1];

    //nao pode ter bomba na celula que ele marcou
    if (l != celula[0] && c != celula[1]) {
      cena.mapa[l][c] = '@';
    }
  }

  int count_bombas = bombas;

  do {
    std::vector<int> indices = gerar_indices(cena);
    int l = indices[0];
    int c = indices[1];

    if (cena.mapa[l][c] != '@' && l != celula[0] && c != celula[1]) {
      // std::cout << "l: " << l << " c: " << c <<std::endl;
      cena.mapa[l][c] = '@';
      count_bombas++;
    }
  } while (count_bombas < cena.dimensoes.minas);
}

//sobrecarga

bool verifica_bomba(cenario cena, std::vector<int> celula) {
  int l = celula[0];
  int c = celula[1];
  if (cena.mapa[l][c] == '@' || cena.mapa[l][c] == '&') {
    return true;
  }
  return false;
}
