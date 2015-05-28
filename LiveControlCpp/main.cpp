//
//  main.cpp
//  LiveControlCpp
//
//  Created by Daniel Lang on 27/05/15.
//  Copyright (c) 2015 KISI. All rights reserved.
//

#include <iostream>
#include "SerialAdapter.h"

using namespace std;

int main(int argc, const char * argv[]) {
    SerialAdapter adapter;
    
//    adapter.interpretHexInput("~FFFF");
//    adapter.interpretHexInput("~FFF");
//    adapter.interpretHexInput("2~F");
//    adapter.interpretHexInput("EFF");
//    
//    adapter.interpretHexInput("~3FF");
//    adapter.interpretHexInput("~3A");
//    adapter.interpretHexInput("F~3");
//    adapter.interpretHexInput("AA");
    
    adapter.connect();
    
    string command;
    for (int i = 0; i < 50; i++) {
        adapter.readFromPort();
    }
    
    adapter.disconnect();
    return 0;
}
