//
//  fileIO.h
//  Klephtica
//
//  Created by Carlos Sousa on 2014-11-05.
//  Copyright (c) 2014 css. All rights reserved.
//

#ifndef FILEIO_H
#define FILEIO_H
#include <iostream>
#include <string>
#include <fstream>
#include <typeinfo>

using std::string;
using std::ifstream;
using std::cerr;
using std::endl;
using std::exception;

class fileIO
{
public:
    fileIO (const string &);
    ~fileIO ();
    
    // Read a single data set from file
    template <typename T>
    void read (T *, const unsigned long &);
    
    // Read several data sets from file
    template <typename T>
    void read (T **, const unsigned long *, const unsigned int &);
    
    void close ();
    
private:
    const string digits {"0123456789"}; // Uniform initialization
    string fileName;
    ifstream file;
    string line;
    size_t pos;
    unsigned long counter;
    
    // Functions for parsing string in each field and then converting string to number
    template <typename T>
    void field (T *, const unsigned long &);
    
    template <typename T>
    void strToNumber (T *, const string &);
    
    void strToNumber (float *, const string &);
    void strToNumber (unsigned short *, const string &);
    void strToNumber (unsigned long *, const string &);
    
    bool isAllSpaces (const string & s, size_t & i, const size_t & N);
    bool isAllSpaces (const string & s);
};


template <typename T>
void fileIO::read (T * dataVec, const unsigned long & dataVecSize)
{
    try {
        counter = 0;
        while (counter < dataVecSize)
        {
            if (!file.eof())
            {
                // Call to getline may raise exception: ios_base::failure
                getline( file, line );
                
                if (!line.empty())
                {
                    if ((pos = line.find( "//" )) == string::npos)
                        // Comments not found, line is not empty
                        field( dataVec, dataVecSize );
                    else {
                        // Comments found, line is not empty
                        line.erase( pos );
                        if (!line.empty())
                            field( dataVec, dataVecSize );
                    }
                }
            }
            else throw "End of file reached before reading all the required fields.";
        }
    }
    catch (ifstream::failure e)
    {
        cerr << "Exception operating file: " << fileName << endl
        << "Error message: " << e.what() << endl;
        file.close();
        throw;
    }
    catch (const char * s)
    {
        cerr << "Exception while reading file: " << fileName << endl
        << "Error message: " << s << endl;
        file.close();
        throw;
    }
    catch (exception e)
    {
        cerr << "Error reading file: " << fileName << endl
        << "Exception: " << e.what() << endl;
        file.close();
        throw;
    }
    catch (...)
    {
        cerr << "Error reading file: " << fileName << endl
        << "Default error." << endl;
        file.close();
        throw;
    }
}

template <typename T>
void fileIO::read (T ** dataVecPtr,
           const unsigned long * dataVecSizePtr,
           const unsigned int & dataSetN)
{
    for (unsigned int dataSet = 0; dataSet < dataSetN; dataSet++)
        read( dataVecPtr[dataSet], dataVecSizePtr[dataSet] );
}

template <typename T>
void fileIO::field (T * vec, const unsigned long & vecSize)
{
    string word;
    
    if ((pos = line.find( ',' )) == string::npos)
    {
        // Delimiter not found, line is not empty
        if (!isAllSpaces( line ))
        {
            // Not all characters are spaces or tabs, delimiter was not found
            if (counter == vecSize-1)
            { // Last floating point number in data set is expected
                word = line;
                line.clear();
                strToNumber( vec, word );
            }
            else throw "Incomplete data set or invalid characters.";
        } // Else: string of all spaces is ignored
    }
    else
    {
        // Delimiter found, line is not empty
        if (counter == vecSize-1)
            throw "Delimiter found after last field of data set.";
        else
        {
            word = line.substr( 0, pos );
            line.erase( 0, pos+1 );
            strToNumber( vec, word );
            
            if (!line.empty())
                field( vec, vecSize );
        }
    }
}

template <typename T>
void fileIO::strToNumber (T * vec, const string & s)
{
    string msg = typeid(T).name();
    msg = "Error converting string to number: convertion to type " + msg
    + " not yet implemented.";
    throw msg.c_str();
}
#endif /* FILEIO_H */