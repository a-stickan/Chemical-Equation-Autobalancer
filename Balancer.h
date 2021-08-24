#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>

namespace linear
{
	inline std::vector<std::vector<long double>> rref(std::vector<std::vector<long double>> matrix)
	{
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
							}
						}
					}
				}
			}
			pivot_indicator++;
			no_pivot_flag = true;
		}
		return matrix;
	}


	inline std::vector<long double> doubleToInt(std::vector<std::vector<long double>> matrix)
	{
		long double smallest = INT_MAX;
		std::vector<long double> solutions;
		long double entry = 0;

		for (int i = 0; i < matrix.size(); ++i)
		{

			entry = abs(matrix[i][matrix[0].size() - 1]); //this has acted up in the past for reasons still unknown

			if (entry == 0)
			{
				entry = 1;
			}

			solutions.push_back(entry);
		}

		solutions.push_back(1); //this fills in the gaps for any compounds not directly associated with an answer
		int x = 100; //bumping this number up will make the program take longer to run, but will provide less decimal-dependent answers
		int closeness_counter = 0;

		while (x > 0)
		{
			for (int i = 0; i < solutions.size(); ++i)
			{
				if (solutions.at(i) != 0 && abs(solutions[i]) < smallest) //bugtest, having 1 always be the smallest would make this all be pointless
				{
					smallest = abs(solutions[i]);
				}
			}
			for (int i = 0; i < solutions.size(); ++i)
			{
				solutions.at(i) /= smallest;
				if (solutions.at(i) * 1.0005 >= (int)solutions.at(i) && solutions.at(i) * 0.9995 <= (int)solutions.at(i))
				{
					closeness_counter++;
				}
			}

			if (closeness_counter = matrix.size() - 1) //loop is terminated should all values be "close enough" to ints
			{
				break;
			}
			smallest = INT_MAX;
			closeness_counter = 0;
			x--;
		}
		return solutions;
	}


	//creates an appropriately sized matrix for stoichiometery problems
	inline std::vector<std::vector<long double>> matrixInitializer(std::string input)
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


	//the basis of the program, branches into various subfunctions seen above
	inline std::string stoichSolver(std::string manual_input = "")
	{
		std::string input;
		if (manual_input == "")
		{
			std::cout << "Please enter your chemical equation in the following format.\n1. Reactants first, products last\n2. Do NOT include charges for anions/cations!\n";
			std::cout << "3. Put an addition sign \"+\" between seperate reactants/products\n4. Make an arrow of any size with \"-\" and \">\" between the reactants and products 2H2 + O2 ---> 2H2O\n";
			std::cout << "5. Do NOT include parentheses in your answer! Write Mg(OH)2 like MgO2H2 instead.\n\n";
			std::cout << "Example:\n\t\tH2 + O2 --> H2O\n\n\n";
			std::getline(std::cin, input);
		}
		else
		{
			input = manual_input;
		}

		//end of GUI stuff

		std::map<std::string, int> elementList;
		std::map<std::string, int> ::iterator itr;

		char char_at_index;
		double side_multiplier = 1;

		std::string string_holder = "";
		std::string num_holder = "";

		int cols_counter = 0;
		int rows_counter = 0;
		int element_total = 0;

		int i_temp;
		std::vector<std::vector<long double>> matrix = matrixInitializer(input);

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
					i_temp = i;
					while (i_temp < input.size() - 1)
					{
						if (isdigit(input.at(i_temp + 1)))
						{
							num_holder = input.at(i_temp + 1);
							element_total += std::stoi(num_holder);
							num_holder = "";
							i++; //the next char is known
						}
						else
						{
							break;
						}
						i_temp++;
					}
					if (element_total == 0) { element_total = 1; } //case of no numbers

					itr = elementList.find(string_holder); //adding to existing map
					if (itr != elementList.end())
					{
						matrix[elementList.at(string_holder)][cols_counter] += element_total * side_multiplier; //already initialized
					}
					else //new string_holder, map gets new entry
					{
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
							num_holder = "";
						}
						else
						{
							i = i_temp - 1;
							break;
						}
						i_temp++;
					}
					side_multiplier *= element_total;
					element_total = 0;
				}
			}
		}

		std::vector<long double> solutions;

		matrix = rref(matrix); //row reduces the matrix
		solutions = doubleToInt(matrix); //interpretation of the matrix
		bool number_placement_flag = false;
		int solutions_counter = 0;

		for (int i = 0; i < input.size() - 1; ++i)
		{
			if (i == 0)
			{
				input = std::to_string(solutions.at(solutions_counter)) + input;
				solutions_counter++;
			}



			if (isalpha(input.at(i + 1)) && number_placement_flag)
			{
				if (solutions_counter >= solutions.size() - 1) //out of pivot variables, onto the free ones
				{
					input = input.substr(0, i) + " " + std::to_string(solutions.at(solutions.size() - 1)) + input.substr(i + 1);
				}
				else if (solutions.at(solutions_counter) != 1)
				{
					input = input.substr(0, i) + " " + std::to_string(solutions.at(solutions_counter)) + input.substr(i + 1);
				}
				number_placement_flag = false;
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
		return input;
	}
}