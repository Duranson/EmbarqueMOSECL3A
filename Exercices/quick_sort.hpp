//
//  quick_sort.hpp
//  Exercices
//
//  Created by Fabien Duranson on 02/02/2021.
//

#ifndef quick_sort_hpp
#define quick_sort_hpp

#include <stdio.h>

#include <stdio.h>

#include <vector>
#include <iostream>
//#include <cstdlib>
#include <ctime>
#include <chrono>
#include <future>
#include <mutex>

std::vector<double> combine(std::vector<double> v_inf, double pivot, std::vector<double> v_sup)
{
    v_inf.push_back(pivot);
    v_inf.insert( v_inf.end(), v_sup.begin(), v_sup.end() );
    return v_inf;
}

std::vector<double> merge(std::vector<std::vector<double>> v_list)
{
    std::vector<double> v;
    for (int i = 0; i < v_list.size(); i++)
    {
        std::vector<double> v2 = v_list[i];
        v.insert( v.end(), v2.begin(), v2.end() );
    }
    return v;
}

std::vector<double> quick_sort(std::vector<double> v)
{
    if (v.size() <= 1)
        return v;
    double pivot = v[0];
    std::vector<double> inf;
    std::vector<double> sup;
    for (int i = 1; i < v.size(); i++)
    {
        if (v[i] < pivot) {
            inf.push_back(v[i]);
        } else {
            sup.push_back(v[i]);
        }
    }
    return combine(quick_sort(inf), pivot, quick_sort(sup));
}

std::vector<std::vector<double>> split(std::vector<std::vector<double>> v_list, int depth, int maxDepth)
{
    if (depth >= maxDepth)
        return v_list;
    std::vector<std::vector<double>> v_list_return;
    for (std::vector<double>& v : v_list)
    {
        double pivot = v[0];
        std::vector<double> inf;
        std::vector<double> sup;
        for (int i = 1; i < v.size(); i++)
        {
            if (v[i] < pivot) {
                inf.push_back(v[i]);
            } else {
                sup.push_back(v[i]);
            }
        }
        sup.push_back(pivot);
        v_list_return.push_back(inf);
        v_list_return.push_back(sup);
    }
    return split(v_list_return, depth + 1, maxDepth);
}

#endif /* quick_sort_hpp */
