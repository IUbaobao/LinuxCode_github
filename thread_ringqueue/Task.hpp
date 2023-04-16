
#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <map>
class CallTask
{
public:
    using func_t = std::function<int(int, int, char)>;

public:
    CallTask() {}
    CallTask(int x, int y, char op, func_t func)
        : _x(x), _y(y), _op(op), _callback(func)
    {
    }

    std::string operator()()
    {
        int reslut = _callback(_x, _y, _op);
        char buff[128];
        snprintf(buff, sizeof buff, "%d %c %d = %d ", _x, _op, _y, reslut);
        return buff;
    }

    std::string toSting()
    {
        char buff[128];
        snprintf(buff, sizeof buff, "%d %c %d = ?", _x, _op, _y);
        return buff;
    }

private:
    int _x;
    int _y;
    char _op;
    func_t _callback;
};

const std::string oper = "+-*/%";

int mymath(int x, int y, char op)
{
    using func_t = std::function<int(int, int)>;
    std::map<char, func_t> OperMap =
    {
        {'+', [](int x, int y)
         { return x + y; }},
        {'-', [](int x, int y)
         { return x - y; }},
        {'*', [](int x, int y)
         { return x * y; }},
        {'/', [](int x, int y)
         {
            if(y==0)
            {
                std::cout<<"除零错误"<<std::endl;
                return -1;
            }
                return x/y; }},
        {'%', [](int x, int y)
         {
            if(y==0)
            {
                std::cout<<"模零错误"<<std::endl;
                return -1;
            }
            return x%y; }}
    };
    return OperMap[op](x,y);
}



class SaveTask
{
public:
    using func_t =std::function<void(std::string&)>;
public:
    SaveTask()
    {}
    SaveTask(const std::string& message,func_t func)
    :_message(message),_func(func)
    {}

    void operator()()
    {
        _func(_message);
    }

private:
    std::string _message;
    func_t _func;
};

void save(std::string& message)
{
    const std::string filename="./log.txt";
    FILE* fp=fopen(filename.c_str(),"a+");
    if(fp==nullptr)
    {
        std::cerr<<"fopen errr"<<std::endl;
        exit(1);
    }
    fputs(message.c_str(),fp);
    fputs("\n",fp);
    fclose(fp);
}