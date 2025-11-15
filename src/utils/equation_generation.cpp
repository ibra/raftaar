#include "equation_generation.hpp"
#include <cstdlib>
#include <ctime>

// this function generatest a list of random mathematical equations for the typing test
// it follows webwork syntax or latex for the equations to help the user improve

std::vector<std::string> generate_equations(int count)
{
    std::vector<std::string> equations;
    srand(time(0));

    for (int i = 0; i < count; i++)
    {
        int type = rand() % 10;

        // generating a bunch of random constants here that can be plugged into the equations
        int a = (rand() % 48) + 1;
        int b = (rand() % 48) + 1;
        int c = (rand() % 20) + 1;
        int n = (rand() % 4) + 2;

        std::string eq;
        switch (type)
        {
        case 0:
            eq = "x^2+" + std::to_string(b) + "x+" + std::to_string(c) + "=0";
            break;
        case 1:
            eq = std::to_string(a) + "x^2-" + std::to_string(b) +
                 "x=" + std::to_string(c);
            break;
        case 2:
            eq = "x^" + std::to_string(n) + "";
            break;
        case 3:
            eq =
                "dy/dx=" + std::to_string(a) + "x^" + std::to_string(n);
            break;
        case 4:
            eq = "int(" + std::to_string(a) + "x^" + std::to_string(n) + ")dx";
            break;
        case 5:
            eq = std::to_string(a) + "x+" + std::to_string(b) +
                 "y=" + std::to_string(c);
            break;
        case 6:
            eq = "sqrt(" + std::to_string(a * a) + ")+" + std::to_string(b);
            break;
        case 7:
            eq = "e^(" + std::to_string(a) + "x)=" + std::to_string(c);
            break;
        case 8:
            eq = "log_" + std::to_string(a) + "(" + std::to_string(c) + ")";
            break;
        case 9:
            eq = std::to_string(a) + "sin(x)+" + std::to_string(b) + "cos(x)";
            break;
        }

        equations.push_back(eq);
    }

    return equations;
}