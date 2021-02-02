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
#include <vector>
#include <mutex>

#include "pi_circle.hpp"
#include "horse_run.hpp"
#include "test_parallel.hpp"

std::mutex mtx;

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
    std::vector<std::thread> tab_id1(nHorses);
    std::vector<int> tab_num_ligne_a_ecran(nHorses); // les valeurs envoy ́ees aux threads srand(time(NULL));
    std::vector<int> scores(nHorses);
    erase_scr(); // On efface l’ ́ecran
    set_curseur_invisible();
    moveto(nHorses + 2,1);
    mtx.lock();
    std::cout << "Creations des threads \n";
    mtx.unlock();
    for (int i = 0; i < nHorses; i++)
    {
        tab_num_ligne_a_ecran[i] = i + 1;
        scores[i] = 0;
        tab_id1[i] = std::thread(courir, tab_num_ligne_a_ecran[i], nColonnes, &mtx, &scores);
    }
    std::thread arbitre = std::thread(arbitrer,nHorses, nColonnes, &mtx, &scores);
    for (int i = 0; i< nHorses; i++){
        tab_id1[i].join();
    }
    arbitre.join();
    moveto(nHorses + 4,2);
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
    
    int nThreads = 12;
    
    // Test
    /*result = test();
    std::cout << "Test termine : verifiez que les lettres n'apparaissent pas sequentiellement." << std::endl;*/
    
    // Horse Run -> a executer à part : /Users/fabienduranson/Library/Developer/Xcode/DerivedData/Exercices-cgbqlfdjzfecmzgjnobqmdkxsixc/Build/Products/Release/Exercices

    /*int nColonnes = 130;
    result = horseRun(nColonnes, nThreads);
    std::cout << "Course terminée !" << std::endl;*/
    
    // Pi Circle
    int n_point = 1e7;
    // const int n_thread = 1;
    result = piCircle(n_point,nThreads);
    std::cout << "Résultat : Pi = " << result << std::endl;
    
    // Show results
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    auto diff_sec = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    std::cout << "Temps pour " << nThreads << " threads = "<< diff_sec.count() << "ms " << std::endl;
    return 0;
}


