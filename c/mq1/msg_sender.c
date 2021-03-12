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



int main(int argc, char** argv) {

	char *error;

	mqd_t msg_id;

	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = MAX_MESSAGES;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;

	if ((msg_id = mq_open (SERVER_QUEUE_NAME, O_WRONLY , QUEUE_PERMISSIONS, &attr)) == -1) {
		perror ("Server: mq_open (client)");
		exit (1);
	}

	char buffer [MSG_BUFFER_SIZE];

	if (mq_send (msg_id, *(argv+1), strlen(*(argv+1)), 0) == -1) {
		perror ("Client: Not able to send message to server");
	}



	return 0;

err:
	fprintf(stderr,"ERROR %s (%d) - %s\n",error,errno,strerror(errno));
	exit(errno);
}
