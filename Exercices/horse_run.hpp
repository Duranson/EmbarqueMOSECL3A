//
//  horse_run_single_file.cpp
//  Embarque-Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#include <stdio.h>

#include <vector>
#include <iostream>
//#include <cstdlib>
#include <ctime>
#include <chrono>
#include <future>
#include <mutex>


char nth_letter(int n)
{
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[n - 1];
}
void moveto(int lig, int col) { printf("\033[%d;%df",lig, col); };
void erase_scr() { printf("\033[2J");}
void erase_line() { printf("\033[1K");}
void set_curseur_visible() { printf("\x1B[?25h"); }
void set_curseur_invisible() { printf("\x1B[?25l"); }

void courir(int ma_ligne, int nColonnes, std::mutex *mtx, std::vector<int> *scores) {
    std::vector<int> &sc = *scores;
    moveto(ma_ligne, 30);
    char mon_signe[3] = {nth_letter(ma_ligne), '>', 0};
    for (int i=0; i < nColonnes; i++)
    {
        mtx->lock();
        moveto(ma_ligne, i);
        erase_line();
        moveto(ma_ligne, i);
        puts(mon_signe);
        sc[ma_ligne - 1] += 1;
        /*moveto(ma_ligne, 0);
        std::cout << sc[ma_ligne - 1];*/
        mtx->unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50*(rand()%3+1)));
    }
}

void arbitrer(int nHorses, int nColumns, std::mutex *mtx, std::vector<int> *scores)
{
    std::vector<int> &sc = *scores;
    int ma_ligne = nHorses + 2;
    int first_horse_id = 0;
    int first_horse_score = 0;
    while (*std::min_element(sc.begin(),sc.end()) < nColumns)
    {
        first_horse_id = std::max_element(sc.begin(),sc.end()) - sc.begin();
        first_horse_score = *std::max_element(sc.begin(),sc.end());
        mtx->lock();
        if (first_horse_score == nColumns){
            moveto(ma_ligne + 1, 0);
            std::cout << "Le cheval " << nth_letter(first_horse_id + 1) << " a gagné !" << std::endl;;
            mtx->unlock();
            return;
        } else {
            moveto(ma_ligne, 0);
            erase_line();
            moveto(ma_ligne, 0);
            std::cout << "Le cheval " << nth_letter(first_horse_id + 1) << " est en tête avec un score de " << first_horse_score << " sur " << nColumns;
        }
        mtx->unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return;
}
