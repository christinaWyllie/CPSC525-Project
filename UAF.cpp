#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>

struct User {
    char username[50]; 
    char password[50];
    bool prof;
    std::vector<char*> Grades; 
};

struct student_grades {
    User* student;     
    char grade[100];  
};

void addUser(std::vector<User*> &users, const char* username, const char* password, bool prof) {
    User* newUser = (User*)malloc(sizeof(User));
    *newUser = (User){{},{}, prof, {}};
    snprintf(newUser->username, sizeof(newUser->username), "%s", username);
    snprintf(newUser->password, sizeof(newUser->password), "%s", password);
    users.push_back(newUser);
}

User* loginUser(std::vector<User*> &users, const char* username, const char* password) {
    for (size_t i = 0; i < users.size(); i++) {
        if(users[i] == nullptr){
            continue;
        }
        if (strcmp(users[i]->username, username) == 0 && strcmp(users[i]->password, password) == 0) {
            printf("Login successful! Welcome, %s.\n", username);
            if (users[i]->prof) {
                printf("You are logged in as a Professor.\n");
            } else {
                printf("You are logged in as a Student.\n");
            }
            return users[i];
        }
    }
    printf("Login failed. Incorrect username or password.\n");
    return nullptr;
}

void checkGrades(User* student) {
    if (student->Grades.empty()) {
        printf("No grades have been posted yet.\n");
    } else {
        printf("Your grades:\n");
        for (size_t i = 0; i < student->Grades.size(); i++) {
            printf("- %s\n", student->Grades[i]);
        }
    }
}

void dropClass(std::vector<User*>& users, User* student) {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i] == student) {
            printf("Dropping %s from the class\n", student->username);

            for (size_t j = 0; j < student->Grades.size(); j++) {
                free(student->Grades[j]);
            }
            student->Grades.clear(); 

            free(users[i]); 
            users[i] = nullptr; 
            printf("You have been removed from the class.\n");
            return;
        }
    }
    printf("Error: Student not found.\n");
}


void submitGrades(std::vector<User*>& users, std::vector<student_grades>& tempGrades, char* currentAssignment) {
    printf("\nSubmit Grades\n");
    tempGrades.clear();
    printf("Enter assignment name: ");
    scanf("%49s", currentAssignment);

    for (size_t i = 0; i < users.size(); i++) {
        if (users[i] != nullptr && !users[i]->prof) {
            printf("Enter grade for %s: ", users[i]->username);
            int numeGrade;
            scanf("%d", &numeGrade);

            student_grades temp;
            temp.student = users[i]; 
            snprintf(temp.grade, sizeof(temp.grade), "On %s, %s scored %d%%.", currentAssignment, users[i]->username, numeGrade);
            tempGrades.push_back(temp);
        }
    }
    printf("Grades for assignment '%s' submitted successfully.\n", currentAssignment);
}


void publishGrades(const std::vector<student_grades>& tempGrades, const char* currentAssignment) {
    if (tempGrades.empty()) {
        printf("No grades to publish. Submit grades first.\n");
        return;
    }

    printf("\nPublish Grades\n");
    printf("Do you want to publish grades for assignment '%s'? (yes/no): ", currentAssignment);
    char confirmation[4];
    scanf("%3s", confirmation);

    if (strcmp(confirmation, "yes") == 0) {
        for (size_t i = 0; i < tempGrades.size(); i++) {
            char* gradeEntry = (char*)malloc(strlen(tempGrades[i].grade) + 1);
            strcpy(gradeEntry, tempGrades[i].grade);
            tempGrades[i].student->Grades.push_back(gradeEntry); 
        }
        printf("Grades for assignment '%s' published successfully.\n", currentAssignment);
    } else {
        printf("Publishing grades for '%s' canceled.\n", currentAssignment);
    }
}



void enrollStudent(std::vector<User*>& users) {
    char newUsername[50];
    char newPassword[50];

    printf("\nEnroll New Student\n");
    printf("Enter new student's username: ");
    scanf("%49s", newUsername);
    printf("Enter new student's password: ");
    scanf("%49s", newPassword);

    for (size_t i = 0; i < users.size(); i++) {
        if (users[i] != nullptr && strcmp(users[i]->username, newUsername) == 0) {
            printf("Error: Username '%s' is already taken. Please try again.\n", newUsername);
            return;
        }
    }

    addUser(users, newUsername, newPassword, false);
    printf("Student '%s' has been successfully enrolled.\n", newUsername);
}

int main() {
    std::vector<User*> users;

    std::vector<student_grades> tempGrades;
    char currentAssignment[50];

    addUser(users, "Prof", "Prof123", true);
    addUser(users, "Joe", "Joe123", false);
    addUser(users, "Mark", "Mark123", false);
    addUser(users, "Bob", "Bob123", false);
    addUser(users, "Henry", "Henry123", false);
    addUser(users, "Cody", "Cody123", false);

    while (true) { 
        char login_username[50];
        char login_password[50];
        User* currentUser = nullptr;

        while (!currentUser) {
            printf("\nWelcome to CPSC 525 HomePage\n");
            printf("\nLogin Page\n");
            printf("Enter username: ");
            scanf("%49s", login_username);
            printf("Enter password: ");
            scanf("%49s", login_password);

            currentUser = loginUser(users, login_username, login_password);

            if (!currentUser) {
                printf("Login failed. Please try again.\n");
            }
        }

        printf("\nCurrent User: %s\n", currentUser->username);
        if (currentUser->prof) {

            int choice = 0;
            do {
                printf("\nProfessor Actions\n");
                printf("1. Submit grades\n");
                printf("2. Publish grades\n");
                printf("3. Enroll a new student\n");
                printf("4. Logout\n");
                printf("Enter the number corresponding to your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        submitGrades(users, tempGrades, currentAssignment);
                        break;
                    case 2:
                        publishGrades(tempGrades, currentAssignment);
                        break;
                    case 3:
                        enrollStudent(users);
                        break;
                    case 4:
                        printf("Logging out.\n");
                        currentUser = nullptr;
                        break;
                    default:
                        printf("Invalid option. Please enter a valid number.\n");
                        break;
                }
            } while (currentUser);
        } else {

            int choice = 0;
            do {
                printf("\nStudent Actions\n");
                printf("1. Check grades\n");
                printf("2. Drop class\n");
                printf("3. Logout\n");
                printf("Enter the number corresponding to your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        checkGrades(currentUser);
                        break;
                    case 2:
                        dropClass(users, currentUser);
                        currentUser = nullptr;
                        break;
                    case 3:
                        printf("Logging out.\n");
                        currentUser = nullptr;
                        break;
                    default:
                        printf("Invalid option. Please enter a valid number.\n");
                        break;
                }
            } while (currentUser);
        }

        printf("\nReturning to the main login page.\n");
    }

    return 0;
}
