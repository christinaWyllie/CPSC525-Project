#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>

// User structure to store user credentials, role, and grades
struct User
{
    char username[51]; // Max 50 Characters
    char password[51];
    bool prof; // Flag to indicate if user is a professor
    std::vector<char *> Grades;
};

// Structure to store multiple grades for students.
struct student_grades
{
    User *student; // Pointer to the student.
    char grade[101];
};

// Adds a new user
void addUser(std::vector<User *> &users, const char *username, const char *password, bool prof)
{
    User *newUser = (User *)malloc(sizeof(User));
    *newUser = (User){{}, {}, prof, {}};
    snprintf(newUser->username, sizeof(newUser->username), "%s", username);
    snprintf(newUser->password, sizeof(newUser->password), "%s", password);
    users.push_back(newUser); // Add the user to master list
}

// Validate user credentials and return a pointer to the user in the master list
User *loginUser(std::vector<User *> &users, const char *username, const char *password)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i] == nullptr)
        {
            continue;
        }
        if (strcmp(users[i]->username, username) == 0 && strcmp(users[i]->password, password) == 0)
        {
            printf("     Login successful! Welcome, %s.\n", username);
            if (users[i]->prof)
            {
                printf("     You are logged in as a Professor.\n");
            }
            else
            {
                printf("     You are logged in as a Student.\n");
            }
            return users[i];
        }
    }
    printf("     Login failed. Incorrect username or password.\n");
    return nullptr;
}

// Display the grades for a student
void checkGrades(User *student)
{
    if (student->Grades.empty())
    {
        printf("     No grades have been posted yet.\n");
    }

    else
    {
        printf("     Your grades:\n");
        for (size_t i = 0; i < student->Grades.size(); i++)
        {
            printf("     - %s\n", student->Grades[i]);
        }
    }
}

// Remove the student from the system and frees associated memory - this is where the use after free begins.
void dropClass(std::vector<User *> &users, User *student)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i] == student)
        {
            printf("     Dropping %s from the class\n", student->username);

            for (size_t j = 0; j < student->Grades.size(); j++)
            {
                free(student->Grades[j]); // Free each grade string
            }
            student->Grades.clear(); // Clear the grade vector

            free(users[i]);     // Free the user structure - this is the cause of the use after free.
            users[i] = nullptr; // Remove user from the master list
            printf("     You have been removed from the class.\n");
            return;
        }
    }
    printf("     Error: Student not found.\n");
}

// Allows the professor to submit grades for all students without having to publish them.
void submitGrades(std::vector<User *> &users, std::vector<student_grades> &tempGrades, char *currentAssignment)
{
    // If some grades already exist, ask to override
    if (!tempGrades.empty())
    {

        printf("Grades for assignment '%s' already exist.\n", currentAssignment);
        printf("Do you want to override them? (yes/no): ");

        char confirmation[4];
        while (1)
        {
            if (scanf("%3s", confirmation) != 1)
            {
                printf("     Invalid input. Please try again.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }
            if (getchar() != '\n')
            {
                printf("     Invalid input. Please enter 'yes' or 'no'.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }

            if (strcmp(confirmation, "yes") == 0)
            {
                tempGrades.clear();
                break;
            }
            else if (strcmp(confirmation, "no") == 0)
            {
                printf("     Grades submission canceled.\n");
                return;
            }
            else
            {
                printf("     Invalid input. Please enter 'yes' or 'no'.\n");
                while (getchar() != '\n')
                    ;
            }
        }
    }

    printf("\nSubmit Grades\n");
    tempGrades.clear();
    printf("Enter assignment name: ");

    while (1)
    {
        printf("Enter assignment name (max 50 characters): ");

        if (scanf("%50s", currentAssignment) != 1)
        {
            printf("     Invalid input. Please try again.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (getchar() != '\n')
        {
            printf("     Input too long. Please enter up to 50 characters.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        break;
    }
    // Collect grades for each student in the master list.
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i] != nullptr && !users[i]->prof)
        {
            printf("Enter grade for %s (0-100): ", users[i]->username);
            int numeGrade;

            while (1)
            {
                if (scanf("%d", &numeGrade) != 1)
                {
                    printf("     Invalid input. Please enter a valid number between 0 and 100.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }

                if (numeGrade < 0 || numeGrade > 100)
                {
                    printf("     Invalid grade. Please enter a number between 0 and 100.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }

                break;
            }

            student_grades temp;
            temp.student = users[i];
            snprintf(temp.grade, sizeof(temp.grade), "On %s, %s scored %d%%.", currentAssignment, users[i]->username, numeGrade);
            /*
                Push the grade to a tempGrades vector, which stores a pointer to the user
                associated with the grade. This design can lead to a Use-After-Free (UAF) 
                vulnerability because when a user is deleted, the pointer in tempGrades is 
                not cleared. If a new user with the same memory size is added, the dangling
                pointer in tempGrades may now point to the new user. This flaw allows 
                publishGrades to assign grades meant for the deleted user to the new user.
            */
            tempGrades.push_back(temp);
        }
    }
    printf("     Grades for assignment '%s' submitted successfully.\n", currentAssignment);
}

//Allows a professor to publish grades
void publishGrades(std::vector<student_grades> &tempGrades, const char *currentAssignment)
{

    if (tempGrades.empty())
    {
        printf("     No grades to publish. Submit grades first.\n");
        return;
    }

    char confirmation[4];

    while (1)
    {
        printf("Do you want to publish grades for assignment '%s'? (yes/no): ", currentAssignment);

        if (scanf("%3s", confirmation) != 1)
        {
            printf("     Invalid input. Please try again.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (strcmp(confirmation, "yes") == 0 || strcmp(confirmation, "no") == 0)
        {
            break;
        }
        else
        {
            printf("     Invalid input. Please enter 'yes' or 'no'.\n");
            while (getchar() != '\n')
                ;
        }
    }

    if (strcmp(confirmation, "yes") == 0)
    {
        for (size_t i = 0; i < tempGrades.size(); i++)
        {
            char *gradeEntry = (char *)malloc(strlen(tempGrades[i].grade) + 1);
            strcpy(gradeEntry, tempGrades[i].grade);
            /*
            Here if the tempGrades pointer points to the new user while having the grades
            for a deleted user, it will publish the grades without any error checking. This is a
            Use-After-Free (UAF).            
            */
            tempGrades[i].student->Grades.push_back(gradeEntry);  
        }
        printf("     Grades for assignment '%s' published successfully.\n", currentAssignment);
        tempGrades.clear();
    }
    else
    {
        printf("     Publishing grades for '%s' canceled.\n", currentAssignment);
    }
}
//Allows the professor to enroll a new student
void enrollStudent(std::vector<User *> &users)
{
    char newUsername[51];
    char newPassword[51];

    printf("Enroll New Student\n");

    while (1)
    {
        printf("Enter new student's username (max 50 characters): ");

        if (scanf("%50s", newUsername) != 1)
        {
            printf("     Invalid input. Please try again.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (getchar() != '\n')
        {
            printf("     Input too long. Please enter up to 50 characters.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        break;
    }

    while (1)
    {
        printf("Enter new student's password (max 50 characters): ");
        if (scanf("%50s", newPassword) != 1)
        {
            printf("     Invalid input. Please try again.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (getchar() != '\n')
        {
            printf("     Input too long. Please enter up to 50 characters.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        break;
    }

    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i] != nullptr && strcmp(users[i]->username, newUsername) == 0)
        {
            printf("     Error: Username '%s' is already taken. Please try again.\n", newUsername);
            return;
        }
    }

    addUser(users, newUsername, newPassword, false);
    printf("Student '%s' has been successfully enrolled.\n", newUsername);
}

int main()
{
    std::vector<User *> users;

    std::vector<student_grades> tempGrades;
    char currentAssignment[50];

    addUser(users, "Prof", "Prof123", true);
    addUser(users, "Joe", "Joe123", false);
    addUser(users, "Mark", "Mark123", false);
    addUser(users, "Bob", "Bob123", false);
    addUser(users, "Henry", "Henry123", false);
    addUser(users, "Cody", "Cody123", false);

    while (true)
    {
        char login_username[51];
        char login_password[51];
        User *currentUser = nullptr;

        while (!currentUser)
        {
            printf("------------------------------------------\n");
            printf("     Welcome to CPSC 525 HomePage\n");
            printf("------------------------------------------\n");
            printf("\nLogin Page\n");

            while (1)
            {
                printf("Enter username (max 50 characters): ");

                if (scanf("%50s", login_username) != 1)
                {
                    printf("     Invalid input. Please try again.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }

                if (getchar() != '\n')
                {
                    printf("     Input too long. Please enter up to 50 characters.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }

                break;
            }

            while (1)
            {
                printf("Enter password (max 50 characters): ");
                if (scanf("%50s", login_password) != 1)
                {
                    printf("     Invalid input. Please try again.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }

                if (getchar() != '\n')
                {
                    printf("     Input too long. Please enter up to 50 characters.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }
                break;
            }

            currentUser = loginUser(users, login_username, login_password);
        }

        printf("\n------------------------------------------\n");
        printf("Current User: %s\n", currentUser->username);

        if (currentUser->prof)
        {

            int choice = 0;
            do
            {

                printf("------------------------------------------\n");
                printf("Professor Actions\n");
                printf("1. Submit grades\n");
                printf("2. Publish grades\n");
                printf("3. Enroll a new student\n");
                printf("4. Logout\n");
                printf("Enter the number corresponding to your choice: ");

                if (scanf("%d", &choice) != 1)
                {
                    printf("     Invalid input. Please try again.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }

                switch (choice)
                {
                case 1:

                    printf("\n------------------------------------------\n");
                    submitGrades(users, tempGrades, currentAssignment);
                    break;
                case 2:

                    printf("\n------------------------------------------\n");
                    publishGrades(tempGrades, currentAssignment);
                    break;
                case 3:

                    printf("\n------------------------------------------\n");
                    enrollStudent(users);
                    break;
                case 4:

                    printf("\n------------------------------------------\n");
                    printf("     Logging out.\n");
                    currentUser = nullptr;
                    break;
                default:
                    printf("     Invalid option. Please enter a valid number.\n");
                    break;
                }
            } while (currentUser);
        }
        else
        {

            int choice = 0;
            do
            {

                printf("------------------------------------------\n");
                printf("Student Actions\n");
                printf("1. Check grades\n");
                printf("2. Drop class\n");
                printf("3. Logout\n");
                printf("Enter the number corresponding to your choice: ");

                if (scanf("%d", &choice) != 1)
                {
                    printf("     Invalid input. Please try again.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }

                switch (choice)
                {
                case 1:
                    printf("\n------------------------------------------\n");
                    checkGrades(currentUser);
                    break;
                case 2:
                    printf("\n------------------------------------------\n");
                    dropClass(users, currentUser);
                    currentUser = nullptr;
                    break;
                case 3:
                    printf("\n------------------------------------------\n");
                    printf("     Logging out.\n");
                    currentUser = nullptr;
                    break;
                default:
                    printf("\n------------------------------------------\n");
                    printf("     Invalid option. Please enter a valid number.\n");
                    break;
                }
            } while (currentUser);
        }
        printf("------------------------------------------\n");

        printf("\n     Returning to the main login page.\n");
    }

    return 0;
}
