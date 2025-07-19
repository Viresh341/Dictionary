#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char word[100];
    char meaning[256];
    struct Node *left, *right;
} Node;

// Function prototypes
Node* createNode(char* word, char* meaning);
Node* insert(Node* root, char* word, char* meaning);
Node* search(Node* root, char* word);
Node* deleteNode(Node* root, char* word);
Node* findMinimum(Node* root);
void displayInOrder(Node* root);
void trimNewline(char* str);

int main() {
    Node* root = NULL;
    FILE *fp = fopen("words.txt", "r");
    char buffer[356];

    // Read words and meanings from file and insert into the BST
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            char *word = strtok(buffer, "-");
            char *meaning = strtok(NULL, "\n");
            if (word && meaning) {
                word[strcspn(word, "\n")] = 0; // Remove newline characters
                meaning[strcspn(meaning, "\n")] = 0;
                root = insert(root, word, meaning);
            }
        }
        fclose(fp);
    } else {
        printf("Failed to open the file.\n");
        return 1;
    }

    int choice;
    char word[100], meaning[256];

    // Menu for user interaction
    do {
        printf("\n1. Insert Word\n2. Search Word\n3. Delete Word\n4. Display Dictionary\n5. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character

        switch(choice) {
            case 1:
                printf("Enter word: ");
                fgets(word, 100, stdin);
                trimNewline(word);
                printf("Enter meaning: ");
                fgets(meaning, 256, stdin);
                trimNewline(meaning);
                root = insert(root, word, meaning);
                break;
            case 2:
                printf("Enter word to search: ");
                fgets(word, 100, stdin);
                trimNewline(word);
                Node* found = search(root, word);
                if (found != NULL) printf("Meaning: %s\n", found->meaning);
                else printf("Word not found.\n");
                break;
            case 3:
                printf("Enter word to delete: ");
                fgets(word, 100, stdin);
                trimNewline(word);
                root = deleteNode(root, word);
                break;
            case 4:
                printf("Dictionary:\n");
                displayInOrder(root);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 5);

    return 0;
}

Node* createNode(char* word, char* meaning) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        strcpy(newNode->word, word);
        strcpy(newNode->meaning, meaning);
        newNode->left = newNode->right = NULL;
    }
    return newNode;
}

Node* insert(Node* root, char* word, char* meaning) {
    if (root == NULL) return createNode(word, meaning);

    int compare = strcmp(word, root->word);
    if (compare < 0) root->left = insert(root->left, word, meaning);
    else if (compare > 0) root->right = insert(root->right, word, meaning);

    return root;
}

Node* search(Node* root, char* word) {
    if (root == NULL || strcmp(root->word, word) == 0) return root;

    if (strcmp(word, root->word) < 0) return search(root->left, word);
    else return search(root->right, word);
}

Node* findMinimum(Node* root) {
    while (root->left != NULL) root = root->left;
    return root;
}

Node* deleteNode(Node* root, char* word) {
    if (root == NULL) return root;

    int compare = strcmp(word, root->word);
    if (compare < 0) {
        root->left = deleteNode(root->left, word);
    } else if (compare > 0) {
        root->right = deleteNode(root->right, word);
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

        Node* temp = findMinimum(root->right);
        strcpy(root->word, temp->word);
        strcpy(root->meaning, temp->meaning);
        root->right = deleteNode(root->right, temp->word);
    }
    return root;
}

void displayInOrder(Node* root) {
    if (root != NULL) {
        displayInOrder(root->left);
        printf("%s - %s\n", root->word, root->meaning);
        displayInOrder(root->right);
    }
}

void trimNewline(char* str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}
