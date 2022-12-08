#include "../headers/headers.hpp"
#include "../headers/Server.hpp"

/*void	check_password( std::string pass_tmp ) -- Secu a ajouter 
{

}*/

int main(int argc, char **argv)
{

// On verfie qu'on ait assez d'arguments, que l'argv1, le port, n'est pas de 
// lettre, puis on les stocks 


	Stock Stock;
	try{
		Stock.Init_Values(argc, argv);
	}
	catch (const std::exception & e)
	{
		std::cerr << "exception found: " << e.what() << std::endl;
		return ( 1 );
	}

	std::cout << "Port = " << Stock.port << " && Pass = " << Stock.pass << std::endl;
	server(&Stock);

	return (0);
}
