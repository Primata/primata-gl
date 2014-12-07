//
//  fileIO.cpp
//  Klephtica
//
//  Created by Carlos Sousa on 2014-11-05.
//  Copyright (c) 2014 css. All rights reserved.
//

#include "fileIO.h"
#include <limits>

using std::numeric_limits;

fileIO::fileIO (const string & s) : fileName( s )
{
    file.exceptions( ifstream::failbit | ifstream::badbit );
    try
    {
        file.open( fileName );
    }
    catch (ifstream::failure e)
    {
        cerr << "Exception operating file: " << fileName << endl
        << "Error message: " << e.what() << endl;
        file.close();
    }
}

fileIO::~fileIO ()
{
    if (file.is_open())
        file.close();
}

void fileIO::close ()
{
    if (file.is_open())
        file.close();
}

void fileIO::strToNumber (float * vec, const string & s)
{
    if (s.empty()) throw "Empty field found.";
    else
    {
        const size_t N = s.length();
        size_t i = 0; // Records the position of the character being analysed
        
        // Ignore leading white spaces
        if (isAllSpaces( s, i, N )) throw "Empty field found.";
        
        if (s[i] == '+' or s[i] == '-') {
            if (++i == N) throw "Invalid number format.";
        }
        
        if (digits.find(s[i]) != string::npos)
        {
            if(++i == N) {
                vec[counter++] = stof(s);
                return;
            }
            
            while (digits.find(s[i]) != string::npos) {
                if (++i == N) {
                    vec[counter++] = stof(s);
                    return;
                }
            }
            
            if (s[i] == '.')
            {
                if (++i == N) {
                    vec[counter++] = stof(s);
                    return;
                }
                
                while (digits.find(s[i]) != string::npos)
                {
                step1:
                    if (++i == N) {
                        vec[counter++] = stof(s);
                        return;
                    }
                }
            }
            
            // Checking for the exponent
            if (s[i] == 'e' or s[i] == 'E')
            {
                if (++i == N) throw "Invalid exponent format.";
                
                if (s[i] == '+' or s[i] == '-')
                {
                    if (++i == N) throw "Invalid exponent format.";
                }
                
                if (digits.find(s[i]) != string::npos)
                {
                    // Must have at least one digit after exponent char (or after sign
                    // char if it exists)
                    if (++i == N) {
                        vec[counter++] = stof(s);
                        return;
                    }
                    
                    // Second digit onwards
                    while (digits.find(s[i]) != string::npos)
                    {
                        if (++i == N) {
                            vec[counter++] = stof(s);
                            return;
                        }
                    }
                }
                else throw "Invalid exponent format.";
            }
            
            if (!isAllSpaces( s, i, N )) throw "Invalid character found.";
            
            // If no exception was throwned then try to convert to float
            vec[counter++] = stof(s);
        }
        else if (s[i] == '.')
        {
            if (++i == N) throw "Invalid number format.";
            else if (digits.find(s[i]) != string::npos) goto step1;
            else throw "Invalid field.";
        }
        else throw "Invalid character found.";
    }
}

void fileIO::strToNumber (unsigned short * vec, const string & s)
{
    if (s.empty()) throw "Empty field found.";
    else
    {
        const size_t N = s.length();
        size_t i = 0; // Records the position of the character being analysed
        
        // Ignore leading white spaces
        if (isAllSpaces( s, i, N )) throw "Empty field found.";
        
        if (s[i] == '+' or s[i] == '-')
            if (++i == N) throw "Invalid number format.";
        
        while (digits.find(s[i]) != string::npos)
        {
            if (++i == N)
            {
                unsigned long n = stoul(s);
                if (n <= numeric_limits<unsigned short>::max())
                {
                    vec[counter++] = (unsigned short) n;
                    return;
                }
                else throw "Unsigned short number out of limits.";
            }
        }
        
        if (!isAllSpaces( s, i, N )) throw "Invalid character found.";
        
        // If no exception was throwned then try to convert to unsigned short
        unsigned long n = stoul(s);
        if (n <= numeric_limits<unsigned short>::max())
            vec[counter++] = (unsigned short) n;
        else throw "Unsigned short number out of limits.";
    }
}

void fileIO::strToNumber (unsigned long * vec, const string & s)
{
    if (s.empty()) throw "Empty field found.";
    else
    {
        const size_t N = s.length();
        size_t i = 0; // Records the position of the character being analysed
        
        // Ignore leading white spaces
        if (isAllSpaces( s, i, N )) throw "Empty field found.";
        
        if (s[i] == '+' or s[i] == '-')
            if (++i == N) throw "Invalid number format.";
        
        while (digits.find(s[i]) != string::npos)
        {
            if (++i == N)
            {
                vec[counter++] = stoul(s);
                return;
            }
        }
        
        if (!isAllSpaces( s, i, N )) throw "Invalid character found.";
        
        // If no exception was throwned
        vec[counter++] = stoul(s);
    }
}

bool fileIO::isAllSpaces (const string & s, size_t & i, const size_t & N)
{
    while (i < N) {
        if (s[i] == ' ' or s[i] == '\t') i++;
        else return false;
    }
    return true;
}

bool fileIO::isAllSpaces (const string & s)
{
    for (char c : s) {
        if (c != ' ' and c != '\t') return false;
    }
    return true;
}