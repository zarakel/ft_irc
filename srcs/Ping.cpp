#include "../headers/Ping.hpp"
#include "../headers/Command.hpp"

int PING(int poll_fd, Stock *Stock)
{
	if(send(poll_fd, "PONG :ft_irc\n", 13, 0) != -1)
	{
		Stock->line.clear();
		return(1);
	}
	Stock->line.clear();
	return(0);
}
