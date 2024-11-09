#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_WORK_LENGTH 50
#define MAX_POSITION_LENGTH 50
#define MAX_PHONE_LENGTH 15
#define MAX_EMAIL_LENGTH 50
#define MAX_SOCIAL_LENGTH 100

typedef struct {
    char first_name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char work[MAX_WORK_LENGTH];
    char position[MAX_POSITION_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char social[MAX_SOCIAL_LENGTH];
} Contact;

typedef struct Node {
    Contact contact;
    struct Node* left;
    struct Node* right;
} Node;

Node* root = NULL;

// Создание нового узла
Node* create_node(Contact contact) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->contact = contact;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Вставка узла в дерево
Node* insert(Node* root, Contact contact) {
    if (root == NULL) {
        return create_node(contact);
    }
    if (strcmp(contact.surname, root->contact.surname) < 0 ||
        (strcmp(contact.surname, root->contact.surname) == 0 &&
         strcmp(contact.first_name, root->contact.first_name) < 0)) {
        root->left = insert(root->left, contact);
    } else {
        root->right = insert(root->right, contact);
    }
    return root;
}

// Поиск узла по имени и фамилии
Node* search(Node* root, char* first_name, char* surname) {
    if (root == NULL || (strcmp(root->contact.first_name, first_name) == 0 &&
                         strcmp(root->contact.surname, surname) == 0)) {
        return root;
    }
    if (strcmp(surname, root->contact.surname) < 0 ||
        (strcmp(surname, root->contact.surname) == 0 &&
         strcmp(first_name, root->contact.first_name) < 0)) {
        return search(root->left, first_name, surname);
    }
    return search(root->right, first_name, surname);
}

// Минимальный узел
Node* find_min(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Удаление узла
Node* delete_node(Node* root, char* first_name, char* surname) {
    if (root == NULL) {
        return root;
    }
    if (strcmp(surname, root->contact.surname) < 0 ||
        (strcmp(surname, root->contact.surname) == 0 &&
         strcmp(first_name, root->contact.first_name) < 0)) {
        root->left = delete_node(root->left, first_name, surname);
    } else if (strcmp(surname, root->contact.surname) > 0 ||
               (strcmp(surname, root->contact.surname) == 0 &&
                strcmp(first_name, root->contact.first_name) > 0)) {
        root->right = delete_node(root->right, first_name, surname);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = find_min(root->right);
        root->contact = temp->contact;
        root->right = delete_node(root->right, temp->contact.first_name, temp->contact.surname);
    }
    return root;
}

// Обход дерева для вывода контактов
void inorder(Node* root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    printf("Name: %s, Surname: %s\n", root->contact.first_name, root->contact.surname);
    inorder(root->right);
}

// Сбор контактов в массив
void tree_to_array(Node* root, Contact* array, int* index) {
    if (root == NULL) {
        return;
    }
    tree_to_array(root->left, array, index);
    array[(*index)++] = root->contact;
    tree_to_array(root->right, array, index);
}

// Построение сбалансированного дерева из массива
Node* array_to_tree(Contact* array, int start, int end) {
    if (start > end) {
        return NULL;
    }
    int mid = (start + end) / 2;
    Node* root = create_node(array[mid]);
    root->left = array_to_tree(array, start, mid - 1);
    root->right = array_to_tree(array, mid + 1, end);
    return root;
}

// Балансировка дерева
Node* balance_tree(Node* root) {
    Contact array[100];
    int index = 0;
    tree_to_array(root, array, &index);
    return array_to_tree(array, 0, index - 1);
}

// Добавление контакта
void add_contact() {
    Contact contact;
    printf("Enter name: ");
    scanf("%s", contact.first_name);
    printf("Enter surname: ");
    scanf("%s", contact.surname);
    // Другие поля аналогично
    root = insert(root, contact);
}

// Удаление контакта
void delete_contact() {
    char first_name[MAX_NAME_LENGTH], surname[MAX_NAME_LENGTH];
    printf("Enter name: ");
    scanf("%s", first_name);
    printf("Enter surname: ");
    scanf("%s", surname);
    root = delete_node(root, first_name, surname);
}

// Вывод контактов
void list_contacts() {
    inorder(root);
}

// Главная функция
int main() {
    int choice;
    while (1) {
        printf("1. Add contact\n2. Delete contact\n3. List contacts\n4. Balance tree\n5. Exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                add_contact();
                break;
            case 2:
                delete_contact();
                break;
            case 3:
                list_contacts();
                break;
            case 4:
                root = balance_tree(root);
                printf("Tree balanced.\n");
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
