#include "../headers/headers.hpp"
#include "../headers/Command.hpp"
#include "../headers/answer.hpp"

int	PASS(int poll_fd, Stock *Stock)
{

	if (Stock->line.size() < Stock->full_command["PASS"].size() ||
	Stock->line.size() > Stock->full_command["PASS"].size())
	{
		MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);
	}

	std::string tmp;
	if (Stock->authentified[Stock->User] == 1 | Stock->tmp_pass[Stock->User] == 1)
	{
		MessageG(poll_fd, ERR_ALREADYREGISTERED, ":You are already registered", Stock);
		Stock->line.clear();
		return (1);
	}
	if (Stock->pass.compare(Stock->line[1]) == 0)
	{
		Stock->line.clear();
		Stock->tmp_pass[Stock->User] = 1;
	//	std::cout << "Good Pass !\n" << std::endl;
		return(0);
	}
	MessageG(poll_fd, ERR_PASSWDMISMATCH, ":Bad password", Stock);
	tmp.clear();
	Stock->line.clear();
	return (1);
}
