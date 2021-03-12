/******************************************* 
* Author     : Diego Cordoba / @d1cor      *
* Contact    : um.edu.ar / juncotic.com    *
*******************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<mqueue.h>
#define MSG_SIZE 128

#define SERVER_QUEUE_NAME   "/test"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

/*
           struct mq_attr {
               long mq_flags;       * Flags (ignored for mq_open())
               long mq_maxmsg;      * Max. # of messages on queue
               long mq_msgsize;     * Max. message size (bytes)
               long mq_curmsgs;     * # of messages currently in queue (ignored for mq_open())
           };

*/

int main(int argc, char** argv) {

	char *error;

	mqd_t msg_id;

	mq_unlink(SERVER_QUEUE_NAME);
	struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGES;
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_curmsgs = 0;

    if ((msg_id = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit (1);
    }

    char buffer [MSG_BUFFER_SIZE];

   while((mq_receive (msg_id, buffer, MSG_BUFFER_SIZE, NULL) )>0) {
		printf("Server message received: %s\n", buffer);
		memset(buffer,0,MSG_BUFFER_SIZE);
	}
	mq_unlink(SERVER_QUEUE_NAME);


	return 0;

err:
	fprintf(stderr,"ERROR %s (%d) - %s\n",error,errno,strerror(errno));
	exit(errno);
}

