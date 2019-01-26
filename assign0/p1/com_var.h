#ifndef COM_VAR_H
#define COM_VAR_H

#include <unistd.h>

template <typename T>
class common_variable{
    public:
        common_variable()
        {
            pipe(p);
        }
        ~common_variable()
        {

        }
        int update(T val)
        {
            const char *msg;
            msg = (char*)val;
        }
        int retrieve()
        {

        }

    private:
        int p[2];
        T value;
};

#endif