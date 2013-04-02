#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<pthread.h>
#include<termios.h>
#include<sys/stat.h>
#include<errno.h>


#define PROT 8000
#define server_ip "121.8.242.229"

static char buff[6] = {0};

void borad_send_server()
{
	int sockfd;
	int recv_byte;
	struct sockaddr_in server_addr;
	int con_ret;
	server_addr.sin_family		=AF_INET;
	server_addr.sin_port		=htons(PROT);
	server_addr.sin_addr.s_addr	=inet_addr(server_ip);
	bzero(&(server_addr.sin_zero),8);
	
	for(;;)
{
	
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket error!\n");
		exit(1);
	}
	printf("create a new socket\n");
	


	con_ret=connect(sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));
	if(con_ret==-1)
	{
		perror("connect error");
		sleep(1);
	}
	
	if(con_ret==0)
	{
		recv_byte=recv(sockfd,buff,6,0);
		if(recv_byte==-1)
		{
			perror("recv");
		}
		if(recv_byte==0)
		{
			printf("recv end\n");
		}
		if(recv_byte>0)
		{
			printf("%s\n",buff);
		}
	}

	close(sockfd);

}
printf("send end\n");
exit(0);

}


void board_seriel()
{
	int fd; 
//	char buf[100];
	char i; 
       // fd=open("/dev/ttyUSB0",O_RDWR); 
        fd=open("/dev/ttySAC1",O_RDWR); 
        if(-1==fd){ 
        perror("提示错误！："); 
        } 
        struct termios newport; 
        newport.c_cflag |= (CLOCAL | CREAD);//串口连接使能 
	cfmakeraw(&newport);//设置串口为原始模式 
        cfsetispeed(&newport,B115200);//设置输入波特率为115200 
        cfsetospeed(&newport,B115200);//设置输出波特率为115200 
        newport.c_cflag &=  ~PARENB;//不启用奇偶校验 
        newport.c_cflag &=  ~CSTOPB;//将停止位设置为一个比特 
        newport.c_cflag &=  ~CSIZE;//用数据位掩码清空数据位设置 
        newport.c_cflag |=  CS8;//设置为数据位为8位 
	newport.c_cc[VMIN]=0;
	newport.c_cc[VTIME]=0;
	tcflush(fd,TCIOFLUSH);//清空串口缓冲队列 
        tcsetattr(fd,TCSANOW,&newport);//串口配置立即生效 
//      memset(buf,0,100);//这里是对数组初始化 
       // while(1) 
//	for(;;)  
	//scanf("%s",buf);	
//	strcpy(buf,argv[1]);
//    	buf[6]='E';
    	while(1)
	{
	if(write(fd,buff,6)>0)
	{
		perror("write");
	}
	sleep(1);
	}
//	write(fd,"\n",strlen("\n"));
        memset(buff,0,6);//这里是对数组初始化 
        read(fd,buff,6);
	//因为传过来的字符是没有结束标志的，所以我们这里要少一位  
        //    printf("%s\n",buf);
	printf("%s\n",buff);
        close(fd);
}

int main()
{
	pthread_t send_id,serial_id;
	int ret;
	while(1)
	{
		ret=pthread_create(&send_id,NULL,(void*)borad_send_server,(void*)NULL);
		if(ret!=0)
		{
			perror("pthread board_send_server error\n");
			exit(1);
		}
		ret=pthread_create(&serial_id,NULL,(void*)board_seriel,(void*)NULL);
		if(ret!=0)
		{
			perror("pthread board_serial error\n");
			exit(1);
		}
		pthread_join(send_id,NULL);
		pthread_join(serial_id,NULL);
	}
	return 0;
}
