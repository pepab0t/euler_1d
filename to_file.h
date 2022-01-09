#ifndef TO_FILE_H_INCLUDED
#define TO_FILE_H_INCLUDED
#include "data.h"
#include "Sit.h"
#include <vector>
#include <fstream>

void to_file(std::ofstream& file, std::vector<Data> w, Sit S, double t);


#endif // TO_FILE_H_INCLUDED
