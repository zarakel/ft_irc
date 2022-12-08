#include "../headers/headers.hpp"
#include "../headers/Command.hpp"
#include "../headers/parse_message.hpp"
#include "../headers/answer.hpp"

int auth_check(int poll_fd, Stock * Stock)
{
	if (Stock->authentified[Stock->User] == 0)
	{
		MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":You aren't authentified", Stock);
		Stock->line.clear();
		return (0);
	}
	return (1);
}	

void authenticate(int poll_fd, Stock *Stock)
{
	if (Stock->tmp_nick[Stock->User] == 1 &&
	Stock->tmp_user[Stock->User] == 1 &&
	Stock->tmp_pass[Stock->User] == 1 &&
	Stock->authentified[Stock->User] == 0)
	{
		Stock->authentified[Stock->User] = 1;
		Stock->Identities[Stock->User].push_back("\0");
		Stock->Flag.push_back("\0");
		std::string tmp = " :" + Stock->Identities[Stock->User][0];
		tmp += "!" + Stock->Users[Stock->User][0] + "@127.0.0.1";
		MessageG(poll_fd, RPL_WELCOME, "Welcome to the IRC" + tmp, Stock);
		tmp.clear();
		if (Stock->Channels.size() > 0)
		{
			for (size_t roll = 0; roll < Stock->Channels.size(); roll++)
			{
				Stock->Channels_Op
				[Stock->Channels[roll][0]].push_back(0);
				Stock->Channels_Invite[Stock->Channels[roll][0]].
				push_back(0);
			}
		}
	}
}

int	command_check(int poll_fd, Stock *Stock)
{
	if (Stock->line[0] == Stock->all_commands[6])
	{
		PING(poll_fd, Stock);
		return (0);
	}

	if (Stock->line[0] == Stock->all_commands[0])
	{
		PASS(poll_fd, Stock);
		authenticate(poll_fd, Stock);
		return (1);
	}

	if (Stock->line[0] == Stock->all_commands[1]
	&& Stock->tmp_pass[Stock->User] == 1)
	{
		NICK(poll_fd, Stock);
		authenticate(poll_fd, Stock);
		return (1);
	}

	if (Stock->line[0] == Stock->all_commands[2]
	&& Stock->tmp_pass[Stock->User] == 1)
	{
		USER(poll_fd, Stock);
		authenticate(poll_fd, Stock);
		return (1);
	}

	else if (Stock->line[0] == Stock->all_commands[3]
	&& auth_check(poll_fd, Stock))
	{
		JOIN(poll_fd, Stock);
		return (1);
	}
			
	else if (Stock->line[0] == Stock->all_commands[4]
	&& auth_check(poll_fd, Stock))
	{
		PRIVMSG(poll_fd, Stock);
		return (1);
	}

	else if (Stock->line[0] == Stock->all_commands[5]
	&& auth_check(poll_fd, Stock))
	{
		MODE(poll_fd, Stock);
		return (1);
	}

	else if (Stock->line[0] == Stock->all_commands[7]
	&& auth_check(poll_fd, Stock))
	{
		KICK(poll_fd, Stock);
		return (1);
	}

	else if (Stock->line[0] == Stock->all_commands[8]
	&& auth_check(poll_fd, Stock))
	{
		INVITE(poll_fd, Stock);
		return (1);
	}

	else if (Stock->line[0] == Stock->all_commands[9]
	&& auth_check(poll_fd, Stock))
	{
		PART(poll_fd, Stock);
		return (1);
	}

	else if (Stock->line[0] == Stock->all_commands[10]
	&& auth_check(poll_fd, Stock))
	{
		QUIT(poll_fd, Stock);
		return (1);
	}

	else if (Stock->line[0] == Stock->all_commands[11]
	&& auth_check(poll_fd, Stock))
	{
		TOPIC(poll_fd, Stock);
		return (1);
	}

	else if (Stock->line[0] == Stock->all_commands[12]
	&& auth_check(poll_fd, Stock))
	{
		NOTICE(poll_fd, Stock);
		return (1);
	}

	Stock->line.clear();
	return (0);
}

int	receive_message(int poll_fd, Stock *Stock)
{
	char		buf[513];
	memset(buf, 0, 513);

	recv(poll_fd, buf, 513, 0);

	if (buf[0] != 0)
	{
		Stock->buf += buf;
	}

	for (size_t roll = 0; std::string(buf) != "\r\n"; roll++)
	{
		if (buf[roll] == '\0')
			return (1);

		else if (buf[roll] == '\n')
			break;
	}

	if (Stock->buf[0] == '\n')
	{
		Stock->buf.clear();
		MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":Bad Usage", Stock);
		return(1);
	}

	for (int it = 0; ((Stock->buf[it] != '\n' | Stock->buf[it] != '\r') && it <= 512) ; it++)
	{
		if (Stock->buf[it] == ':' && (Stock->buf[it + 1] == ' ' || Stock->buf[it + 1] == '\n'
			|| Stock->buf[it + 1] == '\r'))
		{
			Stock->word.clear();
			Stock->line.clear();
			Stock->buf.clear();
			return (1);
		}	
		Stock->word.push_back(Stock->buf[it]);
		if (Stock->buf[it + 1] == ' ')
		{
			while (Stock->buf[it + 1] == ' ')
				it++;
			Stock->line.push_back(Stock->word);
			Stock->word.clear();
			if (Stock->buf[it + 1] == '\n' | Stock->buf[it + 1] == '\r')
			{
				break;
			}
		}
		if (Stock->buf[it + 1] == ':' && Stock->buf[it + 2] != '\0')
		{
			it += 2 ;
			for (size_t end = it; Stock->buf[end] != '\n'; end++)
				Stock->word.push_back(Stock->buf[end]);
			Stock->line.push_back(Stock->word);
			Stock->line.push_back("\0"); // NULL pour savoir quand fin vector
			Stock->word.clear();
			break;
		}	
		if (Stock->buf[it + 1] == '\n' | Stock->buf[it + 1] == '\r')
		{
			Stock->line.push_back(Stock->word);
			Stock->line.push_back("\0"); // NULL pour savoir quand fin vector
			Stock->word.clear();
			break;
		}
		else if (Stock->buf[it + 1] == '\0')
		{
			it += 2;
		}
		if (it > 512)
		{
			Stock->word.clear();
			Stock->line.clear();
			Stock->buf.clear();
			return (1);
		}	
	}
	Stock->word.clear();
	for (size_t c = 0; c < Stock->all_commands.size(); c++) 
	{
		if (Stock->line[0] == Stock->all_commands[c])
		{
			Stock->buf.clear();
			return (0);
		}
	}
	Stock->line.clear();
	Stock->buf.clear();
	MessageG(poll_fd, ERR_NEEDMOREPARAMS, ":Bad Usage", Stock);
	return(1);
}

