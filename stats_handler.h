#ifndef STATS_HANDLER_H
#define STATS_HANDLER_H

#include <iostream>
#include <fstream>

class stats_handler
{
private:
    
    FILE* _file;

public:
    stats_handler() {
        
        _file = fopen("../octave/stats.txt","w");
    }

    ~stats_handler() {
        fclose(_file);
    }

    void save_time(double time) {
        fprintf(_file,"%f\n",time);
    }
};

#endif