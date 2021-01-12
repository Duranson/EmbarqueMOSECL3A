//
//  main.cpp
//  Embarque-Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <future>

// #include "horse_run_single_file.cpp"

#include "pi_circle.hpp"
#include "horse_run.hpp"
#include "test_parallel.hpp"

double test()
{
    int n_threads = 4;
    std::thread Ids[n_threads];
    Ids[0] = std::thread(loop, 1000, "A");
    Ids[1] = std::thread(loop, 1000, "B");
    Ids[2] = std::thread(loop, 1000, "C");
    Ids[3] = std::thread(loop, 1000, "D");
    
    for (auto& id : Ids)
        id.join();
    
    return 0;
}

double horseRun(int nColonnes, int nHorses)
{
    std::thread tab_id1[nHorses];
    int tab_num_ligne_a_ecran[nHorses]; // les valeurs envoy ́ees aux threads srand(time(NULL));
    erase_scr(); // On efface l’ ́ecran
    set_curseur_invisible(); moveto(20,1);
    std::cout << "Creations des threads \n";
    for (int i = 0; i < nHorses; i++)
    {
        tab_num_ligne_a_ecran[i]=i+1;
        tab_id1[i] = std::thread(courir, tab_num_ligne_a_ecran[i], nColonnes);
    }
    moveto(20,2);
    std::cout<< "ATTENTIONS : Sans Join, Main termine et les threads s’arrˆetent\n" ;
    for (int i = 0; i< nHorses; i++){
        tab_id1[i].join();
    }
    std::cout << "\n\n\n\n Fin de main\n";
    set_curseur_visible();
    
    return 0;
}

double piCircle(int n_point, int n_thread)
{
    double pi = 0.;
    std::future<double> results[n_thread];
    for(int i = 0; i < n_thread ; i++)
        results[i] = std::async(std::launch::deferred,estimate_pi, n_point / n_thread);
    for(int i = 0; i < n_thread ; i++)
        pi += results[i].get();
    return pi / n_thread;
}

int main(int argc, const char * argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    double result = 0.;
    
    // Test
    /*result = test();
    std::cout << "Test termine : verifiez que les lettres n'apparaissent pas sequentiellement." << std::endl;*/
    
    // Horse Run
    /*int nColonnes = 130;
    int nHorses = 20;
    result = horseRun(nColonnes, nHorses);
    std::cout << "Course terminée !" << std::endl;*/
    
    // Pi Circle
    /*int n_point = 1e7;
    const int n_thread = 1;
    result = piCircle(n_point,n_thread);
    std::cout << "Résultat : Pi = " << result << std::endl;*/
    
    // Show results
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    auto diff_sec = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    std::cout << "Temps pour threads = "<< diff_sec.count() << "ms " << std::endl;
    return 0;
}


