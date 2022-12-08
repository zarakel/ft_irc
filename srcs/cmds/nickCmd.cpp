#include "../../headers/headers.hpp"
#include "../../headers/Command.hpp"

int	NICK(int poll_fd, Stock *Stock)
{

	if (Stock->line[1].length() > 0 && Stock->line[2].empty() 
	&& Stock->Identities[Stock->User][1].empty())
	{
		Stock->Identities[Stock->User].push_back(Stock->line[1]);
		if (send(poll_fd, "All Good: Your nick is set !\n\r", 29, 0) == -1)
			perror("send");
		Stock->line.clear();
		Stock->nick_already_set[Stock->User] = 1;
		return (20);
	}
	else if (Stock->line[1].length() > 0 && Stock->line[2].empty() 
	&& Stock->Identities[Stock->User][1].empty() == 0
	&& Stock->Identities[Stock->User][1].compare(Stock->line[1]) != 0)
	{
		Stock->Identities[Stock->User].pop_back();
		Stock->Identities[Stock->User].push_back(Stock->line[1]);
		if (send(poll_fd, "All Good: Your nick have changed !\n\r", 35, 0) == -1)
			perror("send");
		Stock->line.clear();
		return (60);
	}
	else if (Stock->line[1].length() > 0 && Stock->line[2].empty() 
	&& Stock->Identities[Stock->User][1].compare(Stock->line[1]) == 0)
	{
		if (send(poll_fd, "Bad Param': Same nick\n\r", 23, 0) == -1)
			perror("send");
		Stock->line.clear();
		return (76);
	}
// Peut etre ajouter sécu
	else
	{
		if (send(poll_fd, "Bad param's: You're nick is not set !\n\r", 39, 0) == -1)
			perror("send");
		Stock->line.clear();
	}
	return (0);
}
