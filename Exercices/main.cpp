//
//  main.cpp
//  Embarque-Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#include <iostream>
#include <thread>
#include <future>

// #include "horse_run_single_file.cpp"

#include "pi_circle.hpp"

int main(int argc, const char * argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    // Horse Run
    // HorseRunSF game = HorseRunSF();
    
    // Pi Circle
    double pi = 0.;
    int n_point = 1e7;
    int n_thread = 20;
    std::future<double> results[n_thread];
    for(int i = 0; i < n_thread ; i++)
        results[i] = std::async(std::launch::deferred,estimate_pi, n_point / n_thread);
    for(int i = 0; i < n_thread ; i++)
        pi += results[i].get();
    double result = pi / n_thread;
    
    // Show results
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    auto diff_sec = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    std::cout << "Temps pour threads = "<< diff_sec.count() << "ms " << std::endl;
    std::cout << "Résultat : " << result << std::endl;
    return 0;
}
