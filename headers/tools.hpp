#ifndef TOOLS_HPP
# define TOOLS_HPP

# include "headers.hpp"

std::string			ft_addback(std::vector<std::string> str, std::string add, int begin=0);

void                ft_split(std::string &in, char *delimiter, std::vector<std::string> &out);

std::vector<std::string>                ft_split(std::string str, std::string sep);

#endif
