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
#include <unistd.h>
#include <thread>
#include <chrono>
#include <future>

class HorseRunSF
{
public:
    int nColonnes = 20;
    int nHorses = 130;
    char nth_letter(int n)
    {
        assert(n >= 0 && n <= 25);
        return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[n];
    }
    void moveto(int lig, int col) { printf("\033[%d;%df",lig, col); };
    void erase_scr() { printf("\033[2J");}
    void set_curseur_visible() { printf("\x1B[?25h"); }
    void set_curseur_invisible() { printf("\x1B[?25l"); }
    void puts( char* mon_signe){ printf(mon_signe); }
    
    void courir(int ma_ligne) {
        std::cout << "Thread" << ma_ligne << "lance \n";
        int i;
        char mon_signe[3] = {nth_letter(ma_ligne), '>', 0};
        // donnerra de ’A’.. ’Z’
        for (i=0; i < nColonnes; i++)
        {
            moveto(ma_ligne, i);
            erase_scr();
            moveto(ma_ligne, i);
            puts(mon_signe);
            usleep(50000*(rand()%3+1));
            // OU std::this thread::sleep for(std::chrono::nanoseconds(50000*(rand()%3+1)));
        }
        set_curseur_visible();
    }
    
    int start()
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
            tab_id1[i] = std::thread(courir, tab_num_ligne_a_ecran[i]);
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
};
