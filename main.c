#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

typedef struct User {
    int age;
    char name[100];
    double salary;
    struct User* next;
} User;

void print(User* user) {
    printf("\t%s\t|\t%d\t|\t%.2f\t|\n", user->name, user->age, user->salary);
}

User* User_Create(char* name, int age, double salary) {
    User* user = malloc(sizeof(User));
    strcpy(user->name, name);
    user->age = age;
    user->salary = salary;
    user->next = NULL;
    return user;
}

User* List_push(User* root, User* newUser) {
    if (root == NULL) {
        return newUser;
    }
    User* tmpUser = root;
    while (tmpUser->next != NULL) {
        tmpUser = tmpUser->next;
    }
    tmpUser->next = newUser;
    return root;
}

User* List_show(User* root) {
    if (root == NULL) {
        printf("List is empty!\n");
        return root;
    }
    int i = 0;
    printf("|-----------------------------------------------------------------------|\n");
    printf("|\tINDEX\t|\tNAME\t|\tAGE\t|\tSALARY\t\t|\n");
    printf("|---------------|---------------|---------------|-----------------------|\n");
    while (root != NULL) {
        printf("|\t[%d]\t|", i);
        print(root);
        root = root->next;
        i++;
    }
    return root;
}

void User_delete(User* user) {
    free(user);
}

User* List_remove(User* root, int ind) {
    if (root == NULL) {
        return root;
    }
    if (ind == 0) {
        User* next = root->next;
        User_delete(root);
        return next;
    }
    User* prev = root;
    User* current = root;
    int i = 0;
    while (current != NULL) {
        if (ind == i) {
            prev->next = current->next;
            User_delete(current);
            break;
        }
        prev = current;
        current = current->next;
        i++;
    }
    return root;
}

User* Menu_createUser(User* root) {
    User tmp;
    printf("Input name:");
    scanf("%s", &tmp.name);
    printf("Input age:");
    scanf("%d", &tmp.age);
    printf("Input salary:");
    scanf("%lf", &tmp.salary);
    root = List_push(root, User_Create(tmp.name, tmp.age, tmp.salary));
    return root;
}

int Show_menu() {
    printf("MENU:\n");
    printf("%d. Print all users\n", 1);
    printf("%d. Add new user\n", 2);
    printf("%d. Delete user\n", 3);
    printf("%d. Get count of users\n", 4);
    printf("%d. Find by salary\n", 5);
    printf("%d. Find substring\n", 6);
    printf("%d. Sort by salary\n", 7);
    printf("%d. Sort by name\n", 8);
    printf("%d. Exit\n", 9);
    printf("Input action [1-9]:\n");
    int actionId;
    scanf("%d", &actionId);
    return actionId;
}

int fileLen(FILE* file) {
    int counter = 0;
    char name[100];
    int age = 0;
    double salary = 0;
    while (fscanf(file, "%s %d %f", name, &age, &salary) != EOF) {
        counter++;
    }
    fseek(file, 0, SEEK_SET);
    return counter;
}

User* Init_data(User* root) {
    FILE* file = NULL;
    file = fopen("load.txt", "r");
    if (!file) {
        printf("Cant open file\n");
        fclose(file);
        return 0;
    }
    else {
        printf("\nOpening file...\n");
        printf("Number of records = %d\n", fileLen(file));
        int age;
        char name[100];
        double salary;
        while (fscanf(file, "%s%d%lf", &name, &age, &salary) != EOF) {
            User tmp;
            strcpy(tmp.name, name);
            tmp.age = age;
            tmp.salary = salary;
            root = List_push(root, User_Create(tmp.name, tmp.age, tmp.salary));
        }
        return root;
        fclose(file);
    }
}

int List_len(User* root) {
    int counter = 0;
    while (root != NULL) {
        root = root->next;
        counter++;
    }
    return counter;
}

User* Find_by_salary(User* root, int start, int finish) {
    int i = 0;
    printf("|-----------------------------------------------------------------------|\n");
    printf("|\tINDEX\t|\tNAME\t|\tAGE\t|\tSALARY\t\t|\n");
    printf("|---------------|---------------|---------------|-----------------------|\n");
    while (root != NULL) {
        if (root->salary >= start && root->salary <= finish) {
            printf("|\t[%d]\t|", i);
            print(root);
            i++;
        }
        root = root->next;
    }
    return root;
}

User* Find_by_substr(User* root, char* substr) {
    char* istr;
    int i = 0;
    printf("|-----------------------------------------------------------------------|\n");
    printf("|\tINDEX\t|\tNAME\t|\tAGE\t|\tSALARY\t\t|\n");
    printf("|---------------|---------------|---------------|-----------------------|\n");
    while (root != NULL) {
        istr = strstr(root->name, substr);
        if (istr != NULL) {
            printf("|\t[%d]\t|", i);
            print(root);
            i++;
        }
        root = root->next;
    }
    return root;
}

void write_to_file(User* root, int size) {
    FILE* fileW = NULL;
    fileW = fopen("save.txt", "w");
    for (int i = 0; i < size; i++) {
        fprintf(fileW, "%s\t%d\t%lf\n", root->name, root->age, root->salary);
        root = root->next;
    }
    fclose(fileW);
}

User* Sort_by_salary_increase(User* root) {

    User* new_root = NULL;
    while (root != NULL) {
        User* node = root;
        root = root->next;

        if (new_root == NULL || node->salary < new_root->salary) {
            node->next = new_root;
            new_root = node;
        }
        else {
            User* current = new_root;
            while (current->next != NULL && !(node->salary < current->next->salary)) {
                current = current->next;
            }
            node->next = current->next;
            current->next = node;
        }
    }
    return new_root;
}

User* Sort_by_salary_descending(User* root) {

    User* new_root = NULL;
    while (root != NULL) {
        User* node = root;
        root = root->next;
        if (new_root == NULL || node->salary > new_root->salary) {
            node->next = new_root;
            new_root = node;
        }
        else {
            User* current = new_root;
            while (current->next != NULL && (node->salary > current->next->salary)) {
                current = current->next;
            }
            node->next = current->next;
            current->next = node;
        }
    }
    return new_root;
}

User* Sort_by_name_increase(User* root) {

    User* new_root = NULL;
    while (root != NULL) {
        User* node = root;
        root = root->next;

        if (new_root == NULL || (strcmp(node->name, new_root->name) < 0)) {
            node->next = new_root;
            new_root = node;
        }
        else {
            User* current = new_root;
            while (current->next != NULL && !(strcmp(node->name, current->next->name) < 0)) {
                current = current->next;
            }
            node->next = current->next;
            current->next = node;
        }
    }
    return new_root;
}

User* Sort_by_name_descending(User* root) {

    User* new_root = NULL;
    while (root != NULL) {
        User* node = root;
        root = root->next;

        if (new_root == NULL || (strcmp(node->name, new_root->name) > 0)) {
            node->next = new_root;
            new_root = node;
        }
        else {
            User* current = new_root;
            while (current->next != NULL && !(strcmp(node->name, current->next->name) > 0)) {
                current = current->next;
            }
            node->next = current->next;
            current->next = node;
        }
    }
    return new_root;
}

int main() {
    User* rootUser = NULL;
    rootUser = Init_data(rootUser);
    int enLoop = 1;
    while (enLoop) {
        int action = Show_menu();
        switch (action) {
        case 1: {
            List_show(rootUser);
        }break;
        case 2: {
            rootUser = Menu_createUser(rootUser);
        }break;
        case 3: {
            printf("Enter user index:");
            int index = 0;
            scanf("%d", &index);
            rootUser = List_remove(rootUser, index);
        }break;
        case 4: {
            printf("Number of users: %i\n", List_len(rootUser));
        }break;
        case 5: {
            printf("Enter start salary:");
            int start = 0;
            scanf("%d", &start);
            printf("Enter finish salary:");
            int finish = 0;
            scanf("%d", &finish);
            Find_by_salary(rootUser, start, finish);
        }break;
        case 6: {
            printf("Enter substring:");
            char substr[64];
            scanf("%s", &substr);
            Find_by_substr(rootUser, substr);
        }break;
        case 7: {
            printf("Sort by:\n");
            printf("%d. Ascending\n", 1);
            printf("%d. Descending\n", 2);
            printf("Input action [1-2]:\n");
            int index = 0;
            scanf("%d", &index);
            if (index == 1) {
                rootUser = Sort_by_salary_increase(rootUser);
            }
            else if (index == 2) {
                rootUser = Sort_by_salary_descending(rootUser);
            }
            List_show(rootUser);
        }break;
        case 8: {
            printf("Sort by:\n");
            printf("%d. Ascending\n", 1);
            printf("%d. Descending\n", 2);
            printf("Input action [1-2]:\n");
            int index = 0;
            scanf("%d", &index);
            if (index == 1) {
                rootUser = Sort_by_name_increase(rootUser);
            }
            else if (index == 2) {
                rootUser = Sort_by_name_descending(rootUser);
            }
            List_show(rootUser);
        }break;
        case 9: {
            printf("Good Bye!");
            write_to_file(rootUser, List_len(rootUser));
            enLoop = 0;
        }break;
        }
    }
    return 0;
}
