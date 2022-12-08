/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 13:31:27 by juan              #+#    #+#             */
/*   Updated: 2022/12/07 14:30:29 by juan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/headers.hpp"
#include "../headers/Kick.hpp"
#include "../headers/answer.hpp"
#include "../headers/tools.hpp"

int KICK(int poll_fd, Stock *Stock)
{
    std::string        channels;
    std::vector<std::string>	    toKick;  // remove an user
    std::string                     comment; //why me? give a reason
    std::vector<std::string>        victim; //select form list of User registed
    std::string                     chl; //select the one from channel to kick

	int check = 1;
    if ( Stock->line.size() < Stock->full_command["KICK"].size()) 
    {
		MessageG( poll_fd, ERR_NEEDMOREPARAMS,
		":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);
    }
	if ((Stock->line.size() == 4) &&
	(Stock->line.size() < Stock->full_command["KICK"].size() ||
	Stock->line.size() > Stock->full_command["KICK"].size()))
	{
		MessageG( poll_fd, ERR_NEEDMOREPARAMS,
		":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);
	}

	if ((Stock->line.size() == 5) &&
	(Stock->line.size() < Stock->full_command["KICK"].size() + 1 ||
	Stock->line.size() > Stock->full_command["KICK"].size() + 1))
	{
		MessageG( poll_fd, ERR_NEEDMOREPARAMS,
		":Not the good amount of parameters", Stock);
		Stock->line.clear();
		return (1);
	}

	channels = Stock->line[1];
 	toKick = ft_split(Stock->line[2], ",");
    
    comment = ":";
    if (Stock->line.size() == Stock->full_command["KICK"].size() + 1)
        comment += Stock->line[3];

        for (int a = 0; a < (int)Stock->Channels.size() ; a++)
	{
		if (Stock->Channels[a][0].compare(channels) == 0)
        	    chl = channels;
	}

        if (chl.empty())
	{
            MessageG( poll_fd, ERR_NOSUCHCHANNEL, channels, Stock);
		check = 0;
	}

        if (Stock->authentified[Stock->User] == 0)
	{
            MessageG( poll_fd, ERR_NOTREGISTERED, ":You have not registered", Stock);
		check = 0;
	}
        //if user isnt on channel
	
        for (size_t roll = 0; roll < Stock->Channels_Users[channels].size(); roll++)
        {
            if (Stock->Channels_Users[channels][roll] == Stock->Identities[Stock->User][0])
            {
                if (Stock->Channels_Op[channels][Stock->User] == 0)
		{
                   MessageG( poll_fd, ERR_CHANOPRIVSNEEDED, channels, Stock);
			check = 0;
		}
               	break;
            }
            else if (roll + 1 == Stock->Channels_Users[channels].size())
		{
                	MessageG( poll_fd, ERR_NOTONCHANNEL, channels, Stock);
			check = 0;
		}
        
       } 
        //find toKick

	int a = 0;
        for (size_t j = 0; j < Stock->Channels_Users[channels].size(); j++)
        {
            if (Stock->Channels_Users[channels][j] == toKick[a])
                	victim.push_back(toKick[a]);
            // if victim is not a valid nick
			// if victim is not on channel
		else if (j + 1 == Stock->Channels_Users[channels].size()
		&& victim.size() == 0)
        	{
			MessageG( poll_fd, ERR_NOTONCHANNEL, channels, Stock );
			check = 0;
			break;
		}

		else if (j + 1 == Stock->Channels_Users[channels].size()
		&& a + 1 < (int)toKick.size())
		{
			j = -1;
			a++;
		}
	}

	if (check == 0)
	{
		channels.clear();
		comment.clear();
		chl.clear();
		victim.clear();
		toKick.clear();
       		Stock->line.clear(); 
		return (0);
	}

	a = 0;
	int z = 0;
            for (; a < (int)victim.size();)
            {
			while (z < (int)Stock->Identities.size())
			{
				for (int y = 0; 
				Stock->Channels_Users[channels].size() != 0 && y < 
				(int)Stock->Channels_Users[channels].size(); y++)
				{
					if (z < (int)Stock->Identities.size() &&
					Stock->Identities[z][0] ==
					Stock->Channels_Users[channels][y])
					{
						MessageG(Stock->client_fd[z],
						"",
						"kicked " + victim[a] + " from " +
						channels + " " + comment, Stock);
						Stock->Channels_Invite[channels][z] = 0;
					}
				}
				z++;
			}
			if (a + 1 < (int)victim.size())
			{
				z = 0;
				a++;
			}
			else if (a + 1 == (int)victim.size())
				break;
		}

		a = 0;	
		std::vector<std::string>::iterator it;
		it = Stock->Channels_Users[channels].begin();
		for (size_t roll = 0; roll < Stock->Channels_Users[channels].size();
		roll++)
		{
			if (victim[a] == Stock->Channels_Users[channels][roll])
			{
				int check_del = 0;
				int check_op = 0;
				if (Stock->Channels_Users[channels].size() > 1
				&& roll == 0)
					check_op = 1;
				if (Stock->Channels_Users[channels].size() > 0)
				{
					if (Stock->Channels_Users[channels].size() == 1)
						check_del = 1;
					Stock->Channels_Users[channels].erase(it);
					if (check_op == 1)
					{
						Stock->Channels_Op[channels]
						[Stock->User] = 0;
						for (size_t a = 0; a <
						Stock->Channels_Op[channels].size();a++)
						{
							if ((int)a != Stock->User)
								Stock->Channels_Op
								[channels][a] = 1;
						}	
					}
				}
				if ((check_del = 1 ||
				roll + 1 == Stock->Channels_Users[channels].size()))
				{
					if (Stock->Channels_Users[channels].size() == 0)
					{
						Stock->Channels_Users.erase(channels);
						for (size_t chan = 0; chan < Stock->Channels.size(); chan++)
						{
							if (Stock->Channels[chan][0] ==
							channels && 
							Stock->Channels_Users
							[channels].size() == 0)
								Stock->Channels.erase
								(chan);
							if (Stock->Channels.size() == 0)
							{
								Stock->Channel_Count = 0;
								victim.clear();
								toKick.clear();
								channels.clear();
								comment.clear();
								chl.clear();
								Stock->line.clear();
								return (0);
							}
						}
					}
					it = Stock->Channels_Users[channels].begin();
					roll = 0;
				}
			}	
			it++;
			if (roll + 1 == Stock->Channels_Users[channels].size()
			&& a + 1 < (int)victim.size())
			{
				roll = -1;
				it = Stock->Channels_Users[channels].begin();
				a++;
			}
		}
		
	victim.clear();
	toKick.clear();
	channels.clear();
	comment.clear();
	chl.clear();
	Stock->line.clear();
    return (0);
}
