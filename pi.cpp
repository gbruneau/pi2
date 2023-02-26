/***************************************************************
 * Computing pi by Binary Splitting Algorithm with GMP libarary.
 **************************************************************/
#include <cmath>
#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <stdint.h>
#include <iomanip>

using namespace std;


template<class T>
void Format1000(std::ostream& os, T n)
{
    T rest = n % 1000; //"last 3 digits"
    n /= 1000;         //"begining"

    if (n > 0) {
        Format1000(os, n); //printing "begining"

        //and last chunk
        os << ' ' << std::setfill('0') << std::setw(3) << rest;
    }
    else
        os << rest; //first chunk of the number
}

uint64_t stoUint64(std::string aString) {
  //  String to uint64_t
  uint64_t value;
  std::istringstream iss(aString);
  iss >> value;
  return value;
}


struct PQT
{
    mpz_class P, Q, T;
};

class Chudnovsky
{
    // Declaration
    mpz_class A, B, C, D, E, C3_24;  // GMP Integer
    int64_t DIGITS, PREC, N;             // Integer
    double DIGITS_PER_TERM;          // Long
    clock_t t0, t1, t2;              // Time
    PQT compPQT(int64_t n1, int64_t n2);     // Computer PQT (by BSA)

public:
    Chudnovsky(int64_t nbrDigit);    // Constructor
    void compPi();                   // Compute PI
};

/*
 * Constructor
 */
Chudnovsky::Chudnovsky(int64_t nbrDigit)
{
    // Constants
    DIGITS = nbrDigit;
    A      = 13591409;
    B      = 545140134;
    C      = 640320;
    D      = 426880;
    E      = 10005;
    DIGITS_PER_TERM = 14.1816474627254776555;  // = log(53360^3) / log(10)
    C3_24  = C * C * C / 24;
    N      = DIGITS / DIGITS_PER_TERM;
    PREC   = DIGITS * log2(10);
}

/*
 * Compute PQT (by Binary Splitting Algorithm)
 */
PQT Chudnovsky::compPQT(int64_t n1, int64_t n2)
{
    int64_t m;
    PQT res;

    if (n1 + 1 == n2) {
        res.P  = (2 * n2 - 1);
        res.P *= (6 * n2 - 1);
        res.P *= (6 * n2 - 5);
        res.Q  = C3_24 * n2 * n2 * n2;
        res.T  = (A + B * n2) * res.P;
        if ((n2 & 1) == 1) res.T = - res.T;
    } else {
        m = (n1 + n2) / 2;
        PQT res1 = compPQT(n1, m);
        PQT res2 = compPQT(m, n2);
        res.P = res1.P * res2.P;
        res.Q = res1.Q * res2.Q;
        res.T = res1.T * res2.Q + res1.P * res2.T;
    }

    return res;
}

/*
 * Compute PI
 */
void Chudnovsky::compPi()
{
     
    string filesPath = "./output/";
    string logFileName = filesPath +"pi-" + to_string(DIGITS) + ".log";
    string piFileName = filesPath + "pi-" + to_string(DIGITS) + ".txt";
    ofstream logFile(logFileName);

    logFile << "**** PI Computation ( ";
    Format1000(logFile,DIGITS);
    logFile << " digits )" << endl;
    cout << "**** PI Computation ( " << DIGITS << " digits )" << endl;

    // Time (start)
    t0 = clock();

    // Compute Pi
    PQT PQT = compPQT(0, N);
    mpf_class pi(0, PREC);
    pi  = D * sqrt((mpf_class)E) * PQT.Q;
    pi /= (A * PQT.Q + PQT.T);

    // Time (end of computation)
    t1 = clock();
    logFile << "TIME (COMPUTE): "
         << (double)(t1 - t0) / CLOCKS_PER_SEC
         << " seconds." << endl;
    cout << "Starting output to " << piFileName  << endl;
    // Output

    ofstream ofs (piFileName);
    ofs.precision(DIGITS + 1);
    ofs << pi << endl;

    // Time (end of writing)
    t2 = clock();
    logFile << "TIME (WRITE)  : "
         << (double)(t2 - t1) / CLOCKS_PER_SEC
         << " seconds." << endl;
    logFile << "TOTAL TIME  : "
         << (double)(t2 - t0) / CLOCKS_PER_SEC
         << " seconds." << endl;

    logFile.close();
    cout << "Completed in "
         << (double)(t2 - t0) / CLOCKS_PER_SEC
         << " seconds." << endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: " <<  argv[0] << " <numDigits>" << endl;
        return 1;
    }

    uint64_t numDigits = stoUint64(argv[1]);
    try
    {
        // Instantiation
        Chudnovsky objMain(numDigits);

        // Compute PI
        objMain.compPi();
    }
    catch (...) {
        cout << "ERROR!" << endl;
        return -1;
    }

    return 0;
}
