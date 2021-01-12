//
//  test_parallel.cpp
//  Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#include <stdio.h>
#include <iostream>

void loop(int N, std::string word)
{
    for(int i = 0; i < N; i++)
        std::cout << word;
}
