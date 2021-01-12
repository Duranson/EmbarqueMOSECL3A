//
//  main.cpp
//  Embarque-Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#include <iostream>
#include <chrono>

// #include "horse_run_single_file.cpp"

# include "pi_circle_sf.cpp"

int main(int argc, const char * argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    // Horse Run
    // HorseRunSF game = HorseRunSF();
    
    // Pi Circle
    int n_point = 1e8;
    int n_threads = 4;
    PiCircleSF picircle = PiCircleSF(n_point,n_threads);
    auto result = picircle.estimate_pi();
    
    // Show results
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    auto diff_sec = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    std::cout << "Temps pour threads = "<< diff_sec.count() << "ms " << std::endl;
    std::cout << "Résultat : " << result << std::endl;
    return 0;
}
