Socket类 ：所有与套接字相关的操作封装起来 ，包括：套接字的创建 关闭 以及获取
InetAddress类： 所有与地址相关的操作都封装起来 包括 ：获取Ip 获取端口号 通过IP和端口号创建对象  
Acceptor类： 封装常规函数到本类中，例如：地址复用 端口复用 bind listen accept等
TCPConnection类： Acceptor调用accept函数时，表明3次握手成功。就可以使用该类的对象进行数据的收发
SocketIO类：把数据的收发操作全都封装在本类中

3个半事件：连接建立
          连接断开
          消息达到：文件描述符可读
          消息发送完成：半个事件