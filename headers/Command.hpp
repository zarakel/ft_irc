#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Stock.hpp"

int		PASS(int poll_fd, Stock *Stock);
int		NICK(int poll_fd, Stock *Stock);
int		USER(int poll_fd, Stock *Stock);
int		JOIN(int poll_fd, Stock *Stock);
int		PRIVMSG(int poll_fd, Stock *Stock);
int		MODE(int poll_fd, Stock *Stock);
int		PING(int poll_fd, Stock *Stock);
int		KICK(int poll_fd, Stock *Stock);
int		INVITE(int poll_fd, Stock *Stock);
int		PART(int poll_fd, Stock *Stock);
int		QUIT(int poll_fd, Stock *Stock);
int		TOPIC(int poll_fd, Stock *Stock);
int		NOTICE(int poll_fd, Stock *Stock);


#endif
