#ifndef PARSE_MESSAGE_HPP
# define PARSE_MESSAGE_HPP

# include "Stock.hpp"

int     command_check(int poll_fd, Stock *Stock);
int     receive_message(int poll_fd, Stock *Stock);
int	auth_check(int poll_fd, Stock *Stock);
void	authenticate(int poll_fd, Stock *Stock);


#endif
