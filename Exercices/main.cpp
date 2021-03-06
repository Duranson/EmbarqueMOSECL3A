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
#include <unistd.h>

#include "pi_circle.hpp"
#include "horse_run.hpp"
#include "test_parallel.hpp"
#include "quick_sort.hpp"
#include "game_of_life.hpp"

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

std::vector<double> quick_sort_multi_thread(std::vector<double> v, int *nThreads)
{
    int maxDepth = floor(log2(*nThreads));
    *nThreads = pow(2,maxDepth);
    if (v.size() < 2 * *nThreads)
    {
        std::cout << "Pas de mutli-thread ici." << std::endl;
        return (quick_sort(v));
    }
    else
    {
        std::vector<std::vector<double>> v_list; v_list.push_back(v);
        std::vector<std::vector<double>> splitted = split(v_list, 0, maxDepth);
        
        std::vector<std::future<std::vector<double>>> splitted_sorted_future(splitted.size());
        std::vector<std::vector<double>> splitted_sorted(splitted.size());
        
        for(int i = 0; i < splitted.size() ; i++)
            splitted_sorted_future[i] = std::async(std::launch::deferred,quick_sort, splitted[i]);
        for(int i = 0; i < splitted.size() ; i++)
            splitted_sorted[i] = splitted_sorted_future[i].get();
        return merge(splitted_sorted);
    }
}

void game_of_life_multi_thread(int nThreads, int grid_width, int grid_height, double proportion, int max_iter)
{
    cv::namedWindow("Game of life", cv::WINDOW_NORMAL);
    if (grid_width < grid_height)
        std::swap(grid_width, grid_height);
    int columns = pow(2,ceil(log2(sqrt(nThreads))));
    int lines = round(pow(2,floor(log2(nThreads))) / columns);
    
    std::vector<std::vector<bool>> grid = generate_initial_grid(grid_width, grid_height, proportion);
    
    show_state_img(0,grid);
    //show_state_print(0,grid);
    cv::resizeWindow("Game of life", 800, 400);
    
    std::vector<std::future<std::vector<std::vector<bool>>>> threads(columns * lines);
    
    std::vector<std::vector<std::vector<bool>>> results(columns * lines);
    
    for (int iter = 0; iter < max_iter; iter++)
    {
        //std::vector<std::vector< std::vector<std::vector<bool>> >> partial_grids;
        for (int column = 0; column < columns; column++)
        {
            for (int line = 0; line < lines; line++)
            {
                int x1 = grid_width / columns * column + 1;
                int x2 = grid_width / columns * (column + 1) + 1;
                int y1 = grid_height / lines * line + 1;
                int y2 = grid_height / lines * (line + 1) + 1;
                threads[column * lines + line] = std::async(std::launch::deferred,process_partial_grid,x1, x2, y1, y2, grid, &mtx);
                //results[column * lines + line] = process_partial_grid(x1, x2, y1, y2, grid, &mtx);
            }
        }
        for (int column = 0; column < columns; column++)
        {
            for (int line = 0; line < lines; line++)
            {
                int x1 = grid_width / columns * column + 1;
                int x2 = grid_width / columns * (column + 1) + 1;
                int y1 = grid_height / lines * line + 1;
                int y2 = grid_height / lines * (line + 1) + 1;
                
                int i = column * lines + line;
                
                std::vector<std::vector<bool>> partial_grid = threads[i].get();
                //std::vector<std::vector<bool>> partial_grid = results[i];
                
                for (int x = x1; x < x2; x++)
                {
                    for (int y = y1; y < y2; y++)
                    {
                        grid[y][x] = partial_grid[y - y1][x - x1];
                    }
                }
            }
        }
        //usleep(1000); // Sleep 1000 milliseconds
        //show_state_print(iter + 1, grid);
        show_state_img(iter + 1,grid);
        /*int c = 0;
        std::cout << "Next generation ? (Press 1)" << std::endl;
        std::cin >> c;
        if (c != 1)
            break;*/
        std::cout << "Generation " << iter + 1 << " done." << std::endl;
    }
    cv::destroyWindow("A_good_name");
}

int main(int argc, const char * argv[]) {
    std::cout << "Quel programme voulez-vous executer ? \n 1. \t Test \n 2. \t Horse Run \n 3. \t Pi estimation \n 4. \t Quick Sort \n 5. \t Game of Life \n" << std::endl;
    int script;
    std::cin >> script;
    
    auto start = std::chrono::high_resolution_clock::now();
    double result = 0.;
    
    int nThreads = 8;
    
    switch (script)
    {
        case 1:
        {
            // Test
            result = test();
            std::cout << "Test termine : verifiez que les lettres n'apparaissent pas sequentiellement." << std::endl;
            break;
            
        }
        case 2:
        {
            // Horse Run -> a executer à part : /Users/fabienduranson/Library/Developer/Xcode/DerivedData/Exercices-cgbqlfdjzfecmzgjnobqmdkxsixc/Build/Products/Release/Exercices
            int nColonnes = 130;
            std::cout << "Nombre de chevaux :";
            std::cin >> nThreads;
            start = std::chrono::high_resolution_clock::now();
            result = horseRun(nColonnes, nThreads);
            std::cout << "Course terminée !" << std::endl;
            break;
            
        }
        case 3:
        {
            // Pi Circle
            int n_point = 1e7;
            std::cout << "Nombre de thread pour le calcul de pi :";
            std::cin >> nThreads;
            start = std::chrono::high_resolution_clock::now();
            result = piCircle(n_point,nThreads);
            std::cout << "Résultat : Pi = " << result << std::endl;
            break;
            
        }
        case 4:
        {
            // Quick Sort
            int n_element = 1e6;
            std::cout << "Nombre de thread pour le tri quick sort (Puissance de 2) :";
            std::cin >> nThreads;
            start = std::chrono::high_resolution_clock::now();
            std::vector<double> v;
            for (int i = 0; i < n_element; i++)
            {
                v.push_back( rand() % (2 * n_element) - n_element );
            }
            std::vector<double> v_sorted = quick_sort_multi_thread(v, &nThreads);
            std::cout << "Résultat trié :" << std::endl;
            /*for(double& d : v_sorted)
                std::cout << d << ", " ;
            std::cout << std::endl;*/
            break;
            
        }
        case 5:
        {
            // Horse Run -> a executer à part : /Users/fabienduranson/Library/Developer/Xcode/DerivedData/Exercices-cgbqlfdjzfecmzgjnobqmdkxsixc/Build/Products/Release/Exercices
            int nThreads = 4;
            int grid_width = 128;
            int grid_height = 64;
            double proportion = 0.3;
            int max_iter = 10000;
            /*std::cout << "Nombre de threads (1,2,4,8,16,32 ou 64) :";
            std::cin >> nThreads;
            std::cout << "Largeur de la grille :";
            std::cin >> grid_width;
            std::cout << "Hauteur de la grille :";
            std::cin >> grid_height;
            std::cout << "Proportion de cellules vivantes à l'étape 0 :";
            std::cin >> proportion;
            std::cout << "Nombre d'étapes :";
            std::cin >> max_iter;*/
            start = std::chrono::high_resolution_clock::now();
            game_of_life_multi_thread(nThreads, grid_width, grid_height, proportion, max_iter);
            std::cout << "Simulation terminée !" << std::endl;
            break;
            
        }
        default:
        {
            std::cout << "Pas de programme correspondant au chiffre " << script << std::endl;
            break;
            
        }
    }
    
    // Show results
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    auto diff_sec = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    std::cout << "Temps pour " << nThreads << " threads = "<< diff_sec.count() << "ms " << std::endl;
    return 0;
}


