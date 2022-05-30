#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "hierarchy.h"

int min(int x, int y) {
    if(x<y)
        return x;
    else 
        return y;
}

int compare_employees(char *e1, char *e2) {       //0 = e1 mai mare, 1 = e2 mai mare
    int min_len = min(strlen(e1), strlen(e2));
    for(int i=0; i<min_len; i++) {
        if((int)(e1[i]) > (int)(e2[i]))
            return 0;
        if((int)(e1[i]) < (int)(e2[i]))
            return 1;
    }
    if(min_len == strlen(e1))
        return 1;
    else 
        return 0;
}

void insert_pos(char **team, int pos, char *employee, int team_len) {
    if(pos == team_len) {
        team[pos] = employee;
    } else {
        char *tmp1 = team[pos];
        char *tmp2 = NULL;
        team[pos] = employee;
        for(int i=pos+1; i<team_len; i++) {
            tmp2 = team[i];
            team[i] = tmp1;
            tmp1 = tmp2;
        }
        team[team_len] = tmp1;
    }
}

void add_member_alphabetically(char **team, char *employee, int head, int tail, int team_len) {

    if(head == tail) {
        if(compare_employees(employee, team[head]))
            insert_pos(team, head, employee, team_len);
        else 
            insert_pos(team, head+1, employee, team_len);
        return;
    }

    int middle = (head+tail)/2;

    if(compare_employees(employee, team[middle])) {
        if(middle == head)
            add_member_alphabetically(team, employee, head, head, team_len);
        add_member_alphabetically(team, employee, head, middle-1, team_len);
    } else {
        if(middle == tail)
            add_member_alphabetically(team, employee, tail, tail, team_len);
        add_member_alphabetically(team, employee, middle+1, tail, team_len);
    }

}

int main() {

    char **team = NULL;
    team = malloc(100*sizeof(char*));
    int team_len = 0;
    char *str1 = "a5";
    char *str2 = "a1";
    char *str3 = "a3";
    char *str4 = "a7";
    char *str5 = "a0";

    insert_pos(team, 0, str1, team_len);
    team_len++;
    // insert_pos(team, 1, str2, team_len);
    add_member_alphabetically(team, str2, 0, team_len-1, team_len);
    team_len++;
    add_member_alphabetically(team, str3, 0, team_len-1, team_len);
    team_len++;
    add_member_alphabetically(team, str4, 0, team_len-1, team_len);
    team_len++;
    add_member_alphabetically(team, str5, 0, team_len-1, team_len);
    team_len++;

    printf("\nteam-ul este: %s, %s, %s, %s, %s\n", team[0], team[1], team[2], team[3], team[4]);
    free(team);
}