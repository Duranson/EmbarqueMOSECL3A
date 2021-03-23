//
//  game_of_life.hpp
//  Exercices
//
//  Created by Fabien Duranson on 12/01/2021.
//

#ifndef game_of_life_hpp
#define game_of_life_hpp

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <opencv2/highgui/highgui.hpp>

int n_neighbors(std::vector<std::vector<bool>> grid, int i, int j)
{
    // Indices de 1 à n car les bords sont à false
    int countNeighbors = 0;
    if (i < 1 || j < 1 || i > grid[0].size() - 1 || j > grid.size() - 1)
        return 0;
    // S
    if (grid[j][i - 1])
        countNeighbors++;
    // SE
    if (grid[j + 1][i - 1])
        countNeighbors++;
    // E
    if (grid[j + 1][i])
        countNeighbors++;
    // NE
    if (grid[j + 1][i + 1])
        countNeighbors++;
    // N
    if (grid[j][i + 1])
        countNeighbors++;
    // NO
    if (grid[j - 1][i + 1])
        countNeighbors++;
    // O
    if (grid[j - 1][i])
        countNeighbors++;
    // SO
    if (grid[j - 1][i - 1])
        countNeighbors++;
    return countNeighbors;
}

void process_cell(int x, int y, std::vector<std::vector<bool>> grid, std::vector<std::vector<bool>> &grid_out, std::mutex *mtx)
{
    int width_out = grid_out[0].size();
    int height_out = grid_out.size();
    mtx->lock();
    int number_of_neighbors = n_neighbors(grid, x, y);
    switch (number_of_neighbors)
    {
        case 3:
            grid_out[(y - 1) % height_out][(x - 1) % width_out] = true;
            break;
        case 2:
            grid_out[(y - 1) % height_out][(x - 1) % width_out] = grid[y][x];
            break;
        default:
            grid_out[(y - 1) % height_out][(x - 1) % width_out] = false;
            break;
    }
    mtx->unlock();
}

std::vector<std::vector<bool>> process_partial_grid(int x1, int x2, int y1, int y2, std::vector<std::vector<bool>> grid, std::mutex *mtx)
{
    std::vector<std::vector<bool>> grid_out((y2 - y1), std::vector<bool>(x2 - x1));
    
    for (int x = x1; x < x2; x++)
        for (int y = y1; y < y2; y++)
            process_cell(x, y, grid, grid_out, mtx);
    
    return grid_out;
}

void show_state_img(int generation, std::vector<std::vector<bool>> grid)
{
    cv::Mat img(grid.size(), grid[0].size(), CV_8UC3, cv::Scalar(200,200, 200));
    
    for(int y=0; y < img.rows; y++)
    {
        for(int x=0; x < img.cols; x++)
        {
            if (grid[y][x])
            {
                // get pixel
                cv::Vec3b & color = img.at<cv::Vec3b>(y,x);
                color[1] = 0;
                color[2] = 0;
                // set pixel
                img.at<cv::Vec3b>(y,x) = color;
            }
        }
    }
    
    cv::imshow("Game of life", img);
    cv::resizeWindow("Game of life", 800, 400);
    cv::waitKey(1);
}

void show_state_print(int step, std::vector<std::vector<bool>> grid)
{
    std::cout << "\nStep " << step << " :" << std::endl;
    for ( std::vector<bool> &line : grid)
    {
        for (int i = 0; i < line.size(); i++)
        {
            std::cout << int(line[i]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<std::vector<bool>> generate_initial_grid(int width, int height, double proportion)
{
    std::vector<std::vector<bool>> grid_out;
    std::vector<bool> border_line;
    for (int i = - 1; i < width + 1; i++)
        border_line.push_back(false);
    grid_out.push_back(border_line);
    for (int i = 0; i < height; i ++)
    {
        std::vector<bool> line;
        line.push_back(false);
        for (int j = 0; j < width; j ++)
        {
            line.push_back( (double) std::rand() / RAND_MAX < proportion);
        }
        line.push_back(false);
        grid_out.push_back(line);
    }
    grid_out.push_back(border_line);
    return grid_out;
}

#endif /* game_of_life_hpp */
