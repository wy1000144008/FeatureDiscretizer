#include <iostream>
#include "dataset.h"



int main() 
{
	Dataset ds(5,"raw/bank-note-exp.txt",",",true); 
	ds.init();
	ds.printInfo();
    std::cout << "Test"  << std::endl; 
    return 0;
}