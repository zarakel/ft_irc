#include "../headers/headers.hpp"
#include "../headers/answer.hpp"
#include "../headers/Quit.hpp"
#include "../headers/Server.hpp"

int QUIT(int poll_fd, Stock *Stock)
{

	if ((Stock->line.size() == 2) &&
	(Stock->line.size() < Stock->full_command["QUIT"].size() ||
	Stock->line.size() > Stock->full_command["QUIT"].size()))
	{
		MessageG( poll_fd, ERR_NEEDMOREPARAMS,
		":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);
	}

	if ((Stock->line.size() == 3) &&
	(Stock->line.size() < Stock->full_command["QUIT"].size() + 1 ||
	Stock->line.size() > Stock->full_command["QUIT"].size() + 1))
	{
		MessageG( poll_fd, ERR_NEEDMOREPARAMS,
		":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);
	}

	std::string reason = "Quit : ";
	
	if (Stock->line.size() > 1)
		reason += Stock->line[1];

	reason += "\n\r";

	for (size_t i = 0; i < reason.size(); i++)
	{
		if (send(poll_fd, &reason[i], 1, 0) == -1)
			perror("send :");
	}

	for (int a = 0; Stock->Channel_Count > 0 &&
	a < Stock->Channel_Count; a++)
	{
		Stock->Channels_Users[Stock->Channels[a][0]][Stock->User].clear();
		Stock->Channels_Users[Stock->Channels[a][0]][Stock->User].resize(0);
		Stock->Channels_Invite[Stock->Channels[a][0]][Stock->User] = 0;
		Stock->Channels_Op[Stock->Channels[a][0]][Stock->User] = 0;
	}

	if (Stock->Identities.size() > 0)
		Stock->Identities.erase(Stock->User);

	if (Stock->Users.size() > 0)
		Stock->Users.erase(Stock->User);

	Stock->tmp_pass[Stock->User] = 0;
	Stock->tmp_nick[Stock->User] = 0;
	Stock->tmp_user[Stock->User] = 0;
	Stock->authentified[Stock->User] = 0;
	close(Stock->client_fd[Stock->User]);
	Stock->line.clear();
	return (0);
}
