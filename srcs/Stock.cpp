#include "../headers/Stock.hpp"
//#include "../headers/error.hpp"

Stock::Stock( void ){ return; }

Stock::~Stock( void ){ return; }

Stock::Stock( Stock const & ){ return; }

Stock & Stock::operator=( Stock const & cp )
{
	this->line = cp.line;
	this->all_commands = cp.all_commands;
	this->Nicks = cp.Nicks;
	this->Identities = cp.Identities;
	this->Channels = cp.Channels;
	this->full_command = cp.full_command;
	this->pass = cp.pass;
	this->word = cp.word;
	this->User = cp.User;
	this->fd_count = cp.fd_count;
	this->Channel_Count = cp.Channel_Count;
	for (int i = 0; i <= this->User; i++)
	{
		this->nick_already_set[i] = cp.nick_already_set[i];
		this->authentified[i] = cp.authentified[i];
		this->tmp_pass[i] = cp.tmp_pass[i];
		this->tmp_nick[i] = cp.tmp_nick[i];
		this->tmp_user[i] = cp.tmp_user[i];
		this->client_fd[i] = cp.client_fd[i];
	}
	return *this;
}

void Stock::Init_Values( int argc, char **argv )
{
	if ( argc != 3 )
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		exit(0);
	}
	this->port = Check_Port(argv[1]);
//	check_password(argv[2]);
	this->pass = argv[2];

//	this->Nicks.push_back("\0");;

	std::string tmp = "PASS"; // Stock->all_commands[0]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "NICK"; // Stock->all_commands[1]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "USER"; // Stock->all_commands[2]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "JOIN"; // Stock->all_commands[3]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "PRIVMSG"; // Stock->all_commands[4]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "MODE"; // Stock->all_commands[5]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "PING"; // Stock->all_commands[6]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "KICK"; // Stock->all_commands[7]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "INVITE"; // Stock->all_commands[8]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "PART"; // Stock->all_commands[9]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "QUIT"; // Stock->all_commands[10]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "TOPIC"; // Stock->all_commands[11]
	this->all_commands.push_back(tmp);
	tmp.clear();

	tmp = "NOTICE"; // Stock->all_commands[12]
	this->all_commands.push_back(tmp);
	tmp.clear();

	this->full_command["PASS"].push_back("PASS");
	this->full_command["PASS"].push_back("\0");
	this->full_command["PASS"].push_back("\0");

	this->full_command["NICK"].push_back("NICK");
	this->full_command["NICK"].push_back("\0");
	this->full_command["NICK"].push_back("\0");

	this->full_command["USER"].push_back("USER");
	this->full_command["USER"].push_back("\0");
	this->full_command["USER"].push_back("\0");
	this->full_command["USER"].push_back("\0");
	this->full_command["USER"].push_back("\0");
	this->full_command["USER"].push_back("\0");

	this->full_command["JOIN"].push_back("JOIN");
	this->full_command["JOIN"].push_back("\0"); // param A
	this->full_command["JOIN"].push_back("\0"); // param B
	this->full_command["JOIN"].push_back("\0");
//	Le param A est le nom du chan
//	Le param B est le mot de pass (key) du chan [optionnel]
//	JOIN peut posséder des params A et B multiples, le minimum est 1 param

	this->full_command["PRIVMSG"].push_back("PRIVMSG");
	this->full_command["PRIVMSG"].push_back("\0");
	this->full_command["PRIVMSG"].push_back("\0");
	this->full_command["PRIVMSG"].push_back("\0");


	this->full_command["MODE"].push_back("MODE");
	this->full_command["MODE"].push_back("\0");
	this->full_command["MODE"].push_back("\0");
	this->full_command["MODE"].push_back("\0");

	
	this->full_command["PING"].push_back("PING");
	this->full_command["PING"].push_back("\0");
	this->full_command["PING"].push_back("\0");

	this->full_command["KICK"].push_back("KICK");
	this->full_command["KICK"].push_back("\0");
	this->full_command["KICK"].push_back("\0");
	this->full_command["KICK"].push_back("\0");

	this->full_command["INVITE"].push_back("INVITE");
	this->full_command["INVITE"].push_back("\0");
	this->full_command["INVITE"].push_back("\0");
	this->full_command["INVITE"].push_back("\0");

	this->full_command["PART"].push_back("PART");
	this->full_command["PART"].push_back("\0");
	this->full_command["PART"].push_back("\0");

	this->full_command["QUIT"].push_back("QUIT");
	this->full_command["QUIT"].push_back("\0");

	this->full_command["TOPIC"].push_back("TOPIC");
	this->full_command["TOPIC"].push_back("\0");

	this->full_command["NOTICE"].push_back("NOTICE");
	this->full_command["NOTICE"].push_back("\0");
	this->full_command["NOTICE"].push_back("\0");
	this->full_command["NOTICE"].push_back("\0");

	this->User = 0;
	this->User_Count = 0;
	this->Channel_Count = 0;
	this->fd_count = 0;
	for (int i = 0; i < 100; i++)
	{
		this->nick_already_set[i] = 0;
		this->authentified[i] = 0;
		this->tmp_nick[i] = 0;
		this->tmp_pass[i] = 0;
		this->tmp_user[i] = 0;
		this->client_fd[i] = 0;
	}
}

char	*Stock::Check_Port( char *port )
{
// est-ce qu'on rajoute une limite de la securite sur les args donnés ?
// on pourrait rajouter le nombre de caracteres, une limite au port

	int     i;

	if (!this->Is_Number(port))
	{
		std::cerr << "Usage: <port> must be numeric" << std::endl;
		exit(0);
	}
	i = atoi( port );
	if ( i < 0 || i > 65535 )
	{
		std::cerr << "Usage: <port> can't be 0, negative or more than 65535" << std::endl;
		exit(0);
	}
// peut être faire une fonction qui vérifie si les ports sont utilisés ou pas
	return (port);	
}

bool Stock::Is_Number(const std::string s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
