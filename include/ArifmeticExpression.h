#ifndef ARIFMETIC_EXPRESSION_H
#define ARIFMETIC_EXPRESSION_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>

/*
TASK:
	1) поддержка + - * / ( ) - DONE
	2) поддержка констант и букв - DONE
	3) поддержка функций - DONE
	4) поддержка унарного минуса
*/

class Lecsems {
private:
	friend class ArithmeticСalculation;

	static bool checkDoubleValue(const std::string& value) { // метод проверки числа на правльность
		check_value cv = check_value::waiting_number;
		for (size_t count = 0; count <= value.size(); ++count) { // проходим каждый символ строки = значениею числа
			switch (cv)
			{
			case Lecsems::check_value::error: // ошибка
				return false;
			case Lecsems::check_value::waiting_number: // ждем только число
				if (value[count] == '.') cv = Lecsems::check_value::error; // если точка, переход на ошибку
				else cv = Lecsems::check_value::waiting_number_or_point; // остальные случаи переход на ождание числа или точки
				break;
			case Lecsems::check_value::waiting_number_or_point: // ждем точку или число
				if (value[count] == '.') cv = Lecsems::check_value::waiting_number_after_point; // если точка, то переход на ожидание числа после точки
				else cv = Lecsems::check_value::waiting_number_or_point; // остальные случаи - переход на это же состояние (ождание числа или точки)
				break;
			case Lecsems::check_value::waiting_number_after_point: // ожидание числа после точки
				if (value[count] == '.') cv = Lecsems::check_value::error; // если точка, то ошибка
				else cv = Lecsems::check_value::waiting_number_after_point; // остальные случаи - переход на это же состояние (ожидание числа после точки)
				break;
			default:
				break;
			}
		}
		return true; // попадет сюда только в том случае, если строка = числу - правильная
	}

	enum class priority {
		constants,
		numbers,
		letters,
		functions,
		operations_add_or_sub,
		operations_mult_or_div,
		breackets
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
		waiting_open_brackets,
		waiting_operator_or_brackets,
		wait_numb_or_brackets,
		wait_unarn_op_or_operand_or_brackets,
		wait_operand,
		waiting_close_brackets
	};

	static class constants {
	private:
		std::vector<std::pair<std::string, double>> m_list_of_constants = { {"G", 9.81}, {"e", 2.71}, {"Pi", 3.14} }; // вектор пар из имени и значении константы
	public:
		size_t size() { // кол-во констант в векторе m_list_of_constants
			return m_list_of_constants.size();
		}
		void print() const { // вывод всех констант парами (имя, значение)
			std::cout << "LIST OF CONSTNTS: " << std::endl;
			for (size_t count = 0; count < m_list_of_constants.size(); ++count) {
				std::cout << m_list_of_constants[count].first << " = " << m_list_of_constants[count].second << std::endl;
			}
		}
		double getValue(const size_t& index) { // возвращает значение константы 
			return m_list_of_constants[index].second;
		}
		std::string getName(const size_t& index) { // возвращает имя константы
			return m_list_of_constants[index].first;
		}
		std::pair < std::string, double> get(const size_t& index) { // возвращает пару коснтанты (имя, значение)
			return m_list_of_constants[index];
		}
		void add(std::string& name, std::string& value) { // доавблаение константы
			if (checkDoubleValue((value))) // проверка константы через статический метод check на правльность
				m_list_of_constants.push_back(std::make_pair(name, std::stod(value))); // если вернулся 1, то пуш пары в вектор с константами
			else
				throw ("ERROR: wrong value of const"); // если вернулся 0, то вывод ошибки
		}
	};

	std::vector<std::pair<std::string, priority>> m_data;
	constants m_constants;

public:
	Lecsems() {}
	Lecsems(const std::string& other) {
		for (size_t count = 0; count < other.size(); ++count) { // проходим каждый символ строки other

			if (other[count] == '+' || other[count] == '-') // если + или -, то пуш в вектор (оператор + или -, приортет операции + или -)
				if (other[count] == '+')
					m_data.push_back(std::make_pair("+", priority::operations_add_or_sub));
				else m_data.push_back(std::make_pair("-", priority::operations_add_or_sub));
			else if (other[count] == '*' || other[count] == '/') // если * или /, то пуш в вектор (оператор * или /, приортет операции * или /)
				if (other[count] == '*')
					m_data.push_back(std::make_pair("*", priority::operations_mult_or_div));
				else m_data.push_back(std::make_pair("/", priority::operations_mult_or_div));
			else if (other[count] == '(' || other[count] == ')') // если ( или ), то пуш в вектор ( скобки ( или ) , приортет скобки)
				if (other[count] == '(')
					m_data.push_back(std::make_pair("(", priority::breackets));
				else m_data.push_back(std::make_pair(")", priority::breackets));
			else if ((other[count] >= 49 && other[count] <= 57) || other[count] == '.') { // если число (проверка через ASCII-таблицу) или точка. Если все ок, то запушим сразу число типа дабл, то есть сразу поддержка осуществляется вещественных чисел
				std::string temp; // вспомогательная строка
				do {
					temp += other[count]; // добавляем число или точку
					++count; // переходим сразу на следующий символ
				} while (((other[count] >= 48 && other[count] <= 57) || other[count] == '.') && count < other.size()); // делаем, пока попадается либо число, либо точка, либо count выйдет за размеры строки
				if (!checkDoubleValue(temp)) // сюда попадаем с сформаировавщейся строкой числа (она может быть неправльной, поэтому идем проверять число на правильность)
					throw ("ERROR: wrong number"); // если число "плохое", то ERROR
				else { // если число норм, то пушим пару в вектор (число в виде строки, и приортиет числа)
					m_data.push_back(std::make_pair(temp, priority::numbers));
					--count; // так как мы сделали один лишний ++count, надо сделать --count
				}
			}
			else { // раз это не число, не оператор +,-,/,*,(,) то это либо название константы, либо какая-то буква, либо функция
				std::string name; // введем вспомогательную строку 
				do {
					name += other[count];
					++count;
				} while (((other[count] >= 65 && other[count] <= 90) || (other[count] >= 97 && other[count] <= 122)) && count < other.size()); // делаем, пока это буква (большая или маленькая) или count выйдет за размеры строки
				--count; // лишний ++, следовательно делаем --

				// проверяем, константа ли наша строка name
				bool exitTemp = false; // вспомгательный "костыль", чтобы выйти из внешнего цикла, если константа найдется в векторе m_constants.m_list_of_constants
				for (size_t COUNT = 0; COUNT < m_constants.size(); ++COUNT) {
					if (name == m_constants.getName(COUNT)) {
						m_data.push_back(std::make_pair(std::to_string(m_constants.getValue(COUNT)), priority::constants)); // пушим пару (значние константы, приоритет константы). !обязательно ли констнаты???
						exitTemp = true;
						break;
					}
				}
				if (exitTemp) // если консанта нашалась, то с помощью "костыля" переходим на следующую итерацию цикла
					continue;
				// так, раз это не конснтанта значит все еще либо функция, либо буква, либо константа, которой нет в векторе констант
				// поэтому спросим пользователя, добавить ли новую константу

				bool check_for_add_const; // булева переменная для проверки добавления константы
				std::cout << "IS " << name << " CONST?" << std::endl;
				std::cout << "IF YES - enter 1. IF NOT - enter 0" << std::endl;
				std::cin >> check_for_add_const;

				if (check_for_add_const == true) { // если 1, то добавляем константу
					std::cout << "ENTER THE VALUE OF " << name << ": ";
					std::string value_of_const; // переменная для ввода значения константы
					std::cin >> value_of_const;
					m_constants.add(name, value_of_const); // добавляем константу, там заодно проверяется константа на правльность, хотя можно все равно сломать, если ввести допустим не цифру а какой-нибудь оператор(надо будет это продумать)
					m_data.push_back(std::make_pair(value_of_const, priority::constants));
				}
				else { // если 0, то значит 100% не константа
					if (name.size() == 1) // глупая проверка, но РАБОЧАЯ xD если name из одного символа, то 100% буква, ибо нет функций из одной буквы (я не знаю такой)
						m_data.push_back(std::make_pair(name, priority::letters));
					else m_data.push_back(std::make_pair(name, priority::functions));
				}
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

	bool check() { // функция на проверку лексем
		state status = state::start;
		size_t count_of_breckets = 0;
		for (size_t count = 0; count < m_data.size(); ++count) { // проходим вектор, состоящий из пар (значение, приортет)
			/*
			Лексема у нас может быть следующих типов:
				1) Скобка
				2) Оператор + или -
				3) Оператор * или /
				4) Буква
				5) Константа
				6) Цифра
				7) Функция
			*/
			switch (status)
			{
			case Lecsems::state::start: // старт, первая лексема, первый элемент в векторе m_data

				/*
					Тут может быть:
						1) Скобка, только ( -> ждем либо унарный опертор - или буква, цифра, функция, константа, либо скобка (
								если ) -> сразу в error
						2) Если оператор * или / -> сразу в error
						3) Если оперотр + или - -> сразу в error
						4) Буква, константа или цифра - все ок
						5) Функция -> ждем скобку (
				*/

				if (m_data[count].first == "(") { // после ( может быть все, что на старте
					++count_of_breckets;
					status = Lecsems::state::wait_unarn_op_or_operand_or_brackets;
				}
				else if (m_data[count].second == priority::constants || m_data[count].second == priority::letters || m_data[count].second == priority::numbers) // после числа, константы или буква, может идти только опертор или закрывающаяся скобка
					status = Lecsems::state::waiting_operator_or_brackets;
				else if (m_data[count].second == priority::functions) // после функции мы ждем только открывающуюся скобку
					status = Lecsems::state::waiting_open_brackets;
				else if (m_data[count].first == ")") // если скобка ), то еррор
					status = Lecsems::state::error;
				else // остальные случаи = еррор
					status = Lecsems::state::error;
				break;

			case Lecsems::state::waiting_operator:
				/*
					Тут может быть:
						1) оператор -> ждем число, константу, букву
						2) скобка -> ошибка
						3) ... -> ошибка
				*/
				if (m_data[count].second == priority::operations_add_or_sub || m_data[count].second == priority::operations_mult_or_div)
					status = Lecsems::state::start;
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::waiting_open_brackets:

				/*
					Тут может быть:
						1) открывающаяся скобка, то переходим на старт
						2) остальные случаи -> еррор
				*/

				if (m_data[count].first == "(") {
					++count_of_breckets;
					status = Lecsems::state::start;
				}
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::waiting_operator_or_brackets:

				/*
					Тут может быть:
						1) скобка ) -> ждем либо оператор, либо скобку )
						2) скобка (
						3) оператор
						4) число, константа, буква
				*/

				if (m_data[count].first == ")") {
					--count_of_breckets;
					status = Lecsems::state::waiting_operator_or_brackets;
				}
				else if (m_data[count].second == priority::operations_add_or_sub || m_data[count].second == priority::operations_mult_or_div)
					status = Lecsems::state::start;
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::wait_numb_or_brackets:
				/*
					Тут могла быть ваша реклама
				*/
				break;

			case Lecsems::state::wait_unarn_op_or_operand_or_brackets:

				/*
					Тут может быть:
						1) - = унарный минус, после него может быть функция или число
						2) число, то ждем оператор или скобку открывающуюся
						3) функция, то ждем только открывающуся скобку
						4) (, то переходим на старт
						5) остальные случаи -> еррор
				*/

				if (m_data[count].first == "-")
					status = Lecsems::state::wait_operand;
				else if (m_data[count].second == priority::numbers || m_data[count].second == priority::constants || m_data[count].second == priority::letters)
					status = Lecsems::state::waiting_operator_or_brackets;
				else if (m_data[count].second == priority::functions)
					status == Lecsems::state::waiting_open_brackets;
				else if (m_data[count].first == "(") {
					status = Lecsems::state::start;
					++count_of_breckets;
				}
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::wait_operand:

				/*
					Тут может быть:
						1) число, то ждем только закрывающуюся скобку
						2) функция, то ждем тольк открывающуюся скобку
						3) остальные случаи -> еррор
				*/

				if (m_data[count].second == priority::numbers || m_data[count].second == priority::constants || m_data[count].second == priority::letters) status = Lecsems::state::waiting_close_brackets;
				else if (m_data[count].second == priority::functions) status = Lecsems::state::waiting_open_brackets;
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::waiting_close_brackets:

				/*
					Тут может быть:
						1) закрывающаяся скобка, то ждем только оператор или закрывающуюся скобку
						2) остальные случаи -> еррор
				*/

				if (m_data[count].first == ")") {
					status = Lecsems::state::waiting_operator_or_brackets;
					--count_of_breckets;
				}
				else status = Lecsems::state::error;
				break;

			case Lecsems::state::error:
				return false;
			default:
				return false;
			}
		}
		return !count_of_breckets; // если число скобок == 0, то возвращам true, в остльных случаях возвращаем false
	}
	friend std::ostream& operator<<(std::ostream& out, const Lecsems& other) {
		for (size_t count = 0; count < other.m_data.size(); ++count) {
			out << "LECSEM IS " << other.m_data[count].first << "; PRIORITET IS ";
			switch (other.m_data[count].second) {
			case (priority::numbers):
			case (priority::letters):
			case (priority::constants): out << "1\n"; break;
			case (priority::functions): out << "FUNC\n"; break;

			case (priority::operations_add_or_sub): out << "2\n"; break;
			case (priority::operations_mult_or_div): out << "3\n"; break;
			case (priority::breackets): out << "4\n"; break;

			default: break;
			}
		}
		return out;
	}
};

class ArithmeticСalculation {
private:
	Lecsems m_expression;
	std::vector<std::pair<std::string, Lecsems::priority>> m_data;
	std::vector<std::pair<std::string, Lecsems::priority>> m_postfix;
public:
	ArithmeticСalculation(const Lecsems& expression) {
		m_expression = expression;
		if (!m_expression.check())
			throw ("ERROR: wrong expression");
	}

	void convertExpression() { // вариант, как преобразовать выражение с унарным минусом в нормальное выражение (чтобы оно работало)
		for (size_t count = 0; count < m_expression.m_data.size(); ++count) {
			if (m_expression.m_data[count].first == "(" && m_expression.m_data[count + 1].first == "-" && m_expression.m_data[count + 2].second == Lecsems::priority::numbers && m_expression.m_data[count + 3].first == ")") {
				std::string temp = "-";
				temp += m_expression.m_data[count + 2].first;
				m_data.push_back(std::make_pair(temp, Lecsems::priority::numbers));
				count += 3;
			}
			else if (m_expression.m_data[count].first == "(" && m_expression.m_data[count + 1].first == "-" && m_expression.m_data[count + 2].second == Lecsems::priority::constants && m_expression.m_data[count + 3].first == ")") {
				m_data.push_back(std::make_pair(std::to_string(std::stod(m_expression.m_data[count + 2].first) * (-1)), Lecsems::priority::numbers));
				count += 3;
			}
			else if (m_expression.m_data[count].first == "(" && m_expression.m_data[count + 1].first == "-" && m_expression.m_data[count + 2].second == Lecsems::priority::letters && m_expression.m_data[count + 3].first == ")") {
				std::cout << "ENTER THE VALUE OF " << m_expression.m_data[count + 2].first << ": ";
				std::string value;
				std::cin >> value;
				if (Lecsems::checkDoubleValue(value))
					m_data.push_back(std::make_pair(std::to_string(std::stod(value) * (-1)), Lecsems::priority::numbers));
				else throw ("ERROR: wrong value");
				count += 3;
			}
			else m_data.push_back(m_expression.m_data[count]);
		}
	}

	std::vector<std::pair<std::string, Lecsems::priority>> getPostfix() {
		std::stack<std::pair<std::string, Lecsems::priority>> stack;

		for (size_t count = 0; count < m_data.size(); ++count) {
			if (m_data[count].second == Lecsems::priority::numbers)
				m_postfix.push_back(m_data[count]);
			else if (m_data[count].second == Lecsems::priority::letters)
				m_postfix.push_back(m_data[count]);
			else if (m_data[count].second == Lecsems::priority::constants)
				m_postfix.push_back(m_data[count]);
			else if (m_data[count].second == Lecsems::priority::functions)
				m_postfix.push_back(m_data[count]);
			else if (m_data[count].first == "(")
				stack.push(m_data[count]);
			else if (m_data[count].first == ")") {
				while (stack.top().first != "(") {
					m_postfix.push_back(stack.top());
					stack.pop();
				}
				stack.pop();
			}
			else if (m_data[count].second == Lecsems::priority::operations_add_or_sub || m_data[count].second == Lecsems::priority::operations_mult_or_div) {
				while (!stack.empty() && ((m_data[count].second == Lecsems::priority::operations_add_or_sub && (stack.top().second == Lecsems::priority::operations_add_or_sub || stack.top().second == Lecsems::priority::operations_mult_or_div)) || (m_data[count].second == Lecsems::priority::operations_mult_or_div && stack.top().second == Lecsems::priority::operations_mult_or_div))) {
					m_postfix.push_back(stack.top());
					stack.pop();
				}
				stack.push(m_data[count]);
			}
		}
		while (!stack.empty()) {
			m_postfix.push_back(stack.top());
			stack.pop();
		}
		/*
		Программа:	15 7 1 1 + - / 3 * 2 1 1 + + 15 * 7 200 1 + - / 3 * - 2 1 1 + + 15 7 1 1 + - / 3 * 2 1 1 + + - 15 7 1 1 + - / 3 * + 2 1 1 + + - * -
		Сайт:		15 7 1 1 + - / 3 * 2 1 1 + + 15 * 7 200 1 + - / 3 * - 2 1 1 + + 15 7 1 1 + - / 3 * 2 1 1 + + - 15 7 1 1 + - / 3 * + 2 1 1 + + - * -
		*/
		return m_postfix;
	}

	std::string calculating() {
		std::stack<std::pair<std::string, Lecsems::priority>> steck;

		size_t temp = 1;

		for (size_t count = 0; count < m_postfix.size(); ++count) {
			if (m_postfix[count].second == Lecsems::priority::numbers || m_postfix[count].second == Lecsems::priority::constants) {
				steck.push(m_postfix[count]);
			}
			else if (m_postfix[count].second == Lecsems::priority::letters) {
				std::cout << "ENTER THE VALUE OF " << m_postfix[count].first << ": ";
				std::cin >> m_postfix[count].first;
				if (!m_expression.checkDoubleValue(m_postfix[count].first))
					throw ("ERROR: wrong number");
				m_postfix[count].second = Lecsems::priority::numbers;
				steck.push(m_postfix[count]);
			}
			else if (m_postfix[count].second == Lecsems::priority::functions)
				steck.push(m_postfix[count]);
			else if (m_postfix[count].second == Lecsems::priority::operations_add_or_sub || m_postfix[count].second == Lecsems::priority::operations_mult_or_div) {
				if (m_postfix[count].first == "+") {
					std::string rightOperand = steck.top().first;
					steck.pop();
					double leftOperand = std::stod(steck.top().first);
					steck.pop();
					//std::cout << temp++ << ") " << leftOperand << "+" << rightOperand << "=" << leftOperand + std::stod(rightOperand) << std::endl;
					steck.push(std::make_pair(std::to_string(leftOperand + std::stod(rightOperand)), Lecsems::priority::operations_add_or_sub));
				}
				else if (m_postfix[count].first == "-") {
					std::string rightOperand = steck.top().first;
					steck.pop();
					double leftOperand = std::stod(steck.top().first);
					steck.pop();
					//std::cout << temp++ << ") " << leftOperand << "-" << rightOperand << "=" << leftOperand - std::stod(rightOperand) << std::endl;
					steck.push(std::make_pair(std::to_string(leftOperand - std::stod(rightOperand)), Lecsems::priority::operations_add_or_sub));
				}
				else if (m_postfix[count].first == "*") {
					std::string rightOperand = steck.top().first;
					steck.pop();
					double leftOperand = std::stod(steck.top().first);
					steck.pop();
					//std::cout << temp++ << ") " << leftOperand << "*" << rightOperand << "=" << leftOperand * std::stod(rightOperand) << std::endl;
					steck.push(std::make_pair(std::to_string(leftOperand * std::stod(rightOperand)), Lecsems::priority::operations_mult_or_div));
				}
				else if (m_postfix[count].first == "/") {
					std::string rightOperand = steck.top().first;
					steck.pop();
					double leftOperand = std::stod(steck.top().first);
					steck.pop();
					//std::cout << temp++ << ") " << leftOperand << "/" << rightOperand << "=" << leftOperand / std::stod(rightOperand) << std::endl;
					steck.push(std::make_pair(std::to_string(leftOperand / std::stod(rightOperand)), Lecsems::priority::operations_mult_or_div));
				}
			}
		}
		return steck.top().first;
	}
	void printPostfix() {
		for (size_t count = 0; count < m_postfix.size(); ++count) {
			std::cout << m_postfix[count].first << " ";
		}
	}
	std::string get_postfix() {
		std::string postfix;
		for (size_t count = 0; count < m_postfix.size(); ++count) {
			postfix += m_postfix[count].first;
			postfix += " ";
		}
		std::cout << postfix;
		return postfix;
	}
};

#endif // !ARIFMETIC_EXPRESSION_H