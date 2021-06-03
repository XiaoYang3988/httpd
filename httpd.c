#include "httpd.h"

void sigchld_handler(int32_t sig) {
	
	while (waitpid(-1, 0, WNOHANG) > 0)
		continue;
	
	return;
}

int init_daemon() {   

    pid_t pid = 0;   

    /* 屏蔽一些控制终端操作的信号 */
    signal(SIGTTOU, SIG_IGN);   
    signal(SIGTTIN, SIG_IGN);   
    signal(SIGTSTP, SIG_IGN);   
    signal(SIGHUP , SIG_IGN);  
   
	/* 设置在后台运行 */
	if ((pid = Fork()) < 0)
		return (-1);
	else if (pid)
	/* 结束父进程，子进程继续 */  
		_exit(0);	

    /* 脱离控制终端、登录会话和进程组 */
    setsid();    
      
	/* 禁止进程重新打开控制终端 */
	if ((pid = Fork()) < 0)
		return (-1);
	else if (pid)
	/* 结束第一子进程，第二子进程继续（第二子进程不再是会话组长）*/
		_exit(0);			


    /* 关闭打开的文件描述符 */
    /* NOFILE 为 <sys/param.h> 的宏定义 */
    /* NOFILE 为文件描述符最大个数，不同系统有不同限制 */
	/* 大部分描述符是未开启的，因此关闭描述符的函数调用发生错误是不可避免的 */
    for(int32_t i = 0; i < NOFILE; ++i){  
        close(i);  
    }  
      
    /* 改变当前工作目录 */  
    //chdir("/tmp");   
      
    /* 重设文件创建掩模 */  
    umask(0);    

	/* 将stdin、stdout和stderr重定向到/dev/null */
	//open("/dev/null", O_RDONLY);
	//open("/dev/null", O_RDWR);
	//open("/dev/null", O_RDWR);

    /* 处理 SIGCHLD 信号 */  
    //signal(SIGCHLD,SIG_IGN);  
      
    return 0;   
}   

int main(void) {
	
	struct sockaddr_in	server_ddr = {};
	int32_t	listening_fd = -1; 
    conf_file	conf = {};

	/* 守护进程初始化 */
	if(init_daemon() < 0) {
		err_sys("init_daemon fail");
	}

	/* 回收僵尸子进程 */
	Signal(SIGCHLD, sigchld_handler);

	/* 获取配置文件信息 */
	conf = return_conf_file();
	
	/* 向系统申请一个套接口描述符，使用的是ipv4网络地址族，类型为双向可信的数据流 */
	listening_fd = Socket(AF_INET, SOCK_STREAM, 0);

	/* 配置服务器ipv4网络地址结构体 */
	/* 地址族为ipv4 */
	server_ddr.sin_family = AF_INET;
	/* 配置端口号，将端口号转化为网络字节序 */
	server_ddr.sin_port = htons(conf.port);
	/* 配置可接受的网络地址，INADDR_ANY为接收任何传入消息的地址 */
	server_ddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	/* 将server_ddr ipv4网络协议赋予到 listening_fd 套接口描述符 */
	Bind(listening_fd, (SA*)&server_ddr, sizeof(server_ddr));
	
	/* 将 listening_fd 主动套接口转换成被动套接口，套接口排队的连接数为 BACKLOG */
	Listen(listening_fd, BACKLOG);

	while(1) {

		struct sockaddr_in	client_ddr = {};
		socklen_t	client_len = 0;
		int32_t	connection_fd = -1;
		
		///* 从已完成连接的队列中返回下一个已完成连接的套接口描述符 */
		//connection_fd = Accept(listening_fd, (SA *)&client_ddr, &client_len);
		///* 根据已连接的套接口描述符处理http请求 */
		//http_accept_dispose(connection_fd);	

		/* 从已完成连接的队列中返回下一个已完成连接的套接口描述符 */
		connection_fd = Accept(listening_fd, (SA *)&client_ddr, &client_len);
		if(Fork() == 0) {
			Close(listening_fd);
			/* 根据已连接的套接口描述符处理http请求 */
			http_accept_dispose(connection_fd);	
			exit(0);
		}
		Close(connection_fd);
	}
	
	Close(listening_fd);
    return 0;
}
