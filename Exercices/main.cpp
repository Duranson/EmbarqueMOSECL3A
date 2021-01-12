//
//  main.cpp
//  Embarque-Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#include <iostream>

// #include "horse_run_single_file.cpp"

# include "pi_circle_sf.cpp"

int main(int argc, const char * argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    // Horse Run
    // HorseRunSF game = HorseRunSF();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    auto diff_sec = std::chrono::duration_cast<std::chrono::nanoseconds>(diff);
    std::cout << "Temps pour threads = "<< diff_sec.count() << "ns ";
    return 0;
}
