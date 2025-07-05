#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nasabah {
    char nama[50];
    char status[10]; // "VIP" atau "REGULER"
    int priority;    // 1 untuk VIP, 2 untuk REGULER
    struct Nasabah* left;
    struct Nasabah* right;
    int height;
} Nasabah;

typedef struct QueueNode {
    Nasabah* nasabah;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Fungsi AVL Tree
int height(Nasabah* node) {
    if (node == NULL) return 0;
    return node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Nasabah* createNode(char* nama, char* status) {
    Nasabah* newNode = (Nasabah*)malloc(sizeof(Nasabah));
    strcpy(newNode->nama, nama);
    strcpy(newNode->status, status);
    newNode->priority = (strcmp(status, "VIP") == 0) ? 1 : 2;
    newNode->left = newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

Nasabah* rightRotate(Nasabah* y) {
    Nasabah* x = y->left;
    Nasabah* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Nasabah* leftRotate(Nasabah* x) {
    Nasabah* y = x->right;
    Nasabah* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Nasabah* node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

Nasabah* insertAVL(Nasabah* node, char* nama, char* status) {
    if (node == NULL) return createNode(nama, status);

    if (strcmp(nama, node->nama) < 0)
        node->left = insertAVL(node->left, nama, status);
    else if (strcmp(nama, node->nama) > 0)
        node->right = insertAVL(node->right, nama, status);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && strcmp(nama, node->left->nama) < 0)
        return rightRotate(node);
    if (balance < -1 && strcmp(nama, node->right->nama) > 0)
        return leftRotate(node);
    if (balance > 1 && strcmp(nama, node->left->nama) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && strcmp(nama, node->right->nama) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, Nasabah* nasabah) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->nasabah = nasabah;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }
    q->rear->next = newNode;
    q->rear = newNode;
}

// Fungsi Pencarian Nasabah
Nasabah* searchAVL(Nasabah* root, char* nama) {
    if (root == NULL || strcmp(root->nama, nama) == 0)
        return root;

    if (strcmp(nama, root->nama) < 0)
        return searchAVL(root->left, nama);
    else
        return searchAVL(root->right, nama);
}

Nasabah* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    QueueNode* temp = q->front;
    Nasabah* nasabah = temp->nasabah;
    q->front = q->front->next;

    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return nasabah;
}

// Fungsi untuk Memperbarui File Setelah Dequeue
void updateFileAfterDequeue(Queue* queue, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Gagal membuka file untuk memperbarui data.\n");
        return;
    }
    QueueNode* temp = queue->front;
    while (temp != NULL) {
        fprintf(file, "%s %s\n", temp->nasabah->nama, temp->nasabah->status);
        temp = temp->next;
    }
    fclose(file);
}


void displayQueue(Queue* q) {
    QueueNode* temp = q->front;

    printf("+----------------------------+----------+\n");
    printf("|           Nama             |  Status  |\n");
    printf("+----------------------------+----------+\n");

    while (temp != NULL) {
        printf("| %-26s | %-8s |\n", temp->nasabah->nama, temp->nasabah->status);
        temp = temp->next;
    }

    printf("+----------------------------+----------+\n");
}


void saveToFile(Nasabah* root, FILE* file) {
    if (root == NULL) return;
    saveToFile(root->left, file);
    fprintf(file, "%s %s\n", root->nama, root->status);
    saveToFile(root->right, file);
}

void saveQueueToFile(Queue* queue, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Gagal membuka file untuk menyimpan data.\n");
        return;
    }
    QueueNode* temp = queue->front;
    while (temp != NULL) {
        fprintf(file, "%s %s\n", temp->nasabah->nama, temp->nasabah->status);
        temp = temp->next;
    }
    fclose(file);
    printf("Data berhasil disimpan ke file.\n");
}

void loadFromFile(Nasabah** root, Queue* queue, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File tidak ditemukan, membuat file baru.\n");
        return;
    }
    char nama[50], status[10];
    while (fscanf(file, "%s %s", nama, status) != EOF) {
        *root = insertAVL(*root, nama, status);
        enqueue(queue, createNode(nama, status));
    }
    fclose(file);
}

void addDummyData(Queue* queue, Nasabah** root) {
    const char* filename = "nasabah_data.txt";
    loadFromFile(root, queue, filename);
}

// Modifikasi Main Menu
void mainMenu() {
    Nasabah* rootAVL = NULL;
    Queue* nasabahQueue = createQueue();
    int choice, subChoice;
    char nama[50], status[10];

    addDummyData(nasabahQueue, &rootAVL);

    while (1) {
        printf("\n===== SELAMAT DATANG =====\n");
        printf("1. AVL\n2. BST\n3. Keluar\n");
        printf("Pilih Metode: ");
        scanf("%d", &choice);

        if (choice == 3) {
            printf("Terima kasih, sampai jumpa!\n");
            break;
        }

        if (choice == 1 || choice == 2) { // Menu AVL atau BST
            while (1) {
                printf("\n===== Menu %s =====\n", (choice == 1) ? "Metode Cepat" : "Metode Biasa");
                printf("1. Tambah Nasabah Baru\n2. Lihat Daftar Nasabah\n3. Proses Antrian Nasabah\n");
                printf("4. Cari Nasabah\n5. Kembali ke Menu Utama\n");
                printf("Pilih Menu: ");
                scanf("%d", &subChoice);

                if (subChoice == 5) {
                    printf("Kembali ke menu utama...\n");
                    break;
                }

                switch (subChoice) {
                    case 1:
                        printf("Masukkan Nama: ");
                        scanf("%s", nama);
                        printf("Masukkan Status (VIP/REGULER): ");
                        scanf("%s", status);
                        rootAVL = insertAVL(rootAVL, nama, status);
                        enqueue(nasabahQueue, createNode(nama, status));
                        saveQueueToFile(nasabahQueue, "nasabah_data.txt");
                        printf("Nasabah baru berhasil ditambahkan.\n");
                        break;

                    case 2:
                        printf("=== Daftar Nasabah ===\n");
                        displayQueue(nasabahQueue);
                        break;
                        
					case 3:
                        printf("=== Proses Antrian Nasabah ===\n");
                        Nasabah* served = dequeue(nasabahQueue);
                        if (served) {
                            printf("Nasabah yang dilayani:\nNama: %s, Status: %s\n", served->nama, served->status);
                            updateFileAfterDequeue(nasabahQueue, "nasabah_data.txt");
                            free(served);
                        } else {
                            printf("Antrian kosong.\n");
                        }
                        break;

                    case 4:
                        printf("Masukkan Nama Nasabah yang Dicari: ");
                        scanf("%s", nama);
                        Nasabah* found = searchAVL(rootAVL, nama);
                        if (found) {
                            printf("Nasabah ditemukan:\nNama: %s, Status: %s\n", found->nama, found->status);
                        } else {
                            printf("Nasabah dengan nama %s tidak ditemukan.\n", nama);
                        }
                        break;

                    default:
                        printf("Pilihan tidak valid. Silakan coba lagi.\n");
                }
            }
        } else {
            printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
