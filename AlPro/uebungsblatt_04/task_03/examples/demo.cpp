#include <iostream>
#include <math.h>
#include <meine_funktionen.hpp>
#include <ostream>

// Demonstration der Funktionen
int main() {
    double x = M_PI / 4; // 45 Grad in Radiant
    double tol = 1e-6; // Toleranzwert

    const double result_sin = sinus(x, tol);
    const double result_cos = kosinus(x, tol);
    const double result_tan = tangens(x, tol);
    const double result_cot = kotangens(x, tol);

    std::cout << "Result sin" << result_sin << std::endl;
    std::cout << "Result cos" << result_cos << std::endl;
    std::cout << "Result tan" << result_tan << std::endl;
    std::cout << "Result cot" << result_cot << std::endl;

    return EXIT_SUCCESS;
}
