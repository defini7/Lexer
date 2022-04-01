#include <iostream>
#include <ctype.h>
#include <list>
#include <string>
#include <fstream>

struct Token
{
	std::string type;
	std::string value;
};

bool strmatch(std::string a, std::string b, int start)
{
	for (int i = 0; i < b.length(); i++)
		if (a[start + i] != b[i])
			return false;

	return true;
}

std::list<Token> Anylize(const std::string sInput, const std::list<Token> lTokenList)
{
	std::list<Token> lTokenized;
	int nCursor = 0;

	auto next_token = [&]() -> Token
	{
		std::string num = "";
		std::string identifier = "";
		std::string str = "";

		while (isdigit(sInput[nCursor]) || sInput[nCursor] == '.')
		{
			num += sInput[nCursor++];

			if (num.length() > 0 && !isdigit(sInput[nCursor]) && sInput[nCursor] != '.')
				return { "number", num };
		}

		if (sInput[nCursor] == '"')
		{
			nCursor++;

			while (sInput[nCursor] != '"')
				str += sInput[nCursor++];

			nCursor++;
			return { "string", str };
		}

		for (const auto& t : lTokenList)
			if (strmatch(sInput, t.value, nCursor))
			{
				nCursor += t.value.length();
				return t;
			}

		while ((isalpha(sInput[nCursor]) || isdigit(sInput[nCursor])) && nCursor < sInput.length())
		{
			identifier += sInput[nCursor];
			nCursor++;
		}

		if (identifier.length() == 0)
			nCursor++;

		return { "identifier", identifier };
	};

	while (nCursor < sInput.length())
	{
		const Token token = next_token();
		if (token.value.length() != 0)
			lTokenized.push_back(token);
	}

	return lTokenized;
}

int main()
{
	const std::list<Token> lTokens = {
		{ "add", "+" },
		{ "sub", "-" },
		{ "mul", "*" },
		{ "div", "/" },
		{ "mod", "%" },
		{ "pow", "^" },
		{ "comma", "," },
		{ "quote", "\"" },
		{ "lparen", "(" },
		{ "rparen", ")" },
		{ "equals", "=" },
		{ "comma", "," },
		{ "notequals", "!=" },
		{ "newline", "\n" },
		{ "tab", "\t" },
		{ "carriage", "\r" },
		{ "comment", "'" }
	};

	std::ifstream file;
	file.open("test.parser", std::ios::in);

	std::string sInput = "";

	while (!file.eof())
	{
		std::string s;
		std::getline(file, s);
		sInput += s;
	}

	const std::list<Token> lTokenized = Anylize(sInput, lTokens);

	for (const auto& t : lTokenized)
		std::cout << "Type: " << t.type << "\nValue: " << t.value << "\n\n";

	return 0;
}
