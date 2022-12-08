#include "../headers/User.hpp"
#include "../headers/answer.hpp"

int USER(int poll_fd, Stock *Stock)
{
	int tmp = poll_fd;
	tmp += 1;

	if (Stock->line.size() < Stock->full_command["USER"].size() ||
		Stock->line.size() > Stock->full_command["USER"].size())
	{
		MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);	
	}

	if (Stock->authentified[Stock->User] == 1)
	{
		MessageG(poll_fd, ERR_ALREADYREGISTERED, ":Already registered", Stock);
		Stock->line.clear();
		return (1);	
	}

	for (size_t i = 1; i < Stock->line.size(); i++)
		Stock->Users[Stock->User].push_back(Stock->line[i]);
	Stock->tmp_user[Stock->User] = 1;
	Stock->line.clear();
	return (0);
}
