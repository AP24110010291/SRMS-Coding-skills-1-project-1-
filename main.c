#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

/* Function Declarations */
int loginSystem();
void mainMenu();
void adminMenu();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

/* main function */
int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nLogin Failed. Exiting...\n");
    }
    return 0;
}

/* Login System */
int loginSystem() {
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[10];

    printf("============== Login Screen ==============\n");
    printf("Username: ");
    scanf("%19s", username);
    printf("Password: ");
    scanf("%19s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: credentials file not found!\n");
        return 0;
    }

    while (fscanf(fp, "%19s %19s %9s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);
            fclose(fp);
            printf("\nWelcome %s (%s)\n", currentUser, currentRole);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/* Main Menu */
void mainMenu() {
    if (strcmp(currentRole, "Admin") == 0)
        adminMenu();
    else
        printf("\nOnly Admin role is supported in this system.\n");
}

/* Admin Menu */
void adminMenu() {
    int choice;
    do {
        printf("\n======== Admin Menu ========\n");
        printf("1. Add New Student\n");
        printf("2. Display All Records\n");
        printf("3. Search Record\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student Record\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Please enter a number 1-6.\n");
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging Out...\n"); return;
            default: printf("Invalid Choice. Try again.\n");
        }
    } while (1);
}

void addStudent() {
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        printf("Error opening student file.\n");
        return;
    }

    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);
    printf("Enter Name (single word): ");
    scanf("%49s", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("Student added successfully.\n");
}

void displayStudents() {
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No records available.\n");
        return;
    }

    printf("\n----- Student Records -----\n");
    printf("Roll\tName\tMarks\n");

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("%d\t%s\t%.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
}

void searchStudent() {
    int rollNo, found = 0;
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    printf("\nEnter Roll Number to search: ");
    scanf("%d", &rollNo);

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == rollNo) {
            printf("\nRecord Found:\n");
            printf("Roll : %d\n", s.roll);
            printf("Name : %s\n", s.name);
            printf("Marks: %.2f\n", s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Record not found.\n");

    fclose(fp);
}

void updateStudent() {
    int rollNo, found = 0;
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter Roll Number to update: ");
    scanf("%d", &rollNo);

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == rollNo) {
            found = 1;
            printf("Enter new name: ");
            scanf("%49s", s.name);
            printf("Enter new marks: ");
            scanf("%f", &s.marks);
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Record not found!\n");
        remove("temp.txt");
    } else {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("Record updated successfully.\n");
    }
}

void deleteStudent() {
    int rollNo, found = 0;
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter Roll Number to delete: ");
    scanf("%d", &rollNo);

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == rollNo) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Record not found.\n");
        remove("temp.txt");
    } else {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("Record deleted successfully.\n");
    }
}
