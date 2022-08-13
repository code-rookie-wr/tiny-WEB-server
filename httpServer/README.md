HTTP服务器
-----------------
使用IO多路复用技术下的epoll和多线程实现了http服务器的并发处理，可使用浏览器通过ip访问服务器下的demo.html  
包含服务器端的代码server.cpp，错误处理函数的集合errorProcess.cpp及头文件errorProcess.h  
html文件夹中包含demo.html及一些错误情况下的.html文件

测试环境：Ubuntu 20.04 LTS
