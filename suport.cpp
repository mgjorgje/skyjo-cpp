#ifndef SUPORT_CPP # Look into header guards... enough for today
#define SUPPORT_CPP
#include <iostream>

struct playerInput{
    int row;
    int column;
};

playerInput getPlayerInput(){
    std::cout<<"Enter a row: ";        
        
        playerInput p;
        std::cin>> p.row;

        std::cout<<"Enter a column: ";
        std::cin >> p.column;

        return p;

}

#endif