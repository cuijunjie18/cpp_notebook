# Socket编程

## 背景

Socket 编程不仅是网络编程的“入门钥匙”，更是构建可靠、高效、可扩展网络应用的“核心引擎”。无论你是开发 Web 服务、移动应用、游戏服务器还是分布式系统，掌握 Socket 编程都是不可或缺的技能

## demo模板

### 客户端

```cpp
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXN 2048

void parse_url(const char *url,std::string &server_ip,
                std::string &server_port,
                std::string &filePath)
{
    int p;
    for (p = 0; url[p] != ':' && url[p] != '/'; p++) server_ip += url[p];
    if (url[p] == ':'){
        p++;
        for (; url[p] != '/'; p++) server_port += url[p];
        p++;
    }else{
        server_port = "80"; // 未指定端口使用默认端口.
    }
    for (; url[p] != '\0'; p++) filePath += url[p];
    if (filePath.empty()) filePath = "/"; // 默认根目录
}

std::string Get_request(
                const std::string &server_ip,
                const std::string &server_port,
                const std::string &filePath)
{
    std::string request  =  "GET /" + filePath + " HTTP/1.1\r\n"
                            "Host: " + server_ip + ":" + server_port;
    return request;
}

int main(int argc,char *argv[])
{
    if (argc < 2){
        std::cerr << "usage:" << argv[0] << " <url>" << std::endl;
        return -1;
    }

    // Parse url
    std::string server_ip;
    std::string server_port;
    std::string filePath;
    parse_url(argv[1], server_ip, server_port, filePath);
    uint16_t port = atoi(&server_port[0]);

    // Generate socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0){
        std::cerr << "Generate socket failed!" << std::endl;
        return -1;
    }

    // Set web_server addr
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET,&server_ip[0],&server_addr.sin_addr) < 0){
        std::cerr << "Server ip is invalid!" << std::endl;
        return -1;
    }

    // Connect to web_server
    if (connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        std::cerr << "Connect failed!" << std::endl;
        return -1;
    }
    std::cout << "Connect to " << server_ip << std::endl; 

    // 发送请求
    char recv_buf[MAXN];
    std::string send_buf = Get_request(server_ip, server_port, filePath);

    while (true){
        if (send(client_socket, &send_buf[0], send_buf.size(), 0) < 0){
            std::cerr << "Send failed!" << std::endl;
            break;
        }
        int recv_length = recv(client_socket, recv_buf, MAXN, 0);
        recv_buf[recv_length] = '\0';
        printf("%s\n",recv_buf);
        break;
    }

    close(client_socket);
    return 0;
}
```

### 服务端

```cpp
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <filesystem>

#define MAXN 2048

// 使用c++17的filesystem判断文件是否存在
bool fileExists(const std::string& filePath) {
    // return std::filesystem::exists(filePath);

    /* 仅检查文件，目录也算文件不存在 */
    std::error_code ec; // 用于避免异常的安全检查
    auto status = std::filesystem::status(filePath, ec);
    
    // 检查错误和文件类型
    return !ec && std::filesystem::is_regular_file(status);
}

// 构造404相应报文
void response_404(int socket){
    std::string content = "<html><body><h1>404 Not Found</h1></body></html>";
    std::string response =  "HTTP/1.1 404 Not Found\r\n"
                            "Content-Type: text/html\r\n"
                            "Connection: close\r\n" // 控制行为： 单次接受
                            "Content-Length: " + std::to_string(content.size()) + "\r\n"
                            "\r\n"
                            "<html><body><h1>404 Not Found</h1></body></html>";
    if (send(socket, &response[0], response.size(), 0) < 0 ){
        std::cerr << "Send failed!" << std::endl;
    }
    return;
}

// 发送文件内容
void send_file(int socket, const std::string& filePath) {
    // 1. 打开文件
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    // 2. 获取文件大小
    auto file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::cout << file_size << "\n";

    // 3. 构建HTTP响应头
    std::string header = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(file_size) + "\r\n"
        "Connection: close\r\n\r\n";

    // 4. 先发送响应头
    if (send(socket, header.c_str(), header.size(), 0) <= 0) {
        std::cerr << "Failed to send header" << std::endl;
        return;
    }

    // 5. 分块发送文件内容
    const size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    while (!file.eof()) {
        file.read(buffer, BUFFER_SIZE);
        int bytes_read = file.gcount();
        if (send(socket, buffer, bytes_read, 0) <= 0) {
            std::cerr << "Failed to send file content" << std::endl;
            return;
        }
    }

    return;
}

// 从http请求报文中解析出请求文件路径
std::string parse_http_request(char *recv_buf,int recv_length){
    int p;
    for (p = 0; p < recv_length; p++){
        if (recv_buf[p] != ' ') continue;
        p++;
        break;
    }
    std::string filePath = ""; 
    for (; recv_buf[p] != ' '; p++) filePath += recv_buf[p];
    if (filePath == "/") filePath = "/index.html";
    return filePath.substr(1); // 去掉/
}

int main(int argc,char *argv[])
{
    if (argc < 2){
        printf("usage:%s <port>\n",argv[0]);
        return -1;
    }

    // arguments parse
    uint16_t port = atoi(argv[1]); 

    // Generate listen socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        std::cerr << "Generate socket failed!" << std::endl;
        return -1;
    }

    // bind socket
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY; // 本地任意
    if (bind(server_socket,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0){
        std::cerr << "Bind failed!" << std::endl;
        return -1;
    }

    // Begin Listen port
    if (listen(server_socket, 1) < 0){
        std::cerr << "Listen failed!" << std::endl;
        return -1;
    } 
    std::cout << "Server is listening on port " << port << std::endl;

    // accept connect socket from listen socket
    sockaddr_in client_addr;
    socklen_t client_socketlen = sizeof(client_addr);
    int connect_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_socketlen);
    if (connect_socket < 0){
        std::cerr << "Connect failed!" << std::endl;
        return -1;
    }
    std::cout << "Client connect."<< std::endl;

    char recv_buf[MAXN];
    std::string filePath;

    while (true) {
        int recv_length = recv(connect_socket, recv_buf, MAXN, 0);
        recv_buf[recv_length] = '\0';
        printf("%s\n",recv_buf);
        if (recv_length > 0){
            filePath = parse_http_request(recv_buf,recv_length);
            if (fileExists(filePath)){
                send_file(connect_socket, filePath);
            }
            else{
                response_404(connect_socket);
            }
        }else if (recv_length == 0){
            std::cout << "Connect close." << std::endl;
            break;
        }else{
            std::cerr << "Recevie failed!" << std::endl;
            break;
        }
    }

    close(server_socket);
    close(connect_socket);
    return 0;
}
```

## 参考

[compute_network_lab](https://github.com/cuijunjie18/Computer_network)