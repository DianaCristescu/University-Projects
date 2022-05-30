#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"
#define MAX_S 50
#define MAX 100

//************************************************************************

Tree create(char *employee_name) {
    Tree employee = NULL;
    employee = (Tree)malloc(sizeof(TreeNode));

    employee->name = NULL; 
    employee->name = (char*)malloc(MAX_S*sizeof(char));
    strncpy(employee->name, employee_name, MAX_S);
    employee->name[strlen(employee_name)] = '\0';

    employee->manager = NULL;
    employee->direct_employees_no = 0;

    employee->team = NULL;
    employee->team = (Tree*)malloc(MAX*sizeof(Tree));

    return employee;
}

//************************************************************************

void insert_pos(Tree *team, int pos, Tree employee, int team_len) {
    if(pos == team_len) {
        team[pos] = employee;
    } else {
        Tree tmp1 = team[pos];
        Tree tmp2 = NULL;
        team[pos] = employee;
        for(int i=pos+1; i<team_len; i++) {
            tmp2 = team[i];
            team[i] = tmp1;
            tmp1 = tmp2;
        }
        team[team_len] = tmp1;
    }
}

//************************************************************************

int min(int x, int y) {
    if(x<y)
        return x;
    else 
        return y;
}

//************************************************************************

int compare_employees(Tree e1_node, Tree e2_node) {       //0 = e1 mai mare, 1 = e2 mai mare
    
    int len1 = strlen(e1_node->name);
    int len2 = strlen(e2_node->name);

    int min_len = min(len1, len2);
    for(int i=0; i<min_len; i++) {
        if((int)(e1_node->name[i]) > (int)(e2_node->name[i]))
            return 0;
        if((int)(e1_node->name[i]) < (int)(e2_node->name[i]))
            return 1;
    }
    if(min_len == len1)
        return 1;
    else 
        return 0;
}

//************************************************************************

void add_member_alphabetically(Tree *team, Tree employee, int head, int tail, int team_len) {

    if(head == tail) {
        if(compare_employees(employee, team[head]))
            insert_pos(team, head, employee, team_len);
        else 
            insert_pos(team, head+1, employee, team_len);
        return;
    }
    
    int middle = (head+tail)/2;

    if(compare_employees(employee, team[middle])) {
        if(middle == head) {
            add_member_alphabetically(team, employee, head, head, team_len);
            return;
        }
        add_member_alphabetically(team, employee, head, middle-1, team_len);
    } else {
        if(middle == tail) {
            add_member_alphabetically(team, employee, tail, tail, team_len);
            return;
        }
        add_member_alphabetically(team, employee, middle+1, tail, team_len);
    }

}

//************************************************************************

void add_team_member(Tree manager, Tree employee) {
    if(manager->direct_employees_no == 0)
        manager->team[0] = employee;
    else {
        int head = 0;
        int tail = manager->direct_employees_no-1;
        add_member_alphabetically(manager->team, employee, head, tail, tail+1);
    }

    manager->direct_employees_no++;
    employee->manager = manager;
}

//************************************************************************

Tree find_employee(Tree tree, char *employee_name) {
    if (tree == NULL) return NULL;
    if(!strcmp(employee_name, tree->name)) {
        return tree;
    } else {
        Tree employee = NULL;
        for(int i=0; i<tree->direct_employees_no; i++) {
            employee = find_employee(tree->team[i], employee_name);
            if (employee != NULL) return employee;
        }
        return NULL;
    }
}

//************************************************************************

void remove_team_member(Tree *team, Tree employee, int head, int tail, int team_len) {

    if(head == tail) {
        for(int i=head; i<team_len-1; i++) {
            team[i] = team[i+1];
        }
        team[team_len-1] = NULL;
        employee->manager = NULL;
        return;
    }
    
    int middle = (head+tail)/2;
    if(employee == team[middle]) {
        remove_team_member(team, employee, middle, middle, team_len);
        return;
    }

    if(compare_employees(employee, team[middle])) {
        if(middle == head) {
            remove_team_member(team, employee, head, head, team_len);
            return;
        }
        remove_team_member(team, employee, head, middle-1, team_len);
    } else {
        if(middle == tail) {
            remove_team_member(team, employee, tail, tail, team_len);
            return;
        }
        remove_team_member(team, employee, middle+1, tail, team_len);
    }

}

//************************************************************************

void recursive_preord_trav(FILE *f, Tree tree) {  //root, left->right
    if (tree != NULL) {
        if(tree->manager == NULL)
            fprintf(f, "%s ", tree->name);
        else 
            fprintf(f, "%s-%s ", tree->name, tree->manager->name);
        
        for(int i=0; i<tree->direct_employees_no; i++) {
            recursive_preord_trav(f, tree->team[i]);
        }
    }
}

//***********************************************************************

int employees_by_manager(Tree *team, Tree manager, int team_len) {
    
    Tree copy = create(manager->name);
    if(team_len == 0)
        team[0] = copy;
    else 
        add_member_alphabetically(team, copy, 0, team_len-1, team_len);
    ++team_len;

    if(manager->direct_employees_no != 0) {

        for(int i=0; i<manager->direct_employees_no; i++) {
            team_len = employees_by_manager(team, manager->team[i], team_len);
        }
        return team_len;

    } else {
        return team_len;
    }

}

//*************************************************************************************************

int employees_by_level(Tree *team, Tree manager, int level, int current_level, int team_len) {

    if(current_level > level) return team_len;

    if(current_level == level) {

        Tree copy = create(manager->name);
        if(team_len == 0)
            team[0] = copy;
        else 
            add_member_alphabetically(team, copy, 0, team_len-1, team_len);
        ++team_len;
        return team_len;

    } else {

        for(int i=0; i<manager->direct_employees_no; i++) {
            team_len = employees_by_level(team, manager->team[i], level, current_level+1, team_len);
        }
        return team_len;

    }

}

//***************************************************************************************************

int get_best_no_employees(Tree manager, int best_no_employees) {

    if(best_no_employees < manager->direct_employees_no)
        best_no_employees = manager->direct_employees_no;

    if(manager->direct_employees_no == 0)
        return best_no_employees;

    else {

        for(int i=0; i<manager->direct_employees_no; i++) {
            best_no_employees = get_best_no_employees(manager->team[i], best_no_employees);
        }
        return best_no_employees;

    }

}

//***************************************************************************************************

int best_manager(Tree *team, Tree manager, int best_no_employees, int team_len) {

    if(manager->direct_employees_no == best_no_employees) {
        Tree copy = create(manager->name);
        if(team_len == 0)
            team[0] = copy;
        else 
            add_member_alphabetically(team, copy, 0, team_len-1, team_len);
        ++team_len;
    }

    if(manager->direct_employees_no != 0) {

        for(int i=0; i<manager->direct_employees_no; i++) {
            team_len = best_manager(team, manager->team[i], best_no_employees, team_len);
        }
        return team_len;

    } else {
        return team_len;
    }

}

//***************************************************************************************************

/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, 
           atunci employee_name e primul om din ierarhie.
 */

Tree hire(Tree tree, char *employee_name, char *manager_name) {

    Tree employee = create(employee_name);

    if (manager_name == NULL) {
        return employee;
    }

    Tree manager = find_employee(tree, manager_name);
    add_team_member(manager, employee);
    return tree;

}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */

Tree fire(Tree tree, char *employee_name) {

    Tree employee = find_employee(tree, employee_name);
    if(employee == NULL) return tree;
    Tree manager = employee->manager;
    if(manager == NULL) return tree;

    for(int i=0; i<employee->direct_employees_no; i++)
        add_team_member(manager, employee->team[i]);

    int head = 0;
    int tail = manager->direct_employees_no-1;
    remove_team_member(manager->team, employee, head, tail, tail+1);
    manager->direct_employees_no--;

    free(employee->name);
    free(employee->team);
    free(employee);
    employee = NULL;

    return tree;
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */

Tree promote(Tree tree, char *employee_name) {
    Tree employee = find_employee(tree, employee_name);
    if(employee == NULL) return tree;
    Tree manager = employee->manager;
    if(manager == NULL || manager->manager == NULL) return tree;
    Tree new_manager = manager->manager;

    tree = fire(tree, employee_name);
    tree = hire(tree, employee_name, new_manager->name);

    return tree;
}

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name) {
    Tree employee = find_employee(tree, employee_name);
    if(employee == NULL) return tree;
    Tree manager = employee->manager;
    if(manager == NULL) return tree;

    Tree new_manager = find_employee(tree, new_manager_name);
    if(new_manager != manager) {
        tree = fire(tree, employee_name);
        tree = hire(tree, employee_name, new_manager_name);
    }
    return tree;
}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name) {
    Tree employee = find_employee(tree, employee_name);
    if(employee == NULL) return tree;
    Tree manager = employee->manager;
    if(manager == NULL) return tree;

    Tree new_manager = find_employee(tree, new_manager_name);
    if(new_manager != manager) {

        int head = 0;
        int tail = manager->direct_employees_no-1;
        remove_team_member(manager->team, employee, head, tail, tail+1);
        manager->direct_employees_no--;

        add_team_member(new_manager, employee);

    }

    return tree;
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire_team(Tree tree, char *employee_name) {
    Tree employee = find_employee(tree, employee_name);
    if(employee == NULL) return tree;
    Tree manager = employee->manager;
    if(manager == NULL) return tree;

    int head = 0;
    int tail = manager->direct_employees_no-1;
    remove_team_member(manager->team, employee, head, tail, tail+1);
    manager->direct_employees_no--;

    destroy_tree(employee);

    return tree;
}

/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */

void get_employees_by_manager(FILE *f, Tree tree, char *employee_name) {
    Tree manager = find_employee(tree, employee_name);
    
    if(manager != NULL) {
        Tree *print_manager_tree = (Tree*)malloc(2*MAX*sizeof(Tree));
        int team_len = employees_by_manager(print_manager_tree, manager, 0);

        for(int i=0; i<team_len; i++) {
            fprintf(f, "%s ", (print_manager_tree[i])->name);
        }

        for(int i=team_len-1; i>=0; i--) {
            free((print_manager_tree[i])->team);
            free((print_manager_tree[i])->name);
            free(print_manager_tree[i]);
        }
        free(print_manager_tree);
    }
    
    fprintf(f, "\n");
    return;
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */

void get_employees_by_level(FILE *f, Tree tree, int level) {

    Tree *print_level = (Tree*)malloc(2*MAX*sizeof(Tree));
    int team_len = employees_by_level(print_level, tree, level, 0, 0);

    for(int i=0; i<team_len; i++) {
        fprintf(f, "%s ", (print_level[i])->name);
    }

    for(int i=team_len-1; i>=0; i--) {
        free((print_level[i])->team);
        free((print_level[i])->name);
        free(print_level[i]);
    }
    free(print_level);
    
    fprintf(f, "\n");
    return;
}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */

void get_best_manager(FILE *f, Tree tree) {
    
    if(tree != NULL) {
        Tree *print_best_manager = (Tree*)malloc(2*MAX*sizeof(Tree));
        int best_no_employees = get_best_no_employees(tree, 0);
        int team_len = best_manager(print_best_manager, tree, best_no_employees, 0);

        for(int i=0; i<team_len; i++) {
            fprintf(f, "%s ", (print_best_manager[i])->name);
        }

        for(int i=team_len-1; i>=0; i--) {
            free((print_best_manager[i])->team);
            free((print_best_manager[i])->name);
            free(print_best_manager[i]);
        }
        free(print_best_manager);
    }
    
    fprintf(f, "\n");
    return;

}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */

Tree reorganize(Tree tree, char *employee_name) { //sparg team-urile dupa manageri si le pun intr-un vector de Tree, le iau la rand si reconstruiesc ierarhia
    
    Tree *managers = (Tree*)malloc(MAX*sizeof(Tree));
    Tree employee = find_employee(tree, employee_name);
    Tree manager = NULL;

    if(employee != NULL) tree = employee;

    int i = 0;
    while(employee != NULL) {

        managers[i] = employee;
        manager = employee->manager;

        if(manager != NULL) {
            int team_len = manager->direct_employees_no;
            remove_team_member(manager->team, employee, 0, team_len-1, team_len);
            manager->direct_employees_no--;
        }

        employee = manager;
        i++;

    }

    for(int j=0; j<i-1; j++) {
        manager = managers[j];
        employee = managers[j+1];
        add_team_member(manager, employee);
    }
    free(managers);

    return tree;
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */

void preorder_traversal(FILE *f, Tree tree) {
    recursive_preord_trav(f, tree);
    fprintf(f, "\n");
}

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */

void destroy_tree(Tree tree) {
    
    if (tree == NULL) return;
    int n = tree->direct_employees_no;
    for(int i=0; i<n; i++) {
        destroy_tree(tree->team[i]);
        --tree->direct_employees_no; 
    }
    free(tree->team);
    free(tree->name);
    free(tree);
    tree = NULL;
}

