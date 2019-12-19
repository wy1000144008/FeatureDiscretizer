#include <iostream>
#include "dataset.h"



int main() 
{
	//para 1 bin_num, 2 feature_num, 3 dataset path, 4 delimiter, 5 is skip head
	Dataset ds(5,"raw/bank-note-exp.txt",",",true); 
	ds.init();
	ds.printInfo();
	ds.make_bins(5);
    std::cout << "Test"  << std::endl; 
    return 0;
}