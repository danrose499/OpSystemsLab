#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1024

int main (int argc, char* argv[]) {

    //Open file
    int fd = open(argv[1], O_RDONLY), length;

    //Check for error opening
    if (fd < 0) {
        perror("open");
        close(fd);
        return 1;
    }       

    //Read file and write to cout
    char buffer[BUFSIZE];
    while( (length = read(fd, buffer, BUFSIZE)) > 0 ) { 
       if(write(1, buffer, length) == -1){
           perror("write"); //Check for error writting
       }
    }
    if(length == -1) //Check for error reading
        perror("read");

    //Close
    if (close(fd) != 0){
        perror("close");
        return 1;
    }
        
    return 0;
}