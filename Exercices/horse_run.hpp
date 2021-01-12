//
//  horse_run.hpp
//  Embarque-Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#ifndef horse_run_hpp
#define horse_run_hpp

#include <stdio.h>
#include <iostream>

class HorseRun
{
private:
    int nHorses;
    int nColonnes;
    void courir(int);
public:
    HorseRun();
    HorseRun(int n_horse, int NB_colonnes);
    int start();
    static void moveto(int, int);
    static void erase_scr();
    static void set_curseur_visible();
    static void set_curseur_invisible();
    static void puts(char[]);
};

#endif /* horse_run_hpp */
