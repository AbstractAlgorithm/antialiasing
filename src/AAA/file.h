#ifndef _core_file_h_
#define _core_file_h_

#include <stdio.h>
#include <stdlib.h>

namespace File
{

    char* loadFile(const char* filename)
    {
        if (!filename) return 0;												// try openning file
        FILE* file;
        if (!(file = fopen(filename, "r"))) return 0;

        int n = 0;																// allocate memory
        char *str = (char*)malloc(sizeof(char));
        if (str == NULL)
        {
#ifdef DEBUGMODE
            printf("ERROR: Could not read file: %s.\n", filename);
#endif
            return 0;
        }

        char c;																	// read file
        while (true)
        {
            c = fgetc(file);
            str[n] = c;
            n++;
            str = (char*)realloc(str, sizeof(char)* n + sizeof(char));
            if (c == EOF)break;
        }
        str[n - 1] = '\0';

        return str;
    }
}

#endif