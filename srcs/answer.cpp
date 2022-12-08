# include "../headers/answer.hpp"

/*void errorMessage(User user, int numReply, std::string arg)
{
    std::map<int, std::string> err;
    std::ostringstream s;

    if (*(arg.end() - 1) == '\n')
		arg = arg.substr(0, arg.length() - 1);
	s << ":ft_IRC " << numReply << " * " << arg << err[numReply] << "\r\n";

	std::string msg = s.str();

	if ( send(user.getFd(), &msg[0], msg.size(), 0) == -1 )
		throw errorExcute(strerror(errno));

}*/

void MessageG(int fd, std::string numReply, std::string arg, Stock *Stock)
{
    std::map<int, std::string> err;
    std::ostringstream s;

	if (*(arg.end() - 1) == '\n')
		arg = arg.substr(0, arg.length() - 1);

	if (Stock->tmp_user[Stock->User] == 1 && Stock->tmp_nick[Stock->User] == 1)
	{
		std::string tmp = ":";
		tmp += Stock->Identities[Stock->User][0];
		tmp += "!";
		tmp += Stock->Users[Stock->User][0];
		tmp += "@127.0.0.1";
		s << ":ft_IRC " << numReply << " " << Stock->Identities[Stock->User][0]
		<< " " << arg << "\n";

		std::string msg = s.str();

		
		if ( send(fd, &msg[0], msg.size(), 0) == -1 )
			throw errorExcute(strerror(errno));
		tmp.clear();
		return;
	}
	std::string tmp = ":Anonymous!Anonymous@127.0.0.1";
	s << ":ft_IRC " << numReply << " " << "Anonymous"
	<< " " << arg << "\n";

	std::string msg = s.str();

	if ( send(fd, &msg[0], msg.size(), 0) == -1 )
		throw errorExcute(strerror(errno));
	tmp.clear();

}
