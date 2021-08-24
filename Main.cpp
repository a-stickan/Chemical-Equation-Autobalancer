#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <ctype.h>
#include <list>

namespace linear
{
	std::vector<std::vector<long double>> rref(std::vector<std::vector<long double>> matrix)
	{
		std::cout << "\n\nRREF Start\n\n";

		bool mult_flag = false;
		long double pivot_multiplier = 0;
		long double other_multiplier = 0;

		int pivot_indicator = 0;
		int pivot_row = 0;
		int no_pivot_flag = true;
		std::set<int> rows_with_pivots;
		//basing off pivot columns makes more sense, rows can be out of order and mess everything up
		while (pivot_indicator < matrix[0].size())
		{
			for (int x = 0; x < matrix.size(); ++x)
			{
				if (matrix[x][pivot_indicator] != 0 && rows_with_pivots.find(x) == rows_with_pivots.end())
				{
					no_pivot_flag = false;
					rows_with_pivots.insert(x);
					pivot_row = x;
					break;
				}
			}
			
			if (!no_pivot_flag)
			{
				for (int r = 0; r < matrix.size(); ++r)
				{
					if (matrix[r][pivot_indicator] != 0)
					{
						pivot_multiplier = 1 / matrix[pivot_row][pivot_indicator];
						other_multiplier = matrix[r][pivot_indicator] / matrix[pivot_row][pivot_indicator];

						for (int c = pivot_indicator; c < matrix[0].size(); ++c)
						{
							if (r == pivot_row)
							{
								matrix[r][c] *= pivot_multiplier;
							}
							else
							{
								matrix[r][c] -= matrix[pivot_row][c] * other_multiplier;
								std::cout << matrix[pivot_row][c] * other_multiplier << "    ";
							}
						}
						std::cout << "\n\n";
					}
				}
			}

			for (auto& x : matrix)
			{
				for (auto& y : x)
				{
					std::cout << y << "   ";
				}
				std::cout << "\n";
			}
			std::cout << "\n\n\n";

			pivot_indicator++;
			no_pivot_flag = true;
		}
		return matrix;
	}


	std::vector<long double> doubleToInt(std::vector<std::vector<long double>> matrix) //this is awful code, but might just work
	{
		std::cout << "This is now the doubleToInt\n\n\n";
		
		long double smallest = INT_MAX;
		std::vector<long double> solutions;
		long double entry = 0;

		for (int i = 0; i < matrix.size(); ++i)
		{
			entry = abs(matrix[i][matrix.size()-1]);
			if (entry == 0)
			{
				entry = 1;
			}
			std::cout << entry << "   ";
			solutions.push_back(entry);
		}
		std::cout << "\n";
		int x = 100;
		int closeness_counter = 0;

		while (x > 0)
		{
			for (int i = 0; i < solutions.size(); ++i)
			{
				if (solutions.at(i) != 0 && abs(solutions[i]) < smallest)
				{
					smallest = abs(solutions[i]);
				}
			}
			std::cout << smallest << "\n";

			for (int i = 0; i < solutions.size(); ++i)
			{
				solutions.at(i) /= smallest;
				std::cout << solutions.at(i) << "   ";
				if (solutions.at(i) * 1.0005 >= (int)solutions.at(i) && solutions.at(i) * 0.9995 <= (int)solutions.at(i))
				{
					closeness_counter++;
				}
			}

			if (closeness_counter = matrix.size() - 1)
			{
				break;
			}
			std::cout << "\n\n";
			smallest = INT_MAX;
			closeness_counter = 0;
			x--;
		}
		return solutions;
	}


	//creates an appropriately sized matrix for stoichiometery problems
	std::vector<std::vector<long double>> matrixInitializer(std::string input)
	{
		char char_at_index;
		std::string string_holder = "";
		int matrix_rows = 0;
		int matrix_cols = 1;
		std::set<std::string> elements;

		for (int x = 0; x < input.size(); ++x)
		{
			char_at_index = input.at(x);
			if (isalpha(char_at_index))
			{
				if (x < input.size() - 1 && isalpha(input.at(x + 1)) && !isupper(input.at(x + 1)))
				{
					string_holder += char_at_index;
				}
				else
				{
					string_holder += char_at_index;
					if (elements.find(string_holder) == elements.end())
					{
						elements.insert(string_holder);
						std::cout << string_holder << " added\n";
						matrix_rows++;
					}
					string_holder = "";
				}
			}
			else if (char_at_index == '>' || char_at_index == '+')
			{
				matrix_cols++;
			}
		}

		std::vector<std::vector<long double>> matrix(matrix_rows, std::vector<long double>(matrix_cols, 0));

		return matrix;
	}



	void stoichSolver(std::string manual_input = "")
	{
		std::string input;
		if (manual_input == "")
		{
			std::cout << "Please enter your chemical equation in the following format.\n1. Reactants first, products last\n2. Do NOT include charges for anions/cations!\n3. Put an addition sign \"+\" between seperate reactants/products\n3. Make an arrow of any size with \"-\" and \">\" between the reactants and products 2H2 + O2 ---> 2H2O\n\n";
			std::cout << "Example:\n\t\t2H2 + O2 --> 2H2O\n\n\n";
			std::cin >> input;
		}
		else
		{
			input = manual_input;
		}



		/*
		string = element name associated with a row
		int = actual row index of the element

		Implementation:
		1. Use string with the map.find feature to locate the row in question on the 2D array "matrix"
		2. Use the cols_counter (works through ++ with each new compound) to locate the correct column
		2. matrix[map.find][cols_counter] should allow input in the correct cell
		4. side_multiplier takes care of the -1 side logic
		*/
		std::map<std::string, int> elementList;
		std::map<std::string, int> ::iterator itr;

		char char_at_index;
		std::string string_converter = "";
		double side_multiplier = 1;

		std::string string_holder = "";
		std::string num_holder = "";

		int cols_counter = 0;
		int rows_counter = 0;

		int element_total = 0;

		int i_temp;

		std::vector<long double> temporary;

		std::vector<std::vector<long double>> matrix = matrixInitializer(input);

		//start of the loop
		for (int i = 0; i < input.size(); ++i)
		{
			char_at_index = input.at(i);

			if (isalpha(char_at_index))
			{
				if (i < input.size() - 1 && isalpha(input.at(i + 1)) && !isupper(input.at(i + 1))) //other similar issues (checking index instead of char) probably exist elsewhere
				{
					string_holder += char_at_index;
				}
				else
				{
					string_holder += char_at_index;

					std::cout << "Element found, string_holder = " << string_holder << "\n";

					i_temp = i;
					while (i_temp < input.size() - 1)
					{
						if (isdigit(input.at(i_temp + 1)))
						{
							num_holder = input.at(i_temp + 1);
							element_total += std::stoi(num_holder);
							num_holder = "";

							std::cout << "Isdigit was triggered, Element_total = " << element_total << "\n";
							i++; //the next char is known
						}
						else
						{
							break;
						}
						i_temp++;
					}
					if (element_total == 0) { std::cout << "Case of no numbers\n"; element_total = 1; } //case of no numbers

					itr = elementList.find(string_holder);
					if (itr != elementList.end())
					{
						std::cout << "Existing element = " << string_holder << "\n\n";

						matrix[elementList.at(string_holder)][cols_counter] += element_total * side_multiplier; //already initialized
					}
					else //new string_holder, map gets new entry
					{
						std::cout << "Unique element = " << string_holder << "\n\n";

						elementList.insert(std::pair<std::string, int>(string_holder, rows_counter));
						matrix[elementList.at(string_holder)][cols_counter] = element_total * side_multiplier;
						rows_counter++; //number of elements present has increased
					}
					string_holder = "";
					element_total = 0;
				}
			}

			else //enforces some rules inherent to stoichiometery problems
			{
				if (char_at_index == '>')
				{
					cols_counter++;
					side_multiplier = -1;
				}
				else if (char_at_index == '+')
				{
					cols_counter++;
					side_multiplier /= abs(side_multiplier);
					std::cout << "Side multiplier reset!\n\n";
				}
				else if (isdigit(char_at_index))
				{
					i_temp = i;
					while (i_temp < input.size() - 1)
					{
						if (isdigit(input.at(i_temp)))
						{
							num_holder = input.at(i_temp);
							element_total += std::stoi(num_holder);

							std::cout << "Next digit (" << num_holder << ") num, element_total = " << element_total << "\n";

							num_holder = "";
						}
						else
						{
							std::cout << "Next digit (" << input.at(i_temp) << ") not a num, element_total = " << element_total << "\n";
							i = i_temp - 1;
							break;
						}
						i_temp++;
					}
					side_multiplier *= element_total;

					std::cout << "Side multiplier = " << side_multiplier << "\n\n";

					element_total = 0;
				}
			}
		}

		for (auto& x : matrix)
		{
			for (auto& y : x)
			{
				std::cout << y << "   ";
			}
			std::cout << "\n";
		}
		std::cout << "\n\n\n";

		std::vector<long double> solutions;
		matrix = rref(matrix);
		solutions = doubleToInt(matrix);
		bool number_placement_flag = false;
		int solutions_counter = 0;

		std::cout << "\n\n\n";

		for (int i = 0; i < input.size() - 1; ++i)
		{
			if (i == 0)
			{
				input = std::to_string(solutions.at(solutions_counter)) + input;
				solutions_counter++;
			}
			
			if (isalpha(input.at(i + 1)) && number_placement_flag)
			{
				
				input = input.substr(0, i) + " " + std::to_string(solutions.at(solutions_counter)) + input.substr(i + 1, input.size());
				solutions_counter++;
			}
			else if (input.at(i + 1) == '+' || input.at(i + 1) == ' ')
			{
				number_placement_flag = true;
			}
			else
			{
				number_placement_flag = false;
			}
		}

		std::cout << input;
	}
}


	




int main()
{
	linear::stoichSolver("B2S3 + H2O ---> H3BO3 + H2S");
	//linear::rref(matrix, true);
	//std::vector<std::vector<double>> answer = linear::matrixInitializer("H2 + SO4 --> H2SO4");

}

//works questionably well
/*
int leading_row = 0;
		while (leading_row < matrix.size())
		{
			for (int r = 0; r < matrix.size(); ++r)
			{

				if (matrix[leading_row][pivot_indicator] == 0) //case of a free variable throwing off the balance, pivot not present
				{
					pivot_indicator--;
					break;
				}
				if (matrix[r][pivot_indicator] == 0)
				{
					//have a condition here for this erroneous situation
				}

				pivot_multiplier = 1/matrix[leading_row][pivot_indicator];
				other_multiplier = matrix[r][pivot_indicator] / matrix[leading_row][pivot_indicator];

				for (int c = 0; c < matrix[0].size(); ++c)
				{
					if (r == leading_row)
					{
						matrix[r][c] *= pivot_multiplier;
					}
					else
					{
						matrix[r][c] -= matrix[leading_row][c] * other_multiplier;
					}
				}
			}

			for (auto& x : matrix)
			{
				for (auto& y : x)
				{
					std::cout << y << "   ";
				}
				std::cout << "\n";
			}
			std::cout << "\n\n\n";

			leading_row++;
			pivot_indicator++;
		}
		return matrix;
	}
*/