#ifndef _PYTHON_READER_HPP
#define _PYTHON_READER_HPP

#pragma once

#include <iostream>
#include <string>


// C.C. : Possible better way is to overload 
// template but I can't figure  it out
inline void readStringFromPython(std::string &inStr, std::string name="String"){
    std::getline(std::cin, inStr);
    std::cerr << "Read in " << name << ": " << inStr << std::endl;
}


template<class T> inline void readValueFromPython(T *val, int n, std::string name){
    std::cerr << "Read in "<< name << ": ";
    for(int i=0; i<n; i++){
        std::cin >> val[i];
        std::cerr << val[i] << " ";
    }
    std::cin.ignore();
    std::cerr << std::endl;
}


template<class T> inline void readValueFromPython(T *val){
    readValueFromPython<T>(val, 1, "Value");
}

template<class T> inline void readValueFromPython(T *val, int n){
    readValueFromPython<T>(val, n, "Value");
}

template<class T> inline void readValueFromPython(T *val, std::string name){
    readValueFromPython<T>(val, 1, name);
}

#endif