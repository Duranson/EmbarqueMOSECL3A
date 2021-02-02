//
//  horse_run_single_file.cpp
//  Embarque-Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#include <stdio.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
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

void courir(int ma_ligne, int nColonnes, std::mutex *mtx) {
    moveto(ma_ligne, 30);
    mtx->lock();
    std::cout << "Thread" << ma_ligne << "lance \n";
    mtx->unlock();
    int i;
    char mon_signe[3] = {nth_letter(ma_ligne), '>', 0};
    // donnerra de ’A’.. ’Z’
    for (i=0; i < nColonnes; i++)
    {
        mtx->lock();
        moveto(ma_ligne, i);
        erase_line();
        moveto(ma_ligne, i);
        puts(mon_signe);
        mtx->unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50*(rand()%3+1)));
    }
    set_curseur_visible();
}
