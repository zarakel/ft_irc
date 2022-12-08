#include "../headers/headers.hpp"

std::string ft_addback(std::vector<std::string> str, std::string sep, int begin)
{
	std::string  res;

	for ( size_t i = begin; i < str.size(); i++ ) {
		res += str[i];
	if ( i != str.size() - 1 )
		res += sep;
	}
	return res;
}

/*
ft_split : split str into a tab on each caracters of delimiter
- in : str to split
- delimiter : list of delimiters to split
- out : reference to the outpout
*/

void    ft_split(std::string &in, char *delimiter, std::vector<std::string> &out)
{
    if (in.empty())
		return ;
	char *token = strtok(const_cast<char*>(in.c_str()), delimiter);
    while (token != NULL)
    {
        out.push_back(std::string(token));
        token = strtok(NULL, delimiter);
	}
}

std::vector<std::string>                ft_split(std::string str, std::string sep)
{

        std::vector<std::string>        res;
        std::string                     tok;

        size_t     start = 0;
        size_t     end = str.find(sep);
        while (end != std::string::npos)
        {
                res.push_back(str.substr(start, end - start));
                start = end + sep.length();
                end = str.find(sep, start);
        }
        res.push_back(str.substr(start, end - start));
        return res;
}
