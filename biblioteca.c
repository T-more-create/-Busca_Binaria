#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Livro {
    char titulo[100];
    char autor[100];
    int ano;
} Livro;

typedef struct Node {
    Livro livro;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct Biblioteca {
    Node* head;
    Node* tail;
} Biblioteca;

Biblioteca* criar_biblioteca() {
    Biblioteca* b = (Biblioteca*)malloc(sizeof(Biblioteca));
    b->head = NULL;
    b->tail = NULL;
    return b;
}

void adicionar_livro_ordenado(Biblioteca* b, Livro livro) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->livro = livro;
    novo->next = NULL;

    if (b->head == NULL) {
        novo->prev = NULL;
        b->head = novo;
        b->tail = novo;
    } else {
        Node* atual = b->head;
        while (atual != NULL && strcmp(atual->livro.titulo, livro.titulo) < 0) {
            atual = atual->next;
        }
        if (atual == b->head) {
            novo->next = b->head;
            b->head->prev = novo;
            b->head = novo;
            novo->prev = NULL;
        } else if (atual == NULL) {
            novo->prev = b->tail;
            b->tail->next = novo;
            b->tail = novo;
        } else {
            novo->prev = atual->prev;
            novo->next = atual;
            atual->prev->next = novo;
            atual->prev = novo;
        }
    }
    printf("Livro '%s' adicionado com sucesso!\n", livro.titulo);
}

void remover_livro(Biblioteca* b, const char* titulo) {
    Node* atual = b->head;

    while (atual != NULL) {
        if (strcmp(atual->livro.titulo, titulo) == 0) {
            if (atual->prev != NULL) {
                atual->prev->next = atual->next;
            } else {
                b->head = atual->next;
            }

            if (atual->next != NULL) {
                atual->next->prev = atual->prev;
            } else {
                b->tail = atual->prev;
            }

            free(atual);
            printf("Livro '%s' removido com sucesso!\n", titulo);
            return;
        }
        atual = atual->next;
    }

    printf("Livro '%s' nao encontrado na biblioteca.\n", titulo);
}

int comparar_livros(const void* a, const void* b) {
    Livro* livroA = (Livro*)a;
    Livro* livroB = (Livro*)b;

    int cmp = strcmp(livroA->titulo, livroB->titulo);
    if (cmp != 0) return cmp;

    cmp = strcmp(livroA->autor, livroB->autor);
    if (cmp != 0) return cmp;

    return livroA->ano - livroB->ano;
}

Livro* copiar_para_array(Biblioteca* b, int* tamanho) {
    *tamanho = 0;
    Node* atual = b->head;

    while (atual != NULL) {
        (*tamanho)++;
        atual = atual->next;
    }

    Livro* livros_array = (Livro*)malloc((*tamanho) * sizeof(Livro));
    atual = b->head;
    for (int i = 0; i < *tamanho; i++) {
        livros_array[i] = atual->livro;
        atual = atual->next;
    }

    return livros_array;
}

int busca_binaria(Livro* livros, int tamanho, const char* titulo, const char* autor, int ano) {
    int esquerda = 0;
    int direita = tamanho - 1;

    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        Livro* livroMeio = &livros[meio];

        int cmpTitulo = strcmp(livroMeio->titulo, titulo);
        int cmpAutor = strcmp(livroMeio->autor, autor);
        int cmpAno = livroMeio->ano - ano;

        if (cmpTitulo == 0 && cmpAutor == 0 && (ano == -1 || cmpAno == 0)) {
            return meio;
        }

        if (cmpTitulo < 0 || (cmpTitulo == 0 && cmpAutor < 0) || (cmpTitulo == 0 && cmpAutor == 0 && cmpAno < 0)) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    return -1; 
}

void buscar_livro(Biblioteca* b) {
    int busca_opcao;
    char titulo[100];
    char autor[100];
    int ano;

    printf("Escolha o criterio de busca:\n");
    printf("1. Por Titulo\n");
    printf("2. Por Autor\n");
    printf("3. Por Ano\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &busca_opcao);
    getchar();

    Livro* livros_array = NULL;
    int tamanho = 0;

    switch (busca_opcao) {
        case 1:
            printf("Digite o titulo do livro: ");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = 0;
            livros_array = copiar_para_array(b, &tamanho);
            break;

        case 2:
            printf("Digite o autor do livro: ");
            fgets(autor, sizeof(autor), stdin);
            autor[strcspn(autor, "\n")] = 0;
            livros_array = copiar_para_array(b, &tamanho);
            break;

        case 3:
            printf("Digite o ano de publicação do livro: ");
            scanf("%d", &ano);
            getchar();
            livros_array = copiar_para_array(b, &tamanho);
            break;

        default:
            printf("Opcao invalida! Tente novamente.\n");
            return;
    }

    if (livros_array != NULL) {
        int indice = busca_binaria(livros_array, tamanho, 
                                   strcmp(titulo, "") == 0 ? "" : titulo,
                                   strcmp(autor, "") == 0 ? "" : autor,
                                   ano == 0 ? -1 : ano);
        if (indice != -1) {
            printf("Livro encontrado: Titulo: %s, Autor: %s, Ano: %d\n",
                   livros_array[indice].titulo,
                   livros_array[indice].autor,
                   livros_array[indice].ano);
        } else {
            printf("Livro nao encontrado!\n");
        }
        free(livros_array);
    }
}

void listar_livros(Biblioteca* b) {
    Node* atual = b->head;
    if (atual == NULL) {
        printf("Nao ha livros na biblioteca.\n");
        return;
    }
    printf("Livros na biblioteca:\n");
    while (atual != NULL) {
        printf("Titulo: %s, Autor: %s, Ano: %d\n", atual->livro.titulo, atual->livro.autor, atual->livro.ano);
        atual = atual->next;
    }
}

void listar_livros_ordenados(Biblioteca* b) {
    int tamanho = 0;
    Livro* livros_array = copiar_para_array(b, &tamanho);

    if (tamanho > 0) {
        qsort(livros_array, tamanho, sizeof(Livro), comparar_livros);
        printf("Livros na biblioteca em ordem alfabética:\n");
        for (int i = 0; i < tamanho; i++) {
            printf("Titulo: %s, Autor: %s, Ano: %d\n", livros_array[i].titulo, livros_array[i].autor, livros_array[i].ano);
        }
    } else {
        printf("Nao ha livros na biblioteca.\n");
    }
    free(livros_array);
}

void liberar_biblioteca(Biblioteca* b) {
    Node* atual = b->head;
    while (atual != NULL) {
        Node* temp = atual;
        atual = atual->next;
        free(temp);
    }
    free(b);
}

int main() {
    Biblioteca* minha_biblioteca = criar_biblioteca();
    int opcao;

    do {
        printf("\nMenu de Gerenciamento da Biblioteca:\n");
        printf("1. Adicionar Livro\n");
        printf("2. Remover Livro\n");
        printf("3. Listar Livros\n");
        printf("4. Listar Livros Ordenados\n");
        printf("5. Buscar Livro\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: {
                Livro livro;
                printf("Digite o titulo do livro: ");
                fgets(livro.titulo, sizeof(livro.titulo), stdin);
                livro.titulo[strcspn(livro.titulo, "\n")] = 0;

                printf("Digite o autor do livro: ");
                fgets(livro.autor, sizeof(livro.autor), stdin);
                livro.autor[strcspn(livro.autor, "\n")] = 0;

                printf("Digite o ano de publicacao: ");
                scanf("%d", &livro.ano);
                getchar();

                adicionar_livro_ordenado(minha_biblioteca, livro);
                break;
            }
            case 2: {
                char titulo[100];
                printf("Digite o titulo do livro a ser removido: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = 0;

                remover_livro(minha_biblioteca, titulo);
                break;
            }
            case 3:
                listar_livros(minha_biblioteca);
                break;
            case 4:
                listar_livros_ordenados(minha_biblioteca);
                break;
            case 5:
                buscar_livro(minha_biblioteca);
                break;
            case 6:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 6);

    liberar_biblioteca(minha_biblioteca);
    return 0;
}
