# Chemical Equation Autobalancer

- Greatly simplifies the process of balancing chemical formulas through the use of C++ and linear algebra
- A console GUI provided on the latest release, making running the program **simple** and **easy** for beginners
- For more advanced users, the program provides has a great variety of functions in the "linear" namespace that can be used to easily enhance any chemistry-based project or repo

## How it works (GUI)

- Enter your chemical equation like in the example below

> B2S3 + H2O ---> H3BO3 + H2S

- In a few seconds, your balanced equation will be output to the screen

> 1.000000B2S3 + 2.000000H2O ---> 6.000000H3BO3 + 3.000000H2S

## Basic Documentation

- std::vector<std::vector<long double>> rref(std::vector<std::vector<long double>> matrix)

Converts any inputted 2D square vector matrix into its reduced row echelon form which is then returned. 

- std::vector<long double> doubleToInt(std::vector<std::vector<long double>> matrix)

Gets the long double solution values in a RREF matrix to integer values (or at least fairly close). Returns the solution set of of the RREF matrix with the integer approximations.

- std::vector<std::vector<long double>> matrixInitializer(std::string input)

Creates and returns a 2D square vector matrix that fits the inputted chemical reaction in question. Each compound occupies a column, each unique element occupies a row.

- std::string stoichSolver(std::string manual_input = "")

Pieces together many of the functions to give the user their solved chemical equation as a string. If avoiding using the GUI, a chemical equation can be entered as a parameter.

