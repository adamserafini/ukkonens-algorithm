#pragma once
#include <iostream>
#include <fstream>
#include <string>

class FASTA_FileReader {
public:
    FASTA_FileReader(std::string);
    std::string parse();
    std::ifstream infile;
};
