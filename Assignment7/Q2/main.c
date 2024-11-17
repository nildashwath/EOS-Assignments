#include "circle.h"
#include "rectangle.h"
#include "square.h"
#include <unistd.h>

int main ()
{
    circle();
    sleep(1);

    rectangle();
    sleep(1);

    square();
    sleep(1);
    
    return 0;
}
