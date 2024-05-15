#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
 
#define TRUE 1
 
/*
* このプログラムは、ソケットを作成したあと無限ループを開始します。
* ループごとに接続を受け入れ、その接続からのデータを出力します。
* 接続が遮断するか、あるいはクライアントが接続を閉じる時点で
* プログラムは新しい接続を受け入れます。
*/
 
main()
{
   int sock, length;
   struct sockaddr_in6 server;
   int msgsock;
   char buf[1024];
   int rval;
 
   /* ソケットを作成する。*/
   sock = socket(AF_INET6, SOCK_STREAM, 0);
   if (sock == -1) {
      perror("opening stream socket");
      exit(1);
   }
   /* ワイルドカードを使用してソケットをバインドする。*/
   bzero (&server, sizeof(server));
   bzero (&sin6, sizeof (sin6));
   server.sin6_family = AF_INET6;
   server.sin6_addr.s6_addr = in6addr_any;
   server.sin6_port = 0;
   if (bind(sock, (struct sockaddr *) &server, sizeof server)
         == -1) {
      perror("binding stream socket");
      exit(1);
   }
   /* 割り当てられたポート番号を調べ、それを出力する。*/
   length = sizeof server;
   if (getsockname(sock,(struct sockaddr *) &server,&length)
         == -1) {
      perror("getting socket name");
      exit(1);
   }
   printf("Socket port #%d¥n", ntohs(server.sin6_port));
   /* 接続の受け入れを開始する。*/
   listen(sock, 5);
   do {
      msgsock = accept(sock,(struct sockaddr *) 0,(int *) 0);
      if (msgsock == -1
         perror("accept");
      else do {
         memset(buf, 0, sizeof buf);
         if ((rval = read(msgsock,buf, 1024)) == -1)
            perror("reading stream message");
         if (rval == 0)
            printf("Ending connection¥n");
         else
            /* データが出力可能であると想定する */
            printf("-->%s¥n", buf);
      } while (rval > 0);
      close(msgsock);
   } while(TRUE);
   /*
    * このプログラムには無限ループが含まれるため、ソケット "sock" は
    * 明示的に閉じられることはありません。しかし、プロセスが中断されるか
    * 正常に終了する場合は自動的に閉じます。
    */
   exit(0);
}
