#include "hunt.h"
#include <stdio.h>

/* Task 1 - Axii */

uint16_t find_spell(uint64_t memory)
{
    /*
     * The spell is 16 bits and they are in the memory exactly after 5
     * consecutive bits of 1:
     *
     *            high                                             low
     *                [][][x]...[x][x][x][1][1][1][1][1][]...[][][]
     *                    |----16bits---|
     *                          Spell
     *
     * There is only 1 group of 5 consecutive bits of 1 and the other groups
     * have less than consecutive bits of 1
     * Steps:
     * - find 5 consecutive bits of 1
     * - the NEXT 16 bits == spell
     */

    uint16_t res = -1;

    /* TODO */

    uint64_t mask = (uint64_t)1;
    int i = 0;
    int j = 0;
    do{  
        if(((memory & mask) >> j) == 1){ 
            i++;
        }
        else{
            i = 0;
        }
        j++;  
        mask <<= 1;  
    }
    while (i != 5);
    res = (uint16_t)((memory << (64-16-j)) >> (64-16));  

    return res;
}


uint16_t find_key(uint64_t memory)
{
    /*
     * The key is 16 bits and they are in the memory exactly before 3
     * consecutive bits of 1:
     *
     *                high                                           low
     *                     [][][][1][1][1][x][x]...[x][x][]...[][][]
     *                                    |----16bits---|
     *                                           Key
     * Steps:
     * - find 3 consecutive bits of 1
     * - the PREVIOUS 16 bits == key
     */

    uint16_t res = -1;

    /* TODO */

    uint64_t mask = (uint64_t)1;
    int i = 0;
    int j = 0;
    do{
        if(((memory & mask) >> j) == 1){  
            i++;
        }
        else{
            i = 0;               
        }
        j++;
        mask <<= 1;
    }
    while (i != 3);
    res = (uint16_t)((memory << (64-j+3)) >> (64-16));

    return res;
}


uint16_t decrypt_spell(uint16_t spell, uint16_t key)
{
    /*
     * Find the spell knowing that
     * spell_encrypted = spell_plaintext ^ key
     */

    uint16_t res = 0;

    /* TODO */

    for (int i=0; i<16; i++){
        int value_bit_i_key = ((int)((key >> i) & (uint16_t)1));  
        int value_bit_i_spell = ((int)((spell >> i) & (uint16_t)1));  
        if (value_bit_i_spell){  
            if (!(value_bit_i_key)){
                res = res | ((uint16_t)1 << i);
            }
        }
        else{  
            if (value_bit_i_key){
                res = res | ((uint16_t)1 << i);
            }
        }
    }

    return res;
}


/* Task 2 - Sword Play */

uint32_t choose_sword(uint16_t enemy)
{
    /*
     * Help Geralt construct a sword that will give him an advantage in battle.
     * The steps we will follow:
     * - choose the corect sword
     * - brand the sword with the correct runes such that the equation
     * for that specific enemy type is true
     *
     * How does the sword look (Sword template):
     *  [][][][] [0][0][0]...[0][0][0] [][][][][][][][][][][][][][][][]
     *  -------- --------------------- --------------------------------
     *     Type    Does not matter      The "Value" such that the equation
     *                               from 2 holds depending on the enemy type
     *    4 bits      12 bits                     16 bits
     *
     * 1. Type of the sword:
     * - 1001 - Silver Sword - for monsters
     * - 0110 - Steel Sword - for humans
     *
     *  If the number of active bits from variable "enemy" is even == Monster
     *  else == Human
     *
     * 2.
     * Monster Equation:
     *    Value ^ (Enemy & (1 - Enemy)) = 0
     *
     * Human Equation:
     *    Value + Enemy = 0
     */
    
    uint32_t res = 0;

    /* TODO */

    int nr_of_1 = 0;
    uint16_t value;
    uint32_t special_1 = (uint32_t)1;

    for (int i=0; i<16; i++){if ((enemy >> i) & (uint16_t)1){nr_of_1++;}}  
    if ((nr_of_1 % 2) == 0){  
        res = (res | (special_1 << 31));
        res = (res | (special_1 << 28));
        value = (enemy & ((uint16_t)1 + ((~(enemy))+ (uint16_t)1)));
    }
    else{  
        res = (res | (special_1 << 30));
        res = (res | (special_1 << 29));
        value = ((~(enemy))+(uint16_t)1);
    }
    res = (res | (uint32_t)value);  


    return res;
}


/* Task 3 - The Witcher Trials */

uint32_t trial_of_the_grasses(uint16_t cocktail)
{
    /*
     * To become a witcher one must survive a cocktail of different
     * herbs. The body should generate the correct antibodies to
     * neutralize the given cocktail.
     *
     *
     * The antibodies need to respect the following properties:
     *   (antibodies_high & cocktail) ^ (antibodies_low | cocktail) = 0 
     *   antibodies_low & antibodies_high = 0
     *   antibodies_low | antibodies_high = cocktail
     *
     * Where: 
     *  [][][]...[][][] | [][][]...[][][]
     *  ---------------   ---------------
     *  antibodies_high    antibodies_low
     *      16 bits           16 bits
     *      -------------------------
     *              antibodies
     */

    uint32_t res = 0;

    /* TODO */

    res = (res | ((uint32_t)cocktail << 16));

    return res;
}


uint8_t trial_of_forrest_eyes(uint64_t map)
{
    /*
     * For the next trail, the candidate is tied up and blindfolded.
     * They are also taken to a forest and they should return until
     * the next morning.
     *
     * The candidate knows that there are 4 possibilities for the forest:
     * Brokilon, Hindar, Wolven Glade and Caed Dhu.
     *
     * The candidate also knows those forests by heart, but first you
     * need to identify the forest.
     *
     * The characteristics for 3 of the forests are as follows:
     * - Brokilon - has ONLY groups of 4 trees (or a forest without any tree)
     *   Ex: a group of 4 trees: "...00111100..."
     * - Hindar - has ONLY 2 trees in the MIDDLE
     * - Wolven Glade - FULL of trees
     *
     * The "map" variable encodes the following information:
     * - 1 tree
     * - 0 patch of grass
     *
     * You should put in the "res" variable the index for the identified
     * forrest.
     * - Brokilon - index 0
     * - Hindar - index 1
     * - Wolven Glade - index 2
     * - Caed Dhu - index 3
     */

    uint8_t res = -1;

    /* TODO */

    /* 
    j = 0 -> retine cati de 1 consecutivi au fost
    nr_of_0 = 0 -> retine cati de 0 sunt in map
    is_Brokilon = 1 -> daca este sau nu(by default e 1 la inceput)
    is_Hindar = 0 -> daca este sau nu
    is_Wolven_Glade = 0 -> daca este sau nu
    */

    int j = 0;
    int is_Brokilon = 1;
    int is_Wolven_Glade = 0;  
    int nr_of_0 = 0;
    int is_Hindar = 0;
    for (int i=0; i<64; i++){  
        if ((map >> i) & (uint64_t)1){  
            j++;
        } 
        else{
            if ((j == 2) && (i == 33) && (nr_of_0 == 31)){  
                is_Brokilon = 0;                            
                is_Hindar = 1;
            }
            else if ((j != 4) && (j != 0) && !(is_Hindar)){
                is_Brokilon = 0;
                break;
            }
            j = 0;             
            nr_of_0++;
        } 
    }
    if (j == 64){   
        is_Brokilon = 0;
        is_Wolven_Glade = 1;
    }
    if ((is_Hindar) && (nr_of_0 != 62)){  
        is_Hindar = 0;
    }

    if (is_Brokilon){
        res = (uint8_t)0;
    }
    else if (is_Hindar){
        res = (uint8_t)1;
    }
    else if (is_Wolven_Glade){
        res = (uint8_t)2;
    }
    else{
        res = (uint8_t)3;
    }

    return res;
}


uint8_t trial_of_the_dreams(uint32_t map)
{
    /*
     * For this final trial, the candidates go into a trance and when
     * they woke up they would be a fully fledged witcher.
     *
     * For this task, you need to help the witcher find the number
     * of steps that would take the candidate from the dream realm to
     * the real world.
     *
     * The "map" variable would have only 2 bits of 1:
     * - candidate location
     * - portal location
     *
     * You should put in the "res" variable the distance from the
     * candidate position to the portal.
     *
     *  Ex:
     *  [0][0][0][0][0][0]....[1][0][0][0][0][1][0]....[0][0][0]
     *
     *  res = 5
     */

    uint8_t res = -1;

    /* TODO */

    int j = 0;                                        
    int j_occupied = 1;
    for (int i=0; i<64; i++){                         
        if ((j_occupied) && ((map >> i) & (uint64_t)1)){
            j = i;
            j_occupied = 0;
        }
        else if (!(j_occupied) && ((map >> i) & (uint64_t)1)){
            res = i - j;
            break;
        }
    }

    return res;
}
