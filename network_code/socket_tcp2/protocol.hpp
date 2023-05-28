#pragma once 

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>


#define SEP " "
#define SEP_LEN strlen(SEP) 
#define LINE_SEP "\r\n"
#define LINE_SEP_LEN strlen(LINE_SEP)


enum
{
    OK,
    DIV_ZERO,
    MOD_ZERO,
    OP_ERROR,
};


// "x op y" -> "content_len"\r\n"x op y"\r\n
// "exitcode result" -> "content_len"\r\n"exitcode result"\r\n
//添加协议报头
std::string enLength(const std::string &text)
{
    std::string send_string=std::to_string(text.size());
    send_string+=LINE_SEP;
    send_string+=text;
    send_string+=LINE_SEP;
    
    return send_string;
}


// "content_len"\r\n"exitcode result"\r\n ->  "exitcode result"
bool deLength(const std::string&package,std::string* text)
{
    auto pos=package.find(LINE_SEP);
    if(pos==std::string::npos)
    {
        return false;
    }
    std::string text_len_string=package.substr(0,pos);
    int text_len=std::stoi(text_len_string);
    *text=package.substr(pos+LINE_SEP_LEN,text_len);
    return true;
}



class Request
{
public:
    Request(int x,int y,char op):_x(x),_y(y),_op(op){}

    Request():_x(0),_y(0),_op(0){}

    bool serialize(std::string*out)
    {
         // 结构化 -> "x op y";
         *out="";
         std::string x_string=std::to_string(_x);
         std::string y_string=std::to_string(_y);
         *out=x_string;
         *out+=SEP;
         *out+=_op;
         *out+=SEP;
         *out+=y_string;
         
         return true;
    }

    bool deserialize(std::string& in)
    {
        // "x op y" -> 结构化
        auto left=in.find(SEP);
        auto right=in.rfind(SEP);
        if(left==std::string::npos || right==std::string::npos)
            return false;
        
        if(left==right)
            return false;
        
        if(right-(left+SEP_LEN)!=1)
            return false;
        
        std::string x_string=in.substr(0,left);
        std::string y_string=in.substr(right+SEP_LEN);

        if(x_string.empty())
            return false;
        if(y_string.empty())
            return false;
        
        _x=std::stoi(x_string);
        _y=std::stoi(y_string);
        _op=in[left+SEP_LEN];

        return true;
    }
public:
    int _x;
    int _y;
    char _op;
};





class Response
{
public:
    Response(int exitcode,int result):_exitcode(exitcode),_result(result){}

    Response():_exitcode(0),_result(0){}


    bool serialize(std::string*out)
    {
        *out="";
        std::string ec_string=std::to_string(_exitcode);
        std::string res_string=std::to_string(_result);

        *out=ec_string;
        *out+=SEP;
        *out+=res_string;

        return true;

    }

    bool deserialize(const std::string& in)
    {
        // "exitcode result"
        auto mid=in.find(SEP);
        if(mid==std::string::npos)
            return false;
        
        std::string ec_string=in.substr(0,mid);
        std::string res_string=in.substr(mid+SEP_LEN);

        if(ec_string.empty())
            return false;
        if(res_string.empty())
            return false;
        
        _exitcode=std::stoi(ec_string);
        _result=std::stoi(res_string);
        return true;
    }

public:
    int _exitcode;// 0：计算成功，!0表示计算失败，具体是多少，定好标准
    int _result;// 计算结果
};




bool recvPackage(int sock,std::string&inbuffer,std::string* text)
{
    char buffer[1024];
    while(true)
    {
        ssize_t n=recv(sock,buffer,sizeof(buffer),0);
        if(n>0)
        {
            buffer[n]=0;
            inbuffer+=buffer;

            auto pos=inbuffer.find(LINE_SEP);
            if(pos==std::string::npos)
                continue;
            std::string text_len_string=inbuffer.substr(0,pos);
            int text_len=std::stoi(text_len_string);
            int total_len=text_len_string.size()+2*LINE_SEP_LEN+text_len;
            if(inbuffer.size()<total_len)
            {
                std::cout << "你输入的消息，没有严格遵守我们的协议，正在等待后续的内容, continue" << std::endl;
                continue;
            }

            // 至少有一个完整的报文

            *text=inbuffer.substr(0,total_len);
            inbuffer.erase(0,total_len);
            break;
        }
        else
            return false;
    }
    return true;
}


