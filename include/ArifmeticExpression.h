#ifndef ARIFMETIC_EXPRESSION_H
#define ARIFMETIC_EXPRESSION_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>


class Lecsems {
public:

	enum class priority {
		number,
		letters,
		functions,
		constants,
		operation_add_or_sub,
		operation_mult_or_div,
		breckets
	};

	enum class check_value {
		error,
		waiting_number,
		waiting_number_or_point,
		waiting_number_after_point
	};

	enum class state {
		start,
		error,
		waiting_operator,
		waiting_brackets,
		waiting_operator_or_brackets,
		wait_numb_or_brackets,
		wait_operand_after_unarn_op,
		wait_unarn_or_number,
		waiting_closed_brackets
	};

	class constants {
	public:
		std::vector<std::pair<std::string, std::string>> m_list_of_const = { {"G", "9.81"}, {"e", "2.71"} };

		size_t size() {
			return m_list_of_const.size();
		}
		std::string valueOfConstByIndex(const size_t index) {
			return m_list_of_const[index].second;
		}
		void getAllConstants() {
			std::cout << "There are next constants:" << std::endl;
			for (size_t count = 0; count < m_list_of_const.size(); ++count) {
				std::cout << m_list_of_const[count].first << " IS " << m_list_of_const[count].second << std::endl;
			}
		}
		void addConstant(const std::string& name_of_const) {
			std::string value_of_const;
			std::cout << "Enter the value of const: ";
			std::cin >> value_of_const;
			if (!Lecsems::checkDoubleValue(value_of_const))
				throw ("ERROR: wrong value");
			else
				m_list_of_const.push_back({ name_of_const,value_of_const });
		}
	};

	std::vector <std::pair<std::string, priority>> m_data;
	constants m_constants;

	Lecsems() {}
	Lecsems(const std::string& other) {
		std::pair<std::string, priority> temp;
		for (size_t count = 0; count < other.size(); ++count) {
			temp.first = "";
			if (other[count] == '+' || other[count] == '-') {
				temp.first = other[count];
				temp.second = priority::operation_add_or_sub;
				m_data.push_back(temp);
			}
			else if (other[count] == '*' || other[count] == '/') {
				temp.first = other[count];
				temp.second = priority::operation_mult_or_div;
				m_data.push_back(temp);
			}
			else if (other[count] == '(' || other[count] == ')') {
				temp.first = other[count];
				temp.second = priority::breckets;
				m_data.push_back(temp);
			}
			else if ((other[count] >= 49 && other[count] <= 57) || other[count] == '.') { // Ó·‡·ÓÚÍ‡ ˜ËÒÎ‡ ˜ÂÂÁ ASCII
				temp.second = priority::number;
				do {
					temp.first += other[count];
					++count;
				} while (((other[count] >= 48 && other[count] <= 57) || other[count] == '.') && count < other.size());
				if (!checkDoubleValue(temp.first)) {
					throw ("ERROR: wrong number");
				}
				else {
					m_data.push_back(temp);
					--count;
				}
			}
			else {
				std::string nameOfConst;
				do {
					nameOfConst += other[count];
					++count;
				} while (((other[count] >= 65 && other[count] <= 90) || (other[count] >= 97 && other[count] <= 122)) && other[count] != '/' && other[count] != '-' && other[count] != '+' && other[count] != '*' && count < other.size());
				--count;

				for (size_t COUNT = 0; COUNT < m_constants.size(); ++COUNT) {
					if (nameOfConst == m_constants.m_list_of_const[COUNT].first) {
						temp.first = m_constants.m_list_of_const[COUNT].second;
						temp.second = priority::constants;
						break;
					}
					else if (COUNT == m_constants.size() - 1) {
						bool check_for_add_const;
						std::cout << "IS " << nameOfConst << " CONST?" << std::endl;
						std::cout << "IF YES - enter 1. IF NOT - enter 0" << std::endl;
						std::cin >> check_for_add_const;

						if (check_for_add_const == true) {
							m_constants.addConstant(nameOfConst);
							temp.first = m_constants.valueOfConstByIndex(m_constants.size() - 1);
							temp.second = priority::constants;
						}
						else {
							temp.first = nameOfConst;
							if (nameOfConst.size() == 1)
								temp.second = priority::letters;
							else temp.second = priority::functions;
						}
						break;
					}
				}
				m_data.push_back(temp);
			}
		}
	}

	Lecsems& operator=(const Lecsems& other) {
		if (this == &other)
			return *this;
		m_constants = other.m_constants;
		m_data = other.m_data;
		return *this;
	}

	static bool checkDoubleValue(const std::string& temp) {
		check_value cv = check_value::waiting_number;
		for (size_t count = 0; count <= temp.size(); ++count) {
			switch (cv)
			{
			case check_value::error:
				return false;
			case check_value::waiting_number:
				if (temp[count] == '.') cv = check_value::error;
				else cv = check_value::waiting_number_or_point;
				break;
			case check_value::waiting_number_or_point:
				if (temp[count] == '.') cv = check_value::waiting_number_after_point;
				else cv = check_value::waiting_number_or_point;
				break;
			case check_value::waiting_number_after_point:
				if (temp[count] == '.') cv = check_value::error;
				else cv = check_value::waiting_number_after_point;
				break;
			default:
				break;
			}
		}
		return true;
	}

	bool check() {
		state st = state::start;
		size_t count_of_breckets = 0;
		for (size_t count = 0; count < m_data.size(); ++count) {
			switch (st)
			{
			case Lecsems::state::start:
				if (m_data[count].first == "." || m_data[count].first == ")" || m_data[count].second == priority::operation_mult_or_div || m_data[count].second == priority::operation_add_or_sub) st = state::error;
				else if (m_data[count].second == priority::functions) st = state::waiting_brackets;
				else if (m_data[count].first == "(") {
					st = state::wait_unarn_or_number;
					++count_of_breckets;
				}
				else if (m_data[count].second == priority::letters || m_data[count].second == priority::number || m_data[count].second == priority::constants) st = state::waiting_operator_or_brackets;
				break;
			case Lecsems::state::wait_unarn_or_number:
				if (m_data[count].second == priority::operation_add_or_sub) st = state::wait_operand_after_unarn_op;
				else if (m_data[count].first == "." || m_data[count].first == ")" || m_data[count].second == priority::operation_mult_or_div || m_data[count].second == priority::operation_add_or_sub) st = state::error;
				else if (m_data[count].second == priority::functions) st = state::waiting_brackets;
				else if (m_data[count].second == priority::letters || m_data[count].second == priority::number || m_data[count].second == priority::constants) st = state::waiting_operator_or_brackets;
				break;
			case Lecsems::state::wait_operand_after_unarn_op:
				if (m_data[count].second == priority::constants || m_data[count].second == priority::letters || m_data[count].second == priority::number) {
					st = state::waiting_closed_brackets;
				}
				else st = state::error;
				break;
			case Lecsems::state::waiting_closed_brackets:
				if (m_data[count].first == ")") {
					--count_of_breckets;
					st = state::waiting_operator;
				}
				else st = state::error;
				break;
			case Lecsems::state::waiting_operator:
				if (m_data[count].second == priority::operation_add_or_sub || m_data[count].second == priority::operation_mult_or_div)
					st = state::wait_numb_or_brackets;
				else
					st = state::error;
				break;
			case Lecsems::state::wait_numb_or_brackets:
				if (m_data[count].second == priority::number || m_data[count].second == priority::letters || m_data[count].second == priority::functions) st = state::waiting_operator;
				else if (m_data[count].second == priority::breckets) {
					st = state::start;
					if (m_data[count].first == "(") ++count_of_breckets;
					else --count_of_breckets;
					if (count_of_breckets < 0) st = state::error;
				}
				else st = state::error;
				break;
			case Lecsems::state::waiting_operator_or_brackets:
				if (m_data[count].second == priority::operation_add_or_sub || m_data[count].second == priority::operation_mult_or_div) st = state::start;
				else if (m_data[count].first == ")") {
					st = state::waiting_operator_or_brackets;
					--count_of_breckets;
					if (count_of_breckets < 0) st = state::error;
				}
				else if (m_data[count].first == "(") {
					st = state::start;
					++count_of_breckets;
				}
				else st = state::error;
				break;
			case Lecsems::state::waiting_brackets:
				if (m_data[count].first == ")") {
					st = state::start;
					--count_of_breckets;
					if (count_of_breckets < 0) st = state::error;
				}
				else if (m_data[count].second == priority::operation_add_or_sub || m_data[count].second == priority::operation_mult_or_div) st = state::error;
				else if (m_data[count].first == "(") {
					st = state::start;
					++count_of_breckets;
				}
				else
					st = state::start;
				break;
			case Lecsems::state::error:
				return false;
			default:
				break;
			}
		}
		if (count_of_breckets == 0)
			return true;
		return false;
	}

	friend std::ostream& operator<<(std::ostream& out, const Lecsems& other) {
		for (size_t count = 0; count < other.m_data.size(); ++count) {
			std::pair<std::string, priority> temp = other.m_data[count];
			out << "LECSEM IS: " << temp.first << "\n";
			priority pr{};
			switch (other.m_data[count].second) {
			case (priority::number):
			case (priority::letters):
			case (priority::functions):
			case (priority::constants): out << "PRIORITY IS: 1\n"; break;

			case (priority::operation_add_or_sub): out << "PRIORITY IS: 2\n"; break;
			case (priority::operation_mult_or_div): out << "PRIORITY IS: 3\n"; break;
			case (priority::breckets): out << "PRIORITY IS: 4\n"; break;

			default: break;
			}
		}
		return out;
	}

	size_t size() {
		return m_data.size();
	}
};

class Arithmetic—alculation : private Lecsems {
private:
	Lecsems m_expression;
	std::vector<std::pair<std::string, priority>> m_postfix;
public:
	Arithmetic—alculation(const Lecsems& expression) {
		m_expression = expression;
		if (!m_expression.check())
			throw ("ERROR: wrong expression");
	}

	std::string calculating() {
		std::stack<std::pair<std::string, priority>> steck;

		for (size_t count = 0; count < m_postfix.size(); ++count) {
			if (m_postfix[count].second == priority::number || m_postfix[count].second == priority::constants) {
				steck.push(m_postfix[count]);
			}
			else if (m_postfix[count].second == priority::letters) {
				std::cout << "ENTER THE VALUE OF " << m_postfix[count].first << ": ";
				std::cin >> m_postfix[count].first;
				if (!m_expression.checkDoubleValue(m_postfix[count].first))
					throw ("ERROR: wrong number");
				m_postfix[count].second = priority::number;
				steck.push(m_postfix[count]);
			}
			else if (m_postfix[count].second == priority::functions) {
				steck.push(m_postfix[count]);
				size_t count_of_breckets = 0;
				for (size_t COUNT = count; COUNT < m_postfix.size(); ++COUNT) {
					if (m_postfix[count].first == "(") {
						++count_of_breckets;
						steck.push(m_postfix[count]);
					}
					else if (m_postfix[count].first == ")") {
						--count_of_breckets;

					}
				}
			}
			else {
				if (m_postfix[count].first == "+") {
					double leftOperand = std::stod(steck.top().first);
					steck.pop();
					double rightOperand = std::stod(steck.top().first);
					steck.pop();
					steck.push(std::make_pair(std::to_string(leftOperand + rightOperand), priority::operation_add_or_sub));
				}
				else if (m_postfix[count].first == "-") {
					double leftOperand = std::stod(steck.top().first);
					steck.pop();
					double rightOperand = std::stod(steck.top().first);
					steck.pop();
					steck.push(std::make_pair(std::to_string(leftOperand - rightOperand), priority::operation_add_or_sub));
				}
				else if (m_postfix[count].first == "*") {
					double leftOperand = std::stod(steck.top().first);
					steck.pop();
					double rightOperand = std::stod(steck.top().first);
					steck.pop();
					steck.push(std::make_pair(std::to_string(leftOperand * rightOperand), priority::operation_add_or_sub));
				}
				else if (m_postfix[count].first == "/") {
					double leftOperand = std::stod(steck.top().first);
					steck.pop();
					double rightOperand = std::stod(steck.top().first);
					steck.pop();
					steck.push(std::make_pair(std::to_string(leftOperand / rightOperand), priority::operation_add_or_sub));
				}
			}
		}
		return steck.top().first;
	}

	std::vector<std::pair<std::string, priority>> getPostfix() {
		std::stack<std::pair<std::string, priority>> stack;

		for (size_t count = 0; count < m_expression.m_data.size(); ++count) {
			if (m_expression.m_data[count].second == priority::number)
				m_postfix.push_back(std::make_pair(m_expression.m_data[count].first, m_expression.m_data[count].second));
			else if (m_expression.m_data[count].second == priority::letters) {
				//std::cout << "ENTER THE VALUE OF " << m_expression.m_data[count].first << ": ";
				//std::cin >> m_expression.m_data[count].first;
				//if (!m_expression.checkDoubleValue(m_expression.m_data[count].first))
				//	throw ("ERROR: wrong number");
				//m_expression.m_data[count].second = priority::number;
				m_postfix.push_back(std::make_pair(m_expression.m_data[count].first, m_expression.m_data[count].second));
			}
			else if (m_expression.m_data[count].second == priority::constants)
				m_postfix.push_back(std::make_pair(m_expression.m_data[count].first, m_expression.m_data[count].second));
			else if (m_expression.m_data[count].first == "(")
				stack.push(std::make_pair(m_expression.m_data[count].first, m_expression.m_data[count].second));
			else if (m_expression.m_data[count].first == ")") {
				while (stack.top().first != "(") {
					m_postfix.push_back(std::make_pair(stack.top().first, stack.top().second));
					stack.pop();
				}
				stack.pop();
			}
			else if (m_expression.m_data[count].second == priority::operation_add_or_sub) {
				if (stack.empty())
					stack.push(std::make_pair(m_expression.m_data[count].first, m_expression.m_data[count].second));
				else if (stack.top().first == "+" || stack.top().first == "-" || stack.top().first == "/" || stack.top().first == "*") {
					if ((m_expression.m_data[count].first == "-") && (m_expression.m_data[count + 1].second == priority::constants || m_expression.m_data[count + 1].second == priority::letters || m_expression.m_data[count + 1].second == priority::number)) {
						m_postfix.push_back(std::make_pair(std::to_string((1) * std::stod(m_expression.m_data[count + 1].first)),m_expression.m_data[count+1].second));
						++count;
					}
					else {
						m_postfix.push_back(stack.top());
						stack.pop();
						stack.push(std::make_pair(m_expression.m_data[count].first, m_expression.m_data[count].second));
					}
				}
				else stack.push(std::make_pair(m_expression.m_data[count].first, m_expression.m_data[count].second));
			}
			else if (m_expression.m_data[count].second == priority::operation_mult_or_div) {
				if (stack.empty())
					stack.push(std::make_pair(m_expression.m_data[count].first, m_expression.m_data[count].second));
				else if (stack.top().first == "*" || stack.top().first == "/") {
					m_postfix.push_back(stack.top());
					stack.pop();
					stack.push(std::make_pair(m_expression.m_data[count].first, m_expression.m_data[count].second));
				}
				else stack.push(std::make_pair(m_expression.m_data[count].first, m_expression.m_data[count].second));
			}
		}

		while (!stack.empty()) {
			if (stack.top().first == "(" || stack.top().first == ")")
				stack.pop();
			else {
				m_postfix.push_back(stack.top());
				stack.pop();
			}
		}
		return m_postfix;
	}

	std::string getPost() {
		std::string temp = "";
		for (size_t count = 0; count < m_postfix.size(); ++count)
			temp += m_postfix[count].first;
		return temp;
	}

	void printPostfix() {
		for (size_t count = 0; count < m_postfix.size(); ++count) {
			std::cout << m_postfix[count].first;
		}
	}

};

#endif // !ARIFMETIC_EXPRESSION_H