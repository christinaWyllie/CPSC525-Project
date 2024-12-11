# CPSC525 - CWE-416: Use After Free

## High-Level Description of the Code and Exploit
The code demonstrates a Use-After-Free (UAF) vulnerability in a course management system.

Code: The course management system allows professors to add students, submit grades, and publish them. Additionally, students can check their grades or drop the class.

Professors can submit grades for students and hold off on publishing them. During this time, grades are stored temporarily in a structure called tempGrades.

A Use-After-Free issue arises if a student drops the class after grades are submitted but before they are published. The program retains a dangling pointer to the freed memory.

If a new student is enrolled during this period, their memory allocation may overlap with the freed memory of the dropped student, leading to the new student inheriting the dropped student’s grades.

Exploit: The exploit simulates how a new student can unintentionally inherit grades of a dropped student due to the dangling pointer caused by the UAF vulnerability.

## How to Compile and Run the Code
Use SSH to connect to the universities linux server.

Clone the Repository:

    git clone https://github.com/christinaWyllie/CPSC525-Project

Navigate to the project directory:

    cd CPSC525-Project

Compile the Code using the Makefile:

    make

Run the Code:

    ./uaf

This allows you to interact with the program as a professor or a student.

Professor login     
username: Prof      
password: Prof123

Student login       
username: Mark      
password: Mark123       


More logins can be found at the start of the main function.

## How to Compile and Run the Exploit
Ensure the code is compiled using the same Makefile:

    make

Run the Exploit:

    ./attackcode

The exploit performs the following steps:

1. A professor logs in and submits grades for all students.
2. A student drops the class, freeing their memory.
3. The professor logs back in, enrolls a new student, and publishes the grades.
4. The new student observes that they inherited the grades of the dropped student due to the Use-After-Free vulnerability.