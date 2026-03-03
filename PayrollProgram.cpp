#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

// Progressive (marginal) federal tax per the table in the assignment.
double calcFederalTaxAnnual(double salary) {
    double tax = 0.0;

    struct Bracket { double lo, hi, rate; };
    Bracket brackets[] = {
        {0.0,     5000.0,  0.00},
        {5000.0,  10000.0, 0.06},
        {10000.0, 20000.0, 0.15},
        {20000.0, 40000.0, 0.20},
        {40000.0, 60000.0, 0.25},
        {60000.0, numeric_limits<double>::infinity(), 0.30}
    };

    for (const auto& b : brackets) {
        if (salary > b.lo) {
            double portion = min(salary, b.hi) - b.lo;
            tax += portion * b.rate;
        }
    }
    return tax;
}

int main() {
    const string filename = "employees.txt";
    ifstream fin(filename);

    if (!fin) {
        cerr << "Error: Could not open file '" << filename << "'.\n"
             << "Make sure employees.txt is in the SAME folder as your .cpp / .exe.\n";
        return 1;
    }

    cout << fixed << setprecision(2);

    cout << left
         << setw(12) << "SSN"
         << setw(28) << "Name (Last, First M.)"
         << right << setw(16) << "Net Monthly"
         << "\n";
    cout << string(56, '-') << "\n";

    string ssn, first, middle, last;
    double salaryAnnual;
    int coveredCount;
    char retirementFlag;

    while (fin >> ssn >> first >> middle >> last >> salaryAnnual >> coveredCount >> retirementFlag) {
        // Gross monthly
        double grossMonthly = salaryAnnual / 12.0;

        // Federal monthly (annual progressive / 12)
        double federalMonthly = calcFederalTaxAnnual(salaryAnnual) / 12.0;

        // State monthly (6% annual / 12)
        double stateMonthly = (salaryAnnual * 0.06) / 12.0;

        // Insurance monthly ($100 per covered person)
        double insuranceMonthly = 100.0 * coveredCount;

        // Retirement monthly (6% if elected)
        double retirementMonthly = 0.0;
        if (retirementFlag == 'Y' || retirementFlag == 'y') {
            retirementMonthly = (salaryAnnual * 0.06) / 12.0;
        }

        // Net monthly
        double netMonthly = grossMonthly
                          - federalMonthly
                          - stateMonthly
                          - insuranceMonthly
                          - retirementMonthly;

        // Format name: Last, First M.
        string formattedName = last + ", " + first + " " + middle;

        cout << left
             << setw(12) << ssn
             << setw(28) << formattedName
             << right << setw(16) << netMonthly
             << "\n";
    }

    return 0;
}