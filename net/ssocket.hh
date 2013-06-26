#if !defined(STREAMSOCKET_HH)
#define STREAMSOCKET_HH

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/un.h>

#define USE_SOCKLEN_T
const int SOCKET_TCP = 0;
const int SOCKET_UDP = 1;
const int SOCKET_UNIX_STREAM = 2;
const int SOCKET_UNIX_DGRAM = 3;

const int SHUTDOWN_RECV = 0x00;
const int SHUTDOWN_SEND = 0x01;
const int SHUTDOWN_BOTH = 0x02;

const int SOCKET_BUF_SIZE = 1024;

class SSocket {
	int fd;
public:
	/*
	 * Construct function
	 */
	SSocket(int fd = -1);

	/*
	 * Destruct function
	 */
	~SSocket();

	/*
	 * Send data to addr on specified port
	 */
	int sendto(const char *addr, 
		int port, 
		const char *buf, 
		int size = 0);

	/*
	 * Send data to addr ,ip is in inet addr long format
	 */
	int sendto(unsigned long ip,
		int port, 
		const char *buf, 
		int size = 0);

	/*
	 * Receive data 
	 */
	int recvfrom(
		unsigned long &ip,
		int &port,
		char* buf, 
		int size, 
		int timeout);

	/*
	 * Receive data, ip is in inet addr long format 
	 */
	int recvfrom(
		unsigned long &ip,
		int &port,
		char* buf, 
		int size);

	/*
	 * Receive data with timeout, no need to know where is the data from
	 */
	int recvfrom(
		char* buf, 
		int size, 
		int timeout);

	/*
	 * Receive data, no need to know where is the data from
	 */
	int recvfrom(
		char* buf, 
		int size); 

	/*
	 * Receive data 
	 */
	int recvfrom(
		char *ip,
		int ipSize,
		int &port,
		char* buf, 
		int size);

	/*
	 * Receive data  with timeout
	 */
	int recvfrom(
		char *ip,
		int ipSize,
		int &port,
		char* buf, 
		int size,
		int timeout);

	/*
	 * Receive with timeout
	 */
	int recv(
		char* buf, 
		int size, 
		int timeout);
		
	/*
	 * Receive without timeout
	 */
	int recv(
		char* buf, 
		int size); 
		
	/*
	 * Accept connect and  return the fd
	 */
	int acceptFD( );
	
	/*
	 * Accept connect and  return SSocket
	 */
	SSocket accept( );

	/*
	 * Accept connect and  return SSocket with timeout
	 */
	SSocket accept(int timeout);

	/*
	 * Accept connect and  return the fd with timeout
	 */
	int acceptFD(int timeout);

	/*
	 * shutdown
	 */
	bool shutdown(int mode = SHUTDOWN_BOTH);

	/*
	 * Create socket 
	 */
	int create(int type);

	/*
	 * Connect to address
	 */
	bool connect(const char *address,int port);

	/*
	 * Connect to address with timeout
	 */
	bool connect(const char *address,int port, int timeout);

	/*
	 * send
	 */
	int send(const char *buf, int size = 0);

//	bool close( ) const;
	bool close( ) ;

	bool sClose( );

	/*
	 * Listen
	 */
	bool listen(int n);

	/*
	 * Bind to port
	 */
	bool bind(int port);

	/**
	 * bind for UNIX internal socket.
	 */
	bool bind(const char* path);

	/**
	 * connect for UNIX internal socket.
	 */
	bool connectUnix(const char* path);

	bool connectUnix(const char* path, int timeout);

	bool available();

	/*
	 * Bind to ip and port
	 */
	bool bind(const char *ip, int port);

	/*
	 * Get address by socket
	 */
	bool getAddrBySocket(char *address) ;

	bool getAddrBySocket(int ifd, char *address);

	/*
	 * Get host by name
	 */
	bool getHostByName(const char *host, char *ip);

	inline void setFD(int i) {
//		close();
		fd = i;
	}

	inline int getFD( ) const {
		return fd;
	}

	unsigned long getIPLong() const;

	inline SSocket& operator = (const SSocket &ssocket) {
//		close();
		fd = ssocket.getFD();
		return *this;
	}

	bool setBlock(int block);
};

#endif 
