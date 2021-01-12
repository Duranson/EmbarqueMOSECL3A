//
//  pi_circle_sf.cpp
//  Embarque-Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#include <stdio.h>
#include <iostream>

class PiCircleSF
{
public:
    int n_points;
    int n_threads;
    
    PiCircleSF(int nPoints, int nThreads)
    {
        n_points = nPoints;
        n_threads = nThreads;
    }
    
    double rand() { return (double) std::rand() / (double) RAND_MAX; }
    
    double estimate_pi()
    {
        int count = 0;
        for (int i = 0; i < n_points; i++)
        {
            double x = rand();
            double y = rand();
            if (x*x + y*y < 1) count++;
        }
        return 4. * (double) count / (double) n_points;
    }
};
