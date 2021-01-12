//
//  pi_circle_sf.cpp
//  Embarque-Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#ifndef pi_circle_hpp
#define pi_circle_hpp

#include <stdio.h>
#include <iostream>
    
double randFloat() { return (double) std::rand() / (double) RAND_MAX; }

double estimate_pi(int n_iter_subfunc)
{
    int count = 0;
    for (int i = 0; i < n_iter_subfunc; i++)
    {
        double x = randFloat();
        double y = randFloat();
        if (x*x + y*y < 1) count++;
    }
    return 4. * (double) count / (double) n_iter_subfunc;
}

#endif
