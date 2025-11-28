#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Symbolic constants
#define MAX_BUKU 100
#define MAX_NAMA 50
#define MAX_JUDUL 100
#define MAX_KODE 20

// Macro dengan argumen untuk menghitung poin
#define POIN(durasi) ((durasi) * 10)

// Uncomment baris berikut untuk mengaktifkan mode DEBUG
// #define DEBUG

// Struktur data Buku (Array)
typedef struct {
    char kode[MAX_KODE];
    char judul[MAX_JUDUL];
    int stok;
} Buku;

// Struktur data Anggota untuk Linked List
typedef struct NodeAnggota {
    int id;
    char nama[MAX_NAMA];
    struct NodeAnggota* next;
} NodeAnggota;

// Struktur data untuk Binary Search Tree
typedef struct NodeTree {
    int id;
    char nama[MAX_NAMA];
    struct NodeTree* left;
    struct NodeTree* right;
} NodeTree;

// Struktur data untuk Queue (Antrean Peminjaman)
typedef struct NodeQueue {
    int idAnggota;
    char kodeBuku[MAX_KODE];
    int durasi;
    struct NodeQueue* next;
} NodeQueue;

typedef struct {
    NodeQueue* front;
    NodeQueue* rear;
} Queue;

// Struktur data untuk Stack (Riwayat Pengembalian)
typedef struct NodeStack {
    int idAnggota;
    char kodeBuku[MAX_KODE];
    int durasi;
    struct NodeStack* next;
} NodeStack;

// Global variables
Buku daftarBuku[MAX_BUKU];
int jumlahBuku = 0;
NodeAnggota* headLinkedList = NULL;
NodeTree* rootTree = NULL;
Queue queuePeminjaman = {NULL, NULL};
NodeStack* stackPengembalian = NULL;

// ========== FUNGSI UNTUK BUKU (ARRAY) ==========
void tambahBuku(char* kode, char* judul, int stok) {
    if (jumlahBuku >= MAX_BUKU) {
        printf("Array buku penuh!\n");
        return;
    }
    
    strcpy(daftarBuku[jumlahBuku].kode, kode);
    strcpy(daftarBuku[jumlahBuku].judul, judul);
    daftarBuku[jumlahBuku].stok = stok;
    jumlahBuku++;
    
    #ifdef DEBUG
    printf("[DEBUG] Buku ditambahkan: %s - %s (Stok: %d)\n", kode, judul, stok);
    #endif
    
    printf("Buku berhasil ditambahkan!\n");
}

void tampilkanBuku() {
    printf("\n=== DAFTAR BUKU ===\n");
    if (jumlahBuku == 0) {
        printf("Tidak ada buku dalam daftar.\n");
        return;
    }
    
    for (int i = 0; i < jumlahBuku; i++) {
        printf("%d. Kode: %s | Judul: %s | Stok: %d\n", 
               i+1, daftarBuku[i].kode, daftarBuku[i].judul, daftarBuku[i].stok);
    }
}

// ========== FUNGSI UNTUK LINKED LIST ==========
void tambahAnggotaLinkedList(int id, char* nama) {
    NodeAnggota* newNode = (NodeAnggota*)malloc(sizeof(NodeAnggota));
    newNode->id = id;
    strcpy(newNode->nama, nama);
    newNode->next = NULL;
    
    if (headLinkedList == NULL) {
        headLinkedList = newNode;
    } else {
        NodeAnggota* temp = headLinkedList;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    
    #ifdef DEBUG
    printf("[DEBUG] Anggota ditambahkan ke Linked List: ID %d - %s\n", id, nama);
    #endif
}

// ========== FUNGSI UNTUK BINARY SEARCH TREE ==========
NodeTree* buatNodeTree(int id, char* nama) {
    NodeTree* newNode = (NodeTree*)malloc(sizeof(NodeTree));
    newNode->id = id;
    strcpy(newNode->nama, nama);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

NodeTree* tambahKeTree(NodeTree* root, int id, char* nama) {
    if (root == NULL) {
        #ifdef DEBUG
        printf("[DEBUG] Node Tree dibuat: ID %d - %s\n", id, nama);
        #endif
        return buatNodeTree(id, nama);
    }
    
    if (id < root->id) {
        root->left = tambahKeTree(root->left, id, nama);
    } else if (id > root->id) {
        root->right = tambahKeTree(root->right, id, nama);
    }
    
    return root;
}

void inorderTraversal(NodeTree* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("ID: %d | Nama: %s\n", root->id, root->nama);
        inorderTraversal(root->right);
    }
}

void tambahAnggota(int id, char* nama) {
    tambahAnggotaLinkedList(id, nama);
    rootTree = tambahKeTree(rootTree, id, nama);
    printf("Anggota berhasil ditambahkan!\n");
}

void tampilkanAnggotaTerurut() {
    printf("\n=== DAFTAR ANGGOTA (Terurut berdasarkan ID) ===\n");
    if (rootTree == NULL) {
        printf("Tidak ada anggota dalam daftar.\n");
        return;
    }
    inorderTraversal(rootTree);
}

// ========== FUNGSI UNTUK QUEUE (ANTREAN PEMINJAMAN) ==========
void enqueue(int idAnggota, char* kodeBuku, int durasi) {
    NodeQueue* newNode = (NodeQueue*)malloc(sizeof(NodeQueue));
    newNode->idAnggota = idAnggota;
    strcpy(newNode->kodeBuku, kodeBuku);
    newNode->durasi = durasi;
    newNode->next = NULL;
    
    if (queuePeminjaman.rear == NULL) {
        queuePeminjaman.front = queuePeminjaman.rear = newNode;
    } else {
        queuePeminjaman.rear->next = newNode;
        queuePeminjaman.rear = newNode;
    }
    
    #ifdef DEBUG
    printf("[DEBUG] Data ditambahkan ke Queue: ID Anggota %d | Kode Buku %s | Durasi %d hari\n", 
           idAnggota, kodeBuku, durasi);
    #endif
    
    printf("Peminjaman ditambahkan ke antrean! Poin yang diperoleh: %d\n", POIN(durasi));
}

void tampilkanQueue() {
    printf("\n=== ANTREAN PEMINJAMAN ===\n");
    if (queuePeminjaman.front == NULL) {
        printf("Antrean kosong.\n");
        return;
    }
    
    NodeQueue* temp = queuePeminjaman.front;
    int no = 1;
    while (temp != NULL) {
        printf("%d. ID Anggota: %d | Kode Buku: %s | Durasi: %d hari | Poin: %d\n", 
               no++, temp->idAnggota, temp->kodeBuku, temp->durasi, POIN(temp->durasi));
        temp = temp->next;
    }
}

// ========== FUNGSI UNTUK STACK (RIWAYAT PENGEMBALIAN) ==========
void push(int idAnggota, char* kodeBuku, int durasi) {
    NodeStack* newNode = (NodeStack*)malloc(sizeof(NodeStack));
    newNode->idAnggota = idAnggota;
    strcpy(newNode->kodeBuku, kodeBuku);
    newNode->durasi = durasi;
    newNode->next = stackPengembalian;
    stackPengembalian = newNode;
    
    #ifdef DEBUG
    printf("[DEBUG] Data ditambahkan ke Stack: ID Anggota %d | Kode Buku %s | Durasi %d hari\n", 
           idAnggota, kodeBuku, durasi);
    #endif
    
    printf("Pengembalian berhasil dicatat! Poin yang diperoleh: %d\n", POIN(durasi));
}

void tampilkanStack() {
    printf("\n=== RIWAYAT PENGEMBALIAN (LIFO) ===\n");
    if (stackPengembalian == NULL) {
        printf("Riwayat kosong.\n");
        return;
    }
    
    NodeStack* temp = stackPengembalian;
    int no = 1;
    while (temp != NULL) {
        printf("%d. ID Anggota: %d | Kode Buku: %s | Durasi: %d hari | Poin: %d\n", 
               no++, temp->idAnggota, temp->kodeBuku, temp->durasi, POIN(temp->durasi));
        temp = temp->next;
    }
}

// ========== MENU UTAMA ==========
void tampilkanMenu() {
    printf("\n========================================\n");
    printf("   SISTEM MANAJEMEN PERPUSTAKAAN\n");
    printf("========================================\n");
    printf("1. Tambah Buku\n");
    printf("2. Tambah Anggota\n");
    printf("3. Tambah Peminjaman (Queue)\n");
    printf("4. Tambah Pengembalian (Stack)\n");
    printf("5. Tampilkan Daftar Buku\n");
    printf("6. Tampilkan Daftar Anggota (Terurut)\n");
    printf("7. Tampilkan Antrean Peminjaman\n");
    printf("8. Tampilkan Riwayat Pengembalian\n");
    printf("9. Keluar\n");
    printf("========================================\n");
    printf("Pilih menu: ");
}

int main() {
    int pilihan;
    char kode[MAX_KODE], judul[MAX_JUDUL], nama[MAX_NAMA];
    int stok, id, durasi;
    
    #ifdef DEBUG
    printf("[DEBUG MODE AKTIF]\n");
    #endif
    
    while (1) {
        tampilkanMenu();
        scanf("%d", &pilihan);
        getchar(); // consume newline
        
        switch (pilihan) {
            case 1:
                printf("\nMasukkan Kode Buku: ");
                fgets(kode, MAX_KODE, stdin);
                kode[strcspn(kode, "\n")] = 0;
                
                printf("Masukkan Judul Buku: ");
                fgets(judul, MAX_JUDUL, stdin);
                judul[strcspn(judul, "\n")] = 0;
                
                printf("Masukkan Stok: ");
                scanf("%d", &stok);
                getchar();
                
                tambahBuku(kode, judul, stok);
                break;
                
            case 2:
                printf("\nMasukkan ID Anggota: ");
                scanf("%d", &id);
                getchar();
                
                printf("Masukkan Nama Anggota: ");
                fgets(nama, MAX_NAMA, stdin);
                nama[strcspn(nama, "\n")] = 0;
                
                tambahAnggota(id, nama);
                break;
                
            case 3:
                printf("\nMasukkan ID Anggota: ");
                scanf("%d", &id);
                getchar();
                
                printf("Masukkan Kode Buku: ");
                fgets(kode, MAX_KODE, stdin);
                kode[strcspn(kode, "\n")] = 0;
                
                printf("Masukkan Durasi Peminjaman (hari): ");
                scanf("%d", &durasi);
                getchar();
                
                enqueue(id, kode, durasi);
                break;
                
            case 4:
                printf("\nMasukkan ID Anggota: ");
                scanf("%d", &id);
                getchar();
                
                printf("Masukkan Kode Buku: ");
                fgets(kode, MAX_KODE, stdin);
                kode[strcspn(kode, "\n")] = 0;
                
                printf("Masukkan Durasi Peminjaman (hari): ");
                scanf("%d", &durasi);
                getchar();
                
                push(id, kode, durasi);
                break;
                
            case 5:
                tampilkanBuku();
                break;
                
            case 6:
                tampilkanAnggotaTerurut();
                break;
                
            case 7:
                tampilkanQueue();
                break;
                
            case 8:
                tampilkanStack();
                break;
                
            case 9:
                printf("\nTerima kasih telah menggunakan sistem perpustakaan!\n");
                return 0;
                
            default:
                printf("\nPilihan tidak valid!\n");
        }
    }
    
    return 0;
}