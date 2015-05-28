//
//  SerialAdapter.h
//  LiveControlCpp
//
//  Created by Daniel Lang on 27/05/15.
//  Copyright (c) 2015 KISI. All rights reserved.
//

#ifndef LiveControlCpp_SerialAdapter_h
#define LiveControlCpp_SerialAdapter_h

#include <iostream>
#include <string>
#include <regex>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

using namespace std;

class SerialAdapter
{
private:
    string leftOver;
    string previousKey;
    int serialPort;
    void stringReplace(string&, const string&, const string&);
    bool hasKeyInputValidLength(string);
    bool isKeyDefault(string);
    
public:
    void connect();
    void disconnect();
    string readFromPort();
    bool interpretHexInput(string);
};

#endif
