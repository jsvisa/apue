#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>

struct udp_msg{
   int length;
   char msg[0];
};

int main( int argc, char **argv)
{
   struct sockaddr_in server, client;
   struct udp_msg *pmsg;
   char   clientip[100];
   char   *buf;
   int    serverfd;
   int    ret;
   int    len, msg_len;
   int count=0;

   buf = (char *)malloc(100*1024);
   serverfd = socket(AF_INET, SOCK_DGRAM, 0);
   if ( -1 == serverfd ){
      printf("create socket failed %s\n", strerror(errno));
      return -1;
   }

   server.sin_family = AF_INET;
   server.sin_port   = htons(60000);
   server.sin_addr.s_addr = INADDR_ANY;
   ret = bind(serverfd, (struct sockaddr *)&server, sizeof(server));
   if ( -1 == ret){
      printf("bind failed %s \n", strerror(errno));
      return -1;
   }

   while(1) {
	   len = sizeof(client);
	   
	   ret = recvfrom(serverfd, buf, 1000, MSG_WAITALL,(struct sockaddr*) &client, &len); 
	   //ret = recvfrom(serverfd, buf, 10, MSG_TRUNC,(struct sockaddr*) &client, &len); 
	    
	   if(ret > 0) {
		   count++;
		   printf("count=%d, size=%d\n", count, ret);
	//	   buf[ret] = 0;
		   while (ret-- > 0) 
		   {
				printf("buf=%0#x\n", (int)(*buf));
				buf++;
		   }

	   }
	   
	   /*
	   ret = recvfrom( serverfd, &msg_len, sizeof(msg_len), 0,(struct sockaddr*) &client, &len); 
	   bzero(buf,100);
	   ret = recvfrom( serverfd, buf, sizeof(msg_len)+ntohl(msg_len), 0,
					  (struct sockaddr*)&client, &len); 
	   pmsg = (struct udp_msg *) buf;
	   printf("recv %d bytes, content is %s\n",
	           ntohl(pmsg->length), pmsg->msg);

	   bzero(clientip,100);
	   inet_ntop(AF_INET, &client.sin_addr.s_addr, clientip, 100);
	   printf("client port is %d, address is  %s\n", 
	            ntohs(client.sin_port),clientip);
	   
	   if ( strcmp(pmsg->msg, "first") ==0 ){
		  strcpy( buf, "I'm receive first");
	   }
	   else{
		  strcpy( buf, "I'm receive others");
	   }

	   msg_len = strlen(buf);
	   pmsg = malloc( sizeof( struct udp_msg) + msg_len );
	   pmsg->length = htonl(msg_len);
	   strcpy(pmsg->msg, buf);

	   ret = sendto(serverfd, pmsg, sizeof(struct  udp_msg) + msg_len ,0,
	                 (struct sockaddr*)&client, len);
	   printf("sent %d bytes\n", ret);

	   free(pmsg);
	   */
   }

   close(serverfd);
   return 0;
}
