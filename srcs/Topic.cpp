#include "../headers/headers.hpp"
#include "../headers/answer.hpp"
#include "../headers/Topic.hpp"

int TOPIC(int poll_fd, Stock *Stock)
{
    std::string       channels;
    std::string       chl;
	std::string		tmp;

    if ( Stock->line.size() < Stock->full_command["TOPIC"].size()) 
    {
		MessageG( poll_fd, ERR_NEEDMOREPARAMS, ":Need more parameters", Stock);
		return (1);
    }
    channels = Stock->line[1];
   
        for (size_t i = 0; i < Stock->Channels.size(); i++)
        {
            if (Stock->Channels[i][0].compare(channels) == 0)
                chl = channels;
        }

        if (chl.empty())
            MessageG( poll_fd, ERR_NOSUCHCHANNEL, channels, Stock);
        for (size_t roll = 0; roll < Stock->Channels_Users[channels].size(); roll++)
        {
            if (Stock->line.size() == 4)
            {
                if (Stock->Channels_Users[channels][roll] == Stock->Identities[Stock->User][0])
                {
                    //if user is not channel operator
                    if (Stock->Channels_Op[channels][Stock->User] == 0)
                        MessageG( poll_fd, ERR_CHANOPRIVSNEEDED, channels, Stock);
                    else
                    {
                        (Stock->line[2].compare(":") == 0) ? tmp = "" :
			tmp = Stock->line[2];
			MessageG(poll_fd, RPL_TOPIC, "Change topic to: " + tmp, Stock);
                        for (size_t roll = 0; roll < Stock->Channels.size(); roll++)
                        {
                            if (Stock->Channels[roll][0] == channels)
                                Stock->Channels[roll][2] = tmp;
                        }
                        break;
                    }   
                }
                else if (roll + 1 == Stock->Channels_Users[channels].size())
                MessageG( poll_fd, ERR_NOTONCHANNEL, channels, Stock);
            }
            if (Stock->line.size() == 3)
            {
                (Stock->line[1] == "")? MessageG( poll_fd, RPL_NOTOPIC,
		Stock->line[1] + " :No topic is set", Stock) :
                MessageG( poll_fd, RPL_TOPIC, Stock->line[1] + " :" +
		Stock->line[1], Stock);
                break;
            }
        }
    Stock->line.clear();
    return (0);
}
