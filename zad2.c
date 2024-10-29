Zad 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _person* position;
typedef struct _person {
    char fname[32], lname[32];
    int birth_year;
    position next;
}person;

position create_person(char* fname, char* lname, int year);
void insert_after(position prev, position new);
int prepend_list(position head, char* fname, char* lname, int year);
int print_list(position first);
position find_last(position head);
int append_list(position head, char* fname, char* lname, int year);
position find_by_lname(position first, char* lname);
position find_previous(position head, position wanted);
position delete(position head, position wanted);
void insert_before(position head, position curr, char* fname, char* lname, int year);
void sort_by_lname(position first);
void print_in_file(position first);
void read_file(position head, FILE* fp);


int main() {
    position head = (position)malloc(sizeof(person));
    if (!head) {
        printf("Neuspjesno alociranje memorije\n");
        return -1;
    }

    head->next = NULL;



    return 0;
}

position create_person(char* fname, char* lname, int* year) {
    position new_person = NULL;

    new_person = (position*)malloc(sizeof(person)); 
    if (new_person == NULL) {
        printf("Neuspjesno alocirana memorija\n");
        return NULL;    
    }

    strcpy(new_person->fname, fname);
    strcpy(new_person->lname, lname);
    new_person->birth_year = year;
    new_person->next = NULL;

    return new_person;

}


void insert_after(position prev, position new) {
    if (prev != NULL && new != NULL) {
        new->next = prev->next;
        prev->next = new;
    }
}

int prepend_list(position head, char* fname, char* lname, int* year) {
    position new_person = NULL;

    new_person = create_person(fname, lname, year);
    if (!new_person) {
        printf("Neuspjesno alocirana memorija\n");
        return -1;
    }

    insert_after(head, new_person);
}

int print_list(position first) {
    position temp = first;

    while (temp != NULL) {
        printf("%s %s %d\n", temp->fname, temp->lname, temp->birth_year);
        temp = temp->next;
    }
}

position find_last(position head) {
    position temp = head;

    while (temp->next != NULL) {
        temp = temp->next;
    }

    return temp;
}

int apend_list(position head, char* fname, char* lname, int* year) {
    position new_person = NULL, last = NULL;

    new_person = create_person(fname, lname, year);
    if (!new_person) {
        printf("Neuspjesno alocirana memorija\n");
        return -1;
    }

    last = find_last(head);

    insert_after(last, new_person);

    return 0;
}

position find_by_lname(position first, char* lname) 
    position temp = first;

    while (temp) {
        if (strcmp(temp->lname, lname) == 0) {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

position find_previus(position head, position wanted) {
    position temp = head;

    while (temp->next != NULL) {
        if (temp->next == wanted) {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

position delete(position head, position wanted) {
    position prev = NULL;
    prev = find_previus(head, wanted);

    if (prev != NULL) {
        position toDelete = prev->next;
        prev->next = toDelete->next;
        free(toDelete);
    }
    else {
        printf("element nije pronadjen\n");
    }

    return head;
}

void insert_before(position head, position curr, char* fname, char* lname, int year) {
    position temp = head;

    while (temp->next != curr && temp->next != NULL) {
        temp = temp->next;
    }
    if (temp->next == NULL) {
        printf("element nije pronadjen\n");
        return;
    }

    position q = create_person(fname, lname, year);

    q->next = temp->next;
    temp->next = q;
}

void sort_by_lname(position first) {     
    position j, prev, temp;
    position end = NULL;

    while (first->next != end) {
        prev = first;
        j = first->next;

        while (j->next != end) {
            if (strcmp(j->lname, j->next->lname) > 0) {
                temp = j->next;
                prev->next = temp;
                j->next = temp->next;
                temp->next = j;

                j = temp;
            }
            prev = j;
            j = j->next;
        }
        end = j;
    }

}

void print_in_file(position first) {     //saljemo head->next
    FILE* fp = NULL;
    fp = fopen("lista.txt", "w");
    if (fp == NULL) {
        printf("neuspjesna alokacija memorije\n");
        return;
    }

    position temp = first;
    fprintf(fp, "IME\tPREZIME\t\tGODINA RODJENJA\n");
    while (temp != NULL) {
        fprintf("%s %s %d\n", temp->fname, temp->lname, temp->birth_year);
        temp = temp->next;
    }

    fclose(fp);
    printf("Podaci uspejsno upisani u datoteku!\n");
}

void read_file(position head, FILE* fp) {
    if (fp == NULL) {
        printf("datoteka ne postoji\n");
        return;
    }

    char buffer[1024];
    fgets(buffer, sizeof(buffer), fp);

    char name[50], lname[50];
    int year;
    position last = head;

    while (fscanf(fp, "%s %s %d", name, lname, &year) == 3) {

        position q = create_person(name, lname, year);
        if (q == NULL) {
            printf("Neuspjesna alokacija memorije\n");
            return;
        }

        last->next = q;
        last = q;

    }
    fclose(fp);
}
