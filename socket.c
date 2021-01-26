# include "main.h"

int Raw_Socket (const char *device)
{
	int sock = socket (PF_PACKET, SOCK_RAW, 0);						// Cria um socket do tipo Raw Socket

	if (sock == -1)
		strerror (errno);

	struct ifreq ifr;											// Descobre o ID da interface
	memset (&ifr, 0, sizeof (struct ifreq));						// Inicializa com 0
	memcpy (ifr.ifr_name, device, sizeof (device));					// Copia a parte da memoria de device em ifr.ifr_name
	if (ioctl (sock, SIOCGIFINDEX, &ifr) == -1)
		strerror (errno);

	int deviceid = ifr.ifr_ifindex;
	struct sockaddr_ll sll;										// Binda, no socket, o endereco guardado em sll

	memset (&sll, 0, sizeof (sll));								// Inicializa com 0
	sll.sll_family = AF_PACKET;									// Sempre AF_PACKET
	sll.sll_ifindex = deviceid;									// ID da interface
	sll.sll_protocol = htons (ETH_P_ALL);							// Protocolo da camada fisica

	if (bind (sock, (struct sockaddr *) &sll, sizeof(sll)) == -1)
		strerror (errno);

	struct packet_mreq mr;										// Coloca a interface em modo promiscuo
	memset (&mr, 0, sizeof (mr));									// Inicializa com 0

	mr.mr_ifindex = deviceid;									// ID da interface que sera mudada
	mr.mr_type = PACKET_MR_PROMISC;								// Modo promiscuo
	if (setsockopt (sock, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof (mr)) == -1)	// Coloca o socket sock em modo promiscuo
		strerror (errno);

	return sock;
}