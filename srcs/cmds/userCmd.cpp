/*
    https://www.techbull.com/techbull/guide/internet/irccommande.html#users
	http://www.iprelax.fr/irc/irc_rfcus4.php

   Command: USER
   Parameters: <username> <hostname> <servername> <realname>

   The USER message is used at the beginning of connection to specify
   the username, hostname, servername and realname of s new user.  It is
   also used in communication between servers to indicate new user
   arriving on IRC, since only after both USER and NICK have been
   received from a client does a user become registered.

   Between servers USER must to be prefixed with client's NICKname.
   Note that hostname and servername are normally ignored by the IRC
   server when the USER command comes from a directly connected client
   (for security reasons), but they are used in server to server
   communication.  This means that a NICK must always be sent to a
   remote server when a new user is being introduced to the rest of the
   network before the accompanying USER is sent.

   It must be noted that realname parameter must be the last parameter,
   because it may contain space characters and must be prefixed with a
   colon (':') to make sure this is recognised as such.

   Since it is easy for a client to lie about its username by relying
   solely on the USER message, the use of an "Identity Server" is
   recommended.  If the host which a user connects from has such a
   server enabled the username is set to that as in the reply from the
   "Identity Server".

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

   Examples:

   USER guest tolmoon tolsun :Ronnie Reagan

                                   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan".

   :testnick USER guest tolmoon tolsun :Ronnie Reagan
                                   ; message between servers with the
                                   nickname for which the USER command
                                   belongs to
*/

void USER(Server &server, User &user, std::vector<std::string> arg)
{
    if ((user.isPassChecked() == false) || (user.getNickname().compare("dflt_nick") == 0))
		return;

    if (args.size() < 5){
        errorMessage(user, ERR_NEEDMOREPARAMS, ":Need more parameters")
        return ;
    }
    
    if (user.getIsSet()){
		errorMessage(user, ERR_ALREADYREGISTRED, usr.getNickname());
		return ;
	}
    user.setUsername(arg[1]);
	user.setHostname(arg[2]);
	user.setServername(arg[3]);
    user.setRealname(ft_addback(arg, " ", 4));
	user.setIsSet(true);

	if (!user.getNickname().empty())
	{
		if (server.getPassword() != "")
			if (!check_password(usr, srv))
				return;
		
		cout << GREEN << "User #" << usr.getFd() << " registred as " << usr.getNick() << RESET << endl;
		messageoftheday(srv, usr);
	}
}
