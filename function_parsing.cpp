#include "function_parsing.h"

#define FUNCTION_PARSE_ERROR throw std::invalid_argument("fail to parse");

namespace fp
{

	//class defination
	//independent_variable
	independent_variable::independent_variable(std::string iv) :_iv(iv)
	{
	}

	std::string independent_variable::str()
	{
		return _iv;
	}

	double independent_variable::value(double x)
	{
		return x;
	}

	base_function_ptr independent_variable::derivative()
	{
		return std::make_shared<constant_function>(1);
	}

	//constant_function
	constant_function::constant_function(double c) :_c(c)
	{
	}

	std::string constant_function::str()
	{
		auto str = std::to_string(_c);
		while ((!str.empty()) && str.at(str.size() - 1) == '0')
		{
			str.pop_back();
		}
		if (!str.empty() && str.at(str.size() - 1) == '.')
		{
			str.pop_back();
		}
		return str;
	}

	double constant_function::value(double x)
	{
		return _c;
	}

	base_function_ptr constant_function::derivative()
	{
		return std::make_shared<constant_function>(0);
	}

	//add_function
	add_function::add_function(base_function_ptr lhs, base_function_ptr rhs) :_lhs(lhs), _rhs(rhs)
	{
	}

	std::string add_function::str()
	{
		return "(" + _lhs->str() + "+" + _rhs->str() + ")";
	}

	double add_function::value(double x)
	{
		try
		{
			return _lhs->value(x) + _rhs->value(x);
		}
		catch (...)
		{
			throw;
		}
	}

	base_function_ptr add_function::derivative()
	{
		return std::make_shared<add_function>(_lhs->derivative(), _rhs->derivative());
	}

	//minus_function
	minus_function::minus_function(base_function_ptr lhs, base_function_ptr rhs) :_lhs(lhs), _rhs(rhs)
	{
	}

	std::string minus_function::str()
	{
		return "(" + _lhs->str() + "-" + _rhs->str() + ")";
	}

	double minus_function::value(double x)
	{
		try
		{
			return _lhs->value(x) - _rhs->value(x);
		}
		catch (...)
		{
			throw;
		}
	}

	base_function_ptr minus_function::derivative()
	{
		return std::make_shared<minus_function>(_lhs->derivative(), _rhs->derivative());
	}

	//multiply_function
	multiply_function::multiply_function(base_function_ptr lhs, base_function_ptr rhs) :_lhs(lhs), _rhs(rhs)
	{
	}

	std::string multiply_function::str()
	{
		return "(" + _lhs->str() + "*" + _rhs->str() + ")";
	}

	double multiply_function::value(double x)
	{
		try
		{
			return _lhs->value(x) * _rhs->value(x);
		}
		catch (...)
		{
			throw;
		}
	}

	base_function_ptr multiply_function::derivative()
	{
		return std::make_shared<add_function>(std::make_shared<multiply_function>(_lhs->derivative(), _rhs), std::make_shared<multiply_function>(_lhs, _rhs->derivative()));
	}

	//divide_function
	divide_function::divide_function(base_function_ptr lhs, base_function_ptr rhs) :_lhs(lhs), _rhs(rhs)
	{
	}

	std::string divide_function::str()
	{
		return "(" + _lhs->str() + "/" + _rhs->str() + ")";
	}

	double divide_function::value(double x)
	{
		try
		{
			return  _lhs->value(x) / _rhs->value(x);
		}
		catch (...)
		{
			throw;
		}
	}

	base_function_ptr divide_function::derivative()
	{
		return std::make_shared<divide_function>(std::make_shared<minus_function>(std::make_shared<multiply_function>(_lhs->derivative(), _rhs), std::make_shared<multiply_function>(_lhs, _rhs->derivative())), std::make_shared<multiply_function>(_rhs, _rhs));
	}

	//power_function
	power_function::power_function(base_function_ptr lhs, base_function_ptr rhs) :_lhs(lhs), _rhs(rhs)
	{
	}

	std::string power_function::str()
	{
		return "(" + _lhs->str() + "^" + _rhs->str() + ")";
	}

	double power_function::value(double x)
	{
		try
		{
			return pow(_lhs->value(x), _rhs->value(x));
		}
		catch (...)
		{
			throw;
		}

	}

	base_function_ptr power_function::derivative()
	{
		return std::make_shared<multiply_function>(std::make_shared<power_function>(_lhs, _rhs), std::make_shared<add_function>(std::make_shared<multiply_function>(_rhs->derivative(), std::make_shared<ln_function>(_lhs)), std::make_shared<multiply_function>(_rhs, std::make_shared<ln_function>(_lhs)->derivative())));
	}

	//ln_function
	ln_function::ln_function(base_function_ptr arg) :_arg(arg)
	{
	}

	std::string ln_function::str()
	{
		return "(ln" + _arg->str() + ")";
	}

	double ln_function::value(double x)
	{
		try
		{
			return std::log(_arg->value(x));
		}
		catch (...)
		{
			throw;
		}
	}

	base_function_ptr ln_function::derivative()
	{
		return std::make_shared<multiply_function>(std::make_shared<divide_function>(std::make_shared<constant_function>(1), _arg), _arg->derivative());
	}


	//sin_function
	sin_function::sin_function(base_function_ptr arg) :_arg(arg)
	{
	}

	std::string sin_function::str()
	{
		return "(sin" + _arg->str() + ")";
	}

	double sin_function::value(double x)
	{
		try
		{
			return std::sin(_arg->value(x));
		}
		catch (...)
		{
			throw;
		}
	}

	base_function_ptr sin_function::derivative()
	{
		return std::make_shared<multiply_function>(std::make_shared<cos_function>(_arg), _arg->derivative());
	}

	//cos_function
	cos_function::cos_function(base_function_ptr arg) :_arg(arg)
	{
	}

	std::string cos_function::str()
	{
		return "(cos" + _arg->str() + ")";
	}

	double cos_function::value(double x)
	{
		try
		{
			return std::cos(_arg->value(x));
		}
		catch (...)
		{
			throw;
		}
	}

	base_function_ptr cos_function::derivative()
	{
		return std::make_shared<multiply_function>(std::make_shared<constant_function>(-1), std::make_shared<multiply_function>(std::make_shared<cos_function>(_arg), _arg->derivative()));
	}
	//class defination end

	int get_matching_bracket(const std::string& expr, unsigned int pos)
	{
		unsigned int tag = 0;
		if (expr.at(pos) == '(')
		{
			for (unsigned int i = pos; i < expr.size(); i++)
			{
				if (expr.at(i) == '(')
					tag++;
				else if (expr.at(i) == ')')
					tag--;
				if (tag == 0)
					return i;
			}
		}
		else if (expr.at(pos) == ')')
		{
			for (unsigned int i = pos; pos >= 0; i--)
			{
				if (expr.at(i) == ')')
					tag++;
				else if (expr.at(i) == '(')
					tag--;
				if (tag == 0)
					return i;
			}
		}
		return -1;
	}

	bool is_add_or_minus(char c)
	{
		return c == '+' || c == '-';
	}

	bool include_operators(const std::string& str)
	{
		for (auto c : str)
		{
			if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
			{
				return true;
			}
		}
		return false;
	}

	bool include_brackets(const std::string& str)
	{
		for (auto c : str)
		{
			if (c == '(' || c == ')')
			{
				return true;
			}
		}
		return false;
	}

	bool is_number(const std::string& str)
	{
		for (auto c : str)
		{
			if (!((c <= '9'&&c >= '0') || c == '.'))
			{
				return false;
			}
		}
		return true;
	}

	bool is_multiply_or_divide(char c)
	{
		return c == '*' || c == '/';
	}

	bool is_power(char c)
	{
		return c == '^';
	}


	base_function_ptr function_parse(std::string func_str)
	{
		try
		{
			func_str.erase(std::remove_if(func_str.begin(), func_str.end(),
				[](char c) {return c == ' '; }), func_str.end());
			while ((!func_str.empty()) && func_str.at(0) == '('&&get_matching_bracket(func_str, 0) == (func_str.size() - 1))
			{
				func_str.erase(func_str.begin());
				func_str.pop_back();
			}
			if (func_str.empty())
			{
				return std::make_shared<constant_function>(0);
			}

			if (func_str.at(func_str.size() - 1) == ')')
			{
				unsigned int fb = get_matching_bracket(func_str, func_str.size() - 1);
				if (fb == -1)
				{
					FUNCTION_PARSE_ERROR
				}
				auto func_name = func_str.substr(0, fb);
				if (!(include_brackets(func_name) || include_operators(func_name)))
				{
					if (func_name == "ln")
					{
						return std::make_shared<ln_function>(function_parse(func_str.substr(fb, func_str.size() - fb)));
					}
					else if (func_name == "sin")
					{
						return std::make_shared<sin_function>(function_parse(func_str.substr(fb, func_str.size() - fb)));
					}
					else if (func_name == "cos")
					{
						return std::make_shared<cos_function>(function_parse(func_str.substr(fb, func_str.size() - fb)));
					}
				}
			}

			if (!include_operators(func_str))
			{
				if (is_number(func_str))
				{
					return std::make_shared<constant_function>(std::atof(func_str.c_str()));
				}
				else
				{
					return std::make_shared<independent_variable>(func_str);
				}
			}


			unsigned int i = 0;
			std::vector<unsigned int> operators;
			while (i < func_str.size())
			{
				if (func_str.at(i) == '(')
				{
					auto next_br = get_matching_bracket(func_str, i);
					if (next_br == -1)
					{
						throw std::invalid_argument("no matching bracket");
					}
					i = next_br + 1;
					continue;
				}
				if (is_add_or_minus(func_str.at(i)))
				{
					operators.push_back(i);
				}
				i++;
			}
			if (!operators.empty())
			{
				auto pPre = function_parse(func_str.substr(0, operators[0]));
				for (i = 0; i < operators.size(); i++)
				{
					if (pPre == nullptr)
					{
						FUNCTION_PARSE_ERROR
					}
					switch (func_str.at(operators.at(i)))
					{
					case '+':
						if (i != operators.size() - 1)
						{
							pPre = std::make_shared<add_function>(pPre, function_parse(func_str.substr(operators.at(i) + 1, operators.at(i + 1) - operators.at(i) - 1)));
						}
						else
						{
							pPre = std::make_shared<add_function>(pPre, function_parse(func_str.substr(operators.at(i) + 1, func_str.size() - operators.at(i) - 1)));
						}
						break;
					case '-':
						if (i != operators.size() - 1)
						{
							pPre = std::make_shared<minus_function>(pPre, function_parse(func_str.substr(operators.at(i) + 1, operators.at(i + 1) - operators.at(i) - 1)));
						}
						else
						{
							pPre = std::make_shared<minus_function>(pPre, function_parse(func_str.substr(operators.at(i) + 1, func_str.size() - operators.at(i) - 1)));
						}
						break;
					}
				}
				return pPre;
			}
			else
			{
				operators.clear();
				i = 0;
				while (i < func_str.size())
				{
					if (func_str.at(i) == '(')
					{
						auto next_br = get_matching_bracket(func_str, i);
						if (next_br == -1)
						{
							throw std::invalid_argument("no matching bracket");
						}
						i = next_br + 1;
						continue;
					}
					if (is_multiply_or_divide(func_str.at(i)))
					{
						operators.push_back(i);
					}
					i++;
				}
				if (!operators.empty())
				{
					auto pPre = function_parse(func_str.substr(0, operators[0]));
					for (i = 0; i < operators.size(); i++)
					{
						if (pPre == nullptr)
						{
							FUNCTION_PARSE_ERROR
						}
						switch (func_str.at(operators.at(i)))
						{
						case '*':
							if (i != operators.size() - 1)
							{
								pPre = std::make_shared<multiply_function>(pPre, function_parse(func_str.substr(operators.at(i) + 1, operators.at(i + 1) - operators.at(i) - 1)));
							}
							else
							{
								pPre = std::make_shared<multiply_function>(pPre, function_parse(func_str.substr(operators.at(i) + 1, func_str.size() - operators.at(i) - 1)));
							}
							break;
						case '/':
							if (i != operators.size() - 1)
							{
								pPre = std::make_shared<divide_function>(pPre, function_parse(func_str.substr(operators.at(i) + 1, operators.at(i + 1) - operators.at(i) - 1)));
							}
							else
							{
								pPre = std::make_shared<divide_function>(pPre, function_parse(func_str.substr(operators.at(i) + 1, func_str.size() - operators.at(i) - 1)));
							}
							break;
						}
					}
					return pPre;
				}
				else
				{
					operators.clear();
					i = 0;
					while (i < func_str.size())
					{
						if (func_str.at(i) == '(')
						{
							auto next_br = get_matching_bracket(func_str, i);
							if (next_br == -1)
							{
								throw std::invalid_argument("no matching bracket");
							}
							i = next_br + 1;
							continue;
						}
						if (is_power(func_str.at(i)))
						{
							operators.push_back(i);
						}
						i++;
					}
					if (!operators.empty())
					{
						auto pPre = function_parse(func_str.substr(0, operators[0]));
						for (i = 0; i < operators.size(); i++)
						{
							if (pPre == nullptr)
							{
								FUNCTION_PARSE_ERROR
							}
							if (i != operators.size() - 1)
							{
								pPre = std::make_shared<power_function>(pPre, function_parse(func_str.substr(operators.at(i) + 1, operators.at(i + 1) - operators.at(i) - 1)));
							}
							else
							{
								pPre = std::make_shared<power_function>(pPre, function_parse(func_str.substr(operators.at(i) + 1, func_str.size() - operators.at(i) - 1)));
							}
						}
						return pPre;
					}
				}
			}
		}
		catch (...)
		{
			throw;
		}
		FUNCTION_PARSE_ERROR
		return nullptr;
	}

}//namespace fp
