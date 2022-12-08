#include "../headers/headers.hpp"
#include "../headers/Invite.hpp"
#include "../headers/answer.hpp"

int INVITE(int poll_fd, Stock *Stock)
{
	//line[1] -> nickname
	//line[2] -> chan

	if (Stock->line.size() < Stock->full_command["INVITE"].size() ||
	Stock->line.size() > Stock->full_command["INVITE"].size())
	{
		MessageG( poll_fd, ERR_NEEDMOREPARAMS,
		":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);
	}

	int invited = 0;
	for (size_t roll = 0; roll < Stock->Identities.size(); roll++)
	{
		if (Stock->line[1] == Stock->Identities[roll][0])
		{
			invited = (int)roll;
			break;
		}
		if (roll + 1 == Stock->Identities.size())
		{
			Stock->line.clear();
			return (0);
		}
	}
	for (size_t roll = 0; roll < Stock->Channels.size(); roll++)
	{
		if (Stock->line[2] == Stock->Channels[roll][0])
		{
			for (size_t search = 0;
			search < Stock->Channels_Users[Stock->Channels[roll][0]].size();
			search++)
			{
				for (size_t s2 = 0; s2 < Stock->Channels_Users[Stock->Channels[roll][0]].size(); s2++)
				{
					if (Stock->Channels_Users
					[Stock->Channels[roll][0]][s2] == Stock->line[1])
					{
						MessageG(poll_fd, ERR_USERONCHANNEL,
						"Invited User is already on the chan",
						Stock);
						Stock->line.clear();
						return (0);
					}
				}
				if (Stock->Channels_Users[Stock->Channels[roll][0]]
				[search] == Stock->Identities[Stock->User][0])
				{
					if (Stock->Channels_Op[Stock->line[2]]
					[Stock->User] == 0 &&
					Stock->Channels[roll][3] == "+i")
					{
						MessageG(poll_fd, ERR_CHANOPRIVSNEEDED,
						"Inviting User isn't OP", Stock);
						break;
					}
					MessageG(poll_fd, RPL_INVITING, "Invitation sent",
					Stock);
					MessageG(Stock->client_fd[invited], RPL_INVITING,
					" has invited " + Stock->line[1] +
					" to the channel " + Stock->line[2], Stock);
					Stock->line.clear();
					Stock->Channels_Invite[Stock->Channels[roll][0]]
					[invited] = 1;
					return (1);
				}
				if (search + 1 ==
				Stock->Channels_Users[Stock->Channels[roll][0]].size())
				{
					MessageG(poll_fd, ERR_NOTONCHANNEL,
					"Inviting User is Unknown", Stock);
				}
			}
			break;
		}

		if (roll + 1 == Stock->Channels.size())
		{
			MessageG(poll_fd, ERR_NOSUCHCHANNEL, "Channel Unknown",
			Stock);
		}
	}
	Stock->line.clear();
	return (0);
}
