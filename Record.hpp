#ifndef RECORD_H_
#define RECORD_H_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Record {
  std::string name;
  double milliseconds;
  int nivel;
};


bool record_cmp(Record &a, Record &b);

std::ostream &operator<<(std::ostream &os, Record const &r);

std::ostream &operator<<(std::ostream &os, std::vector<Record> &vet);

void sort(std::vector<Record> &records);
void print_ranking(std::vector<Record> &records);
std::vector<Record> ranking_nivel (std::vector<Record> &records);

void write_records(std::vector<Record> &records, std::string &file_name);

void read_records(std::vector<Record> &records, std::string &file_name);


void write(char * file_name, int n, Record ** vector);

Record ** read(char * file_name, int & n);

#endif