#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Record.hpp"

// O que acontece ao inveter o operador para >
bool record_cmp(Record &a, Record &b) {
  return a.milliseconds < b.milliseconds;
}

std::ostream &operator<<(std::ostream &os, Record const &r) {
  return os << '{' << r.name << ',' << r.milliseconds / 1000.0 << ',' << r.nivel << '}';
}

std::ostream &operator<<(std::ostream &os, std::vector<Record> &vet) {
  os << '<' << std::endl << '\t';
  if (vet.size() > 0) {
    os << vet[0];
  }
  for (int i = 1; i < vet.size(); i++) {
    os << ',' << std::endl << '\t' << vet[i];
  }
  os << std::endl << '>';
  return os;
}

// O que acontece ao remover o &?
void sort(std::vector<Record> &records) {
  std::sort(records.begin(), records.end(), record_cmp);
}

std::vector<Record> ranking_nivel (std::vector<Record> &records, int nivel){
  std::vector<Record> saida;
  for (int i = 0; i < records.size(); i++){
    if(records[i].nivel == nivel){
      saida.push_back(records[i]);
    }
  }
  return saida;
}
void print_ranking(std::vector<Record> &records) {
  
  std::vector<Record> iniciante = ranking_nivel(records, 0);
  std::vector<Record> intermediario = ranking_nivel(records, 1);
  std::vector<Record> avancado = ranking_nivel(records, 2);
  
  std::sort(iniciante.begin(), iniciante.end(), record_cmp);
  std::sort(intermediario.begin(), intermediario.end(), record_cmp);
  std::sort(avancado.begin(), avancado.end(), record_cmp);

  std::cout << "\033[1;32m---------- MELHORES PONTUAÇÕES-------------"<<std::endl;
  if (iniciante.size() > 0){
    std::cout <<"INICIANTE     "<<std::endl;
    std::cout <<"              NOME: " << iniciante[0].name <<std::endl;
    std::cout <<"              TEMPO: " << iniciante[0].milliseconds << "/s"<< std::endl;
  }
  if (intermediario.size() > 0){
    std::cout <<"INTERMEDIARIO "<<std::endl;
    std::cout <<"              NOME: " << intermediario[0].name <<std::endl;
    std::cout <<"              TEMPO: " << intermediario[0].milliseconds << "/s"<< std::endl;
  }
  if (avancado.size() > 0){
    std::cout <<"AVANÇADO      "<<std::endl;
    std::cout <<"              NOME: " << avancado[0].name <<std::endl;
    std::cout <<"              TEMPO: " << avancado[0].milliseconds << "/s"<< std::endl;
  }

  std::cout << "--------------------------------------------\033[0m"<<std::endl;
}

void write_records(std::vector<Record> &records, std::string &file_name) {

  std::ofstream file;
  file.open(file_name.c_str(), std::ios::app);
  if (file.is_open()) {
    file << records.size() << std::endl;
    for (Record rec : records) {
      file << rec.name << std::endl;
      file << rec.milliseconds << std::endl;
      file << rec.nivel << std::endl;
    }
    file.close();
  }
}

void read_records(std::vector<Record> &records, std::string &file_name) {
  std::ifstream inFile(file_name); 
  
  int lines = std::count(std::istreambuf_iterator<char>(inFile), 
             std::istreambuf_iterator<char>(), '\n');
  
  std::ifstream file;
  file.open(file_name.c_str());
  if (file.is_open()) {
    records.clear();
    int size = lines / 3;
    // file >> size;
    // file.ignore();
    for (int i = 0; i < size; i++) {
      Record rec;
      std::getline(file, rec.name);
      file >> rec.milliseconds;
      file >> rec.nivel;
      file.ignore();
      records.push_back(rec);
    }
    file.close();
  }
}

// Alocação dinâmica de memória

void write(char * file_name, int n, Record ** vector){

  std::ofstream file;
  file.open(file_name, std::ios::app);
  if (file.is_open()) {
    for (int i=0;i<n;i++) {
      file << vector[i]->name << std::endl;
      file << vector[i]->milliseconds << std::endl;
      file << vector[i]->nivel << std::endl;
    }
    file.close();
  }
}

Record ** read(char * file_name, int & n){
  Record ** vector = new Record*[100];
  std::ifstream file;
  file.open(file_name);
  if (file.is_open()) {
    n = 0;
    while(!file.eof()){
      Record * rec = new Record;
      std::getline(file, rec->name);
      file >> rec->milliseconds;
      file >> rec->nivel;
      file.ignore();
      vector[n++] = rec;
      
      if(n >= 100){
        Record ** bigger = new Record*[n];
        for(int i=0;i<n;i++){
          bigger[i] = vector[i];
        }
        delete [] vector;
        vector = bigger;
      }
    }
    file.close();
  }

  Record ** result = new Record*[n];
  for(int i=0;i<n;i++){
    result[i] = vector[i];
  }
  delete [] vector;
  return result;
}