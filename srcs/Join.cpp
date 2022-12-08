#include "../headers/headers.hpp"
#include "../headers/Join.hpp"
#include "../headers/answer.hpp"

int	JOIN(int poll_fd, Stock * Stock)
{
	std::vector<std::string > tmp_Channel;
	std::vector<std::string > tmp_Key;
	std::string tmp;
	int	it = 0;
	int	o = 0;

	if (Stock->line.size() == 4 && (Stock->line.size() <
	Stock->full_command["JOIN"].size() || Stock->line.size() >
	Stock->full_command["JOIN"].size()))
	{
		MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);
	}

	else if (Stock->line.size() >= 5 && (Stock->line.size() <
	Stock->full_command["JOIN"].size() + 1 || Stock->line.size() >
	Stock->full_command["JOIN"].size()) + 1)
	{
		MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);
	}

	if (Stock->line.size() == 2)
	{
		MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":Channel is empty", Stock);
		Stock->line.clear();
		return (0);
	}

	if (Stock->line[1][0] != '#' && Stock->line[1][0] != '&' &&
		Stock->line[1][0] != '+' && Stock->line[1][0] != '!')
	{
		MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":Bad channel prefix", Stock);
		Stock->line.clear();
		return (0);
	}

	if (Stock->line.size() > 50)
	{
		MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":More than 50 characters", Stock);
		Stock->line.clear();
		return (0);
	}

	for ( int i = 0; Stock->line[1][i]; i++)
	{
		if (Stock->line[1][i] == ' ' | Stock->line[1][i] == 7)
		{
			MessageG(poll_fd, ERR_NEEDMOREPARAMS,
			":Chan name contain wrong characters", Stock);
			Stock->line.clear();
			return (0);
		}
		tmp.push_back(Stock->line[1][i]);
		if ( Stock->line[1][i + 1] == '\0'
		| Stock->line[1][i + 1] == ',' )
		{
			tmp_Channel.push_back(tmp);
			tmp.clear();
		}
		if (Stock->line[1][i + 1] == ',' &&
		(Stock->line[1][i + 2] == '#' || Stock->line[1][i + 2] == '&'
		|| Stock->line[1][i + 2] == '+' || Stock->line[1][i + 2] == '!'))
		{
			i++;
			it++;
		}
		else if (Stock->line[1][i + 1] == ',' &&
		(Stock->line[1][i + 2] != '#' || Stock->line[1][i + 2] != '&'
		|| Stock->line[1][i + 2] != '+' || Stock->line[1][i + 2] != '!'))
		{
			MessageG(poll_fd, ERR_NEEDMOREPARAMS,
			":Chan name contain wrong characters", Stock);
			Stock->line.clear();
			return (0);
		}
	}
	tmp_Channel.push_back("\0");
	tmp.clear();
	for (int i = 0; i <= it; i++)
		tmp_Key.push_back("\0");
	it = 0;

// vérif des keys
	for ( int i = 0; Stock->line[2][i] && 
	Stock->line[2][i] != '\n' | Stock->line[2][i] != '\r'; i++)
	{
		if (Stock->line[2][i] == ' ' || Stock->line[2][i] == 7)
		{
			MessageG(poll_fd, ERR_NEEDMOREPARAMS,
			":Key contain wrong characters", Stock);
			Stock->line.clear();
			return (0);
		}
		tmp.push_back(Stock->line[2][i]);
		it++;
		if ( Stock->line[2][i + 1] == '\0'
		| Stock->line[2][i + 1] == ',' )
		{
			tmp_Key[o].replace(0, it, tmp);
			tmp.clear();
		}

		if (Stock->line[2][i + 1] == ',' &&
		Stock->line[2][i + 2] == ',')
		{
			it = 0;
			i++;
			o++;
		}
	}

	o = 0;
	for ( int i = 0; Stock->Channel_Count > 0 && i < (Stock->Channel_Count + 1)
	&& tmp_Channel[o].compare("\0") != 0; i++)
	{
		// si on repere un channel déja existant

		if ( i < Stock->Channel_Count && 
		tmp_Channel[o] == Stock->Channels[i][0] &&
		tmp_Key[o] == Stock->Channels[i][1])
		{
					for ( size_t z = 0;
			z < Stock->Channels_Users[Stock->Channels[i][0]].size(); z++)
			{
				if (Stock->Identities[Stock->User][0] == 
				Stock->Channels_Users[Stock->Channels[i][0]][z])
				{
					MessageG(poll_fd, RPL_TOPIC,
					"is already on channel [" +
					Stock->Channels[i][0] + "] : " +
					Stock->Channels[i][2] , Stock);
					break;
				}

				else if (z + 1 == 
				Stock->Channels_Users[Stock->Channels[i][0]].size())
				{
					if (Stock->Channels[i].size() == 3 ||
					(Stock->Channels[i].size() == 4 &&
					Stock->Channels[i][3].compare("+i") == 0 &&
					Stock->Channels_Invite[Stock->Channels[i][0]]
					[Stock->User] == 1))
					{
						MessageG(poll_fd, RPL_TOPIC,
						"has joined channel [" +
						Stock->Channels[i][0] + "] :" +
						Stock->Channels[i][2] , Stock);
						Stock->Channels_Users
						[Stock->Channels[i][0]].push_back
						(Stock->Identities[Stock->User][0]);
						break;
					}
					else if (Stock->Channels[i].size() == 4
					&& Stock->Channels[i][3].compare("+i") == 0
					&& Stock->Channels_Invite[Stock->Channels[i][0]]
					[Stock->User] == 0)
					{
						MessageG(poll_fd, ERR_INVITEONLYCHAN,
						":Bad Usage: Invite only Chan", Stock);
					}
				}
			}
			o++;
			if (o == (int)tmp_Channel.size())
			{
				tmp_Channel.clear();
				tmp_Key.clear();
				Stock->line.clear();
				return (1);
			}
		}

		else if ( i < Stock->Channel_Count &&
		tmp_Channel[o] == Stock->Channels[i][0] &&
		tmp_Key[o] != Stock->Channels[i][1])
		{
			MessageG(poll_fd, ERR_BADCHANNELKEY, ": Wrong key", Stock);
			tmp_Channel.clear();
			tmp_Key.clear();
			Stock->line.clear();
			return (0);	
		}
		else if ( i == Stock->Channel_Count &&
		o < (int)tmp_Channel.size() )
		{
			for (int check = 0; check <= Stock->Channel_Count; check++)
			{
				if (tmp_Channel[o] == Stock->Channels[check][0])
				{
					i = -1;
					break;
				}
				else if (check + 1 == Stock->Channel_Count)
				{
					Stock->Channels[i].push_back(tmp_Channel[o]);
					Stock->Channels[i].push_back("\0");
					Stock->Channels[i].push_back("\0");
					MessageG(poll_fd, RPL_TOPIC, ": New Channel [" +
					Stock->Channels[i][0] +
					"] Created" + Stock->Channels[i][2] , Stock);
					Stock->Channels_Users
					[Stock->Channels[i][0]].push_back
					(Stock->Identities[Stock->User][0]);
					for (int roll = 0; roll <
					(int)Stock->Identities.size(); roll++)
					{
						if (roll == Stock->User)
						{
							Stock->Channels_Op
							[Stock->Channels[i][0]]
							.push_back(1);
							Stock->Channels_Invite
							[Stock->Channels[i][0]]
							.push_back(1);
							roll++;
						}
						Stock->Channels_Op[Stock->Channels[i][0]]
						.push_back(0);
						Stock->Channels_Invite
						[Stock->Channels[i][0]].push_back(0);
					}
					Stock->Channel_Count++;
					if (o + 1 < (int)tmp_Channel.size() )
					{
						o++;
						i = -1;
						break;
					}
					else 
						tmp_Channel.clear();
						tmp_Key.clear();
						Stock->line.clear();
						return (1);
				}
			}

		}
	}

	if ( Stock->Channel_Count == 0 )
	{
		for (int i = 0; i < (int)tmp_Channel.size() &&
		tmp_Channel[i].compare("\0") != 0; i++)
		{
			if (tmp_Key[i].compare("\0") != 0)
			{
				MessageG(poll_fd, ERR_BADCHANNELKEY,
				": Wrong key", Stock);
				tmp_Channel.clear();
				tmp_Key.clear();
				Stock->line.clear();
				return(0);	
			}
			Stock->Channels[i].push_back(tmp_Channel[i]);
			Stock->Channels[i].push_back(tmp_Key[i]);
			Stock->Channels[i].push_back("\0");
			MessageG(poll_fd, RPL_TOPIC, "has joined channel [" +
			Stock->Channels[i][0] + "] :" +
			Stock->Channels[i][2] , Stock);
			Stock->Channels_Users
			[Stock->Channels[i][0]].push_back
			(Stock->Identities[Stock->User][0]);
			for (int roll = 0; roll < (int)Stock->Identities.size(); roll++)
					{
						if (roll == Stock->User)
						{
							Stock->Channels_Op
							[Stock->Channels[i][0]].
							push_back(1);
							Stock->Channels_Invite
							[Stock->Channels[i][0]].
							push_back(1);
							roll++;
						}
						Stock->Channels_Op[Stock->Channels[i][0]]
						.push_back(0);
						Stock->Channels_Invite
						[Stock->Channels[i][0]].push_back(0);
					}
			Stock->Channel_Count++;
		}
	}
	tmp_Channel.clear();
	tmp_Key.clear();
	Stock->line.clear();
	return (0);
}
