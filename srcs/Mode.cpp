#include "../headers/Mode.hpp"
#include "../headers/headers.hpp"
#include "../headers/answer.hpp"

int MODE(int poll_fd, Stock *Stock)
{
	int user_check = -1;
	int chan_check = -1;
	size_t size = (Stock->Identities.size() <= (size_t)Stock->Channel_Count) ?
	Stock->Identities.size() : (size_t)Stock->Channel_Count;

	if ((Stock->line.size() < Stock->full_command["MODE"].size())
	|| (Stock->line.size() > Stock->full_command["MODE"].size()))
	{
		MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);
	}	

	for (size_t search = 0; search < size; search++)
	{
		if (search < Stock->Identities.size() && Stock->line[1] == Stock->Identities[search][0])
		{
			user_check = search;
			break;
		}
		
		if (search < (size_t)Stock->Channel_Count && Stock->line[1] == Stock->Channels[search][0])
		{
			chan_check = search;
			break;
		}

		if (search + 1 == size && user_check && (user_check == -1 && chan_check == -1))
		{
			MessageG(poll_fd, ERR_NOSUCHNICK, ": Nick isn't known", Stock);
			Stock->line.clear();
			return (1);
		}
	}
	if ((Stock->line[2][0] != '+' && Stock->line[2][0] != '-')
	&& (Stock->line[2][1] != 'a' && Stock->line[2][1] != 'i' &&
	Stock->line[2][1] != 'w' && Stock->line[2][1] != 'r' && Stock->line[2][1] != 'o'
	&& Stock->line[2][1] != 'O' && Stock->line[2][1] != 's'))
	{
		MessageG(poll_fd, ERR_UMODEUNKNOWNFLAG, ": Params aren't known", Stock);
		return (1);
	}
/*	for (int fill = 0; fill < Stock->User; fill++)
	{
		if ((int)Stock->Flag.size() > fill)
			fill = (int)Stock->Flag.size();
		if (fill < Stock->User && Stock->Flag.empty() == 1)
			Stock->Flag.pushback("\0");
	}*/
	if (user_check >= 0)
	{
		if (Stock->Identities[user_check][0].compare(Stock->Identities[Stock->User][0]) != 0)
		{	
			MessageG(poll_fd, ERR_USERSDONTMATCH, ": Nick doesn't match", Stock);
			Stock->line.clear();
			return (1);
		}
		/*Stock->Flag[user_check] = Stock->line[2];
		std::string tmp = Stock->line[2];
		tmp += '\n';
		if (send(poll_fd, static_cast<void *>(&tmp), tmp.size() + 1, 0) == -1)
			perror("send: ");*/
		MessageG(poll_fd, RPL_UMODEIS, ": " + Stock->line[2], Stock);
		Stock->line.clear();
		//tmp.clear();
		return (1);
	}
	if (chan_check >= 0)
	{
		/*Stock->Flag[user_check] = Stock->line[2];
		std::string tmp = Stock->line[2];
		tmp += '\n';
		if (send(poll_fd, static_cast<void *>(&tmp), tmp.size() + 1, 0) == -1)
			perror("send: ");*/
		MessageG(poll_fd, RPL_UMODEIS, ": " + Stock->line[2], Stock);
		if (Stock->Channels[chan_check].size() > 3)
			Stock->Channels[chan_check][3] = Stock->line[2];
		else
			Stock->Channels[chan_check].push_back(Stock->line[2]);
		Stock->line.clear();
		//tmp.clear();
		return (1);
	}
	Stock->line.clear();
	return (0);
}
