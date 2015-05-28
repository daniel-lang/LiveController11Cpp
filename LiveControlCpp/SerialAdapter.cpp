//
//  SerialAdapter.cpp
//  LiveControlCpp
//
//  Created by Daniel Lang on 27/05/15.
//  Copyright (c) 2015 KISI. All rights reserved.
//

#include "SerialAdapter.h"

bool SerialAdapter::interpretHexInput(string input)
{
    stringReplace(input, "\r", "");
    int tildeIndex = (int)input.find('~');
    int caretIndex = (int)input.find('^');
    
    //whole command or beginning of command
    if(input.at(0) == '~' || input.at(0) == '^')
    {
        
        if(hasKeyInputValidLength(input))
        {
            if(isKeyDefault(input))
            {
                cout << "Key released: " << previousKey << endl;
            }
            previousKey = input;
            leftOver = "";
            
            return true;
        }
        else
        {
            leftOver = input;
            return false;
        }
    }
    //tilde is somewhere in there
    else if(tildeIndex != string::npos || caretIndex != string::npos)
    {
        string firstPart = input.substr(0, tildeIndex);
        string secondPart = input.substr(tildeIndex);
        
        interpretHexInput(firstPart);
        interpretHexInput(secondPart);
    }
    //tilde is not included
    else
    {
        bool result = interpretHexInput(leftOver + input);
        if(!result)
        {
            leftOver += input;
        }
    }
    
    return false;
}

bool SerialAdapter::hasKeyInputValidLength(string input)
{
    char buttonGroup = input.at(1);
    int tildeIndex = (int)input.find('~');
    //int caretIndex = (int)input.find('^');
    
    if(tildeIndex != string::npos)
    {
        if(buttonGroup == '1' || buttonGroup == '2' || buttonGroup == 'F')
        {
            return (input.length() == 5);
        }
        if(buttonGroup == '3' || buttonGroup == '4')
        {
            return (input.length() == 4);
        }
    }
    
    return false;
}

bool SerialAdapter::isKeyDefault(string input)
{
    bool isInputDefault = (input == "~1FFF" || input == "~2FFF" || input == "~FFFF");
    bool isSpecialKey = (input == "~3FF");
    bool isFader = regex_match(input, regex("~4[0-9A-F]{2}"));
    
    return isInputDefault || isSpecialKey || isFader;
}

void SerialAdapter::connect()
{
    serialPort = open("/dev/cu.usbserial-FTR6TP9F", O_RDWR | O_NOCTTY);
    cout << "connected to device" << endl;
    
    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);
    
    if ( tcgetattr ( serialPort, &tty ) != 0 ) {
        std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }
    
    tty_old = tty;
    
    cfsetospeed (&tty, (speed_t)B9600);
    cfsetispeed (&tty, (speed_t)B9600);
    
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;
    
    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
    
    /* Make raw */
    cfmakeraw(&tty);
    
    /* Flush Port, then applies attributes */
    tcflush( serialPort, TCIFLUSH );
    if ( tcsetattr ( serialPort, TCSANOW, &tty ) != 0) {
        std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }
}

string SerialAdapter::readFromPort()
{
    int n = 0,
    spot = 0;
    char buf = '\0';
    
    char response[1024];
    memset(response, '\0', sizeof response);
    
    do {
        n = read(serialPort, &buf, 1);
        sprintf(&response[spot], "%c", buf);
        spot += n;
    } while(buf != '\r' && n > 0);
    
    if (n < 0) {
        cout << "Error reading: " << strerror(errno) << endl;
    }
    else if (n == 0) {
        cout << "Read nothing!" << endl;
    }
    else {
        interpretHexInput(response);
    }
    
    return "";
}

void SerialAdapter::disconnect()
{
    close(serialPort);
    serialPort = NULL;
}

void SerialAdapter::stringReplace(std::string& str, const std::string& oldStr, const std::string& newStr){
    size_t pos = 0;
    while((pos = str.find(oldStr, pos)) != std::string::npos){
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}