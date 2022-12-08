/* 
1. INVITE
La commande /invite te permet d'inviter quelqu'un sur un channel. 
Si le channel est en mode +i (invite), 
il s'agit de la seule solutions pour autoriser les gens à rejoindre votre channel. 
N'abusez pas de cette commande, et évitez d'inviter en masse (massinvite).

    * Syntaxe : /invite <channelname> <nickname>
    Example : /invite #cool MEAT

Commande: INVITE
Paramètres: <pseudonyme> <canal>

Le message INVITE est utilisé pour inviter des utilisateurs dans un canal. 
Le paramètre <pseudonyme> est le pseudonyme de la personne à inviter dans le canal destination <canal>. 
Il n'est pas nécessaire que le canal dans lequel la personne est invitée existe, 
ni même soit valide. Pour inviter une personne dans un canal en mode sur invitation (MODE +i), 
le client envoyant l'invitation doit être opérateur sur le canal désigné.

Réponses numériques :

           ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_USERONCHANNEL
           ERR_CHANOPRIVSNEEDED
           RPL_INVITING                    RPL_AWAY
Exemples:

:Angel INVITE Wiz #Dust ; 
    L'utilisateur Angel invite WiZ sur le canal #Dust

INVITE Wiz #Twilight_Zone ; 
    Commande pour inviter WiZ sur #Twilight_zone
*/

#include "headers.hpp"

void    invite(User &user, Server &server, std::vector<std::string> args)
{
	if (!user.isRegistered())
		return ;
	if (args.size() < 2)
    {
		std::cerr << "ERR_NEEDMOREPARAMS (461)" << std::endl;
		return ;
	}
}