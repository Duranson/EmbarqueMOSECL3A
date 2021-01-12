//
//  horse_run.cpp
//  Embarque-Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#include "horse_run.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <future>

HorseRun::HorseRun()
{
    nHorses = 20;
    nColonnes = 130;
}

HorseRun::HorseRun( int n_horse, int NB_colonnes)
{
    nHorses = n_horse;
    nColonnes = NB_colonnes;
}

char nth_letter(int n)
{
    assert(n >= 0 && n <= 25);
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[n];
}

void courir(int ma_ligne, int nColonnes) {
    std::cout << "Thread" << ma_ligne << "lance \n";
    int i;
    char mon_signe[3] = {nth_letter(ma_ligne), '>', 0};
    // donnerra de ’A’.. ’Z’
    for (i=0; i < nColonnes; i++)
    {
        HorseRun::moveto(ma_ligne, i);
        HorseRun::erase_scr();
        HorseRun::moveto(ma_ligne, i);
        HorseRun::puts(mon_signe);
        usleep(50000*(rand()%3+1));
        // OU std::this thread::sleep for(std::chrono::nanoseconds(50000*(rand()%3+1)));
    }
    HorseRun::set_curseur_visible();
}

void HorseRun::moveto(int lig, int col) { printf("\033[%d;%df",lig, col); };
void HorseRun::erase_scr() { printf("\033[2J");}
void HorseRun::set_curseur_visible() { printf("\x1B[?25h"); }
void HorseRun::set_curseur_invisible() { printf("\x1B[?25l"); }
void HorseRun::puts( char* mon_signe){ printf(mon_signe); }

int HorseRun::start()
{
    int i;
    std::thread tab_id1[nHorses];
    int tab_num_ligne_a_ecran[nHorses]; // les valeurs envoy ́ees aux threads srand(time(NULL));
    erase_scr(); // On efface l’ ́ecran
    set_curseur_invisible(); moveto(20,1);
    std::cout << "Creations des threads \n";
    for (i = 0; i < nHorses; i++)
    {
        tab_num_ligne_a_ecran[i]=i+1;
        tab_id1[i] = std::thread(courir, tab_num_ligne_a_ecran[i], nColonnes);
    }
    moveto(20,2);
    std::cout<< "ATTENTIONS : Sans Join, Main termine et les threads s’arrˆetent\n" ;
    for (i=0; i< nHorses; i++){
        tab_id1[i].join();
    }
    std::cout << "\n\n\n\n Fin de main\n";
    set_curseur_visible();
    return 0;
}
