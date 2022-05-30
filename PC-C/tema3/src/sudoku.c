#include "cJSON.h"
#include "cJSON.c"
#include <stdio.h>
#include <stdlib.h>

#define STR_ATR 100

void task1(FILE *input, FILE *output){

    fseek(input, 0, SEEK_END);
    long int input_size = ftell(input);
    fseek(input, 0, SEEK_SET);

    char *json_str = malloc(input_size + 1);
    fread(json_str, 1, input_size, input);
    json_str[input_size] = '\0';
    

    cJSON *json = cJSON_Parse(json_str);


    cJSON *bitmap = cJSON_GetObjectItem(json, "bitmap");
    cJSON *bitmap_item = NULL;
    int n = cJSON_GetArraySize(bitmap);

    cJSON *file_header = cJSON_GetObjectItem(json, "file_header");
 
    cJSON *offset = cJSON_GetObjectItem(file_header, "offset");
    cJSON *signature = cJSON_GetObjectItem(file_header, "signature");
    cJSON *reserved = cJSON_GetObjectItem(file_header, "reserved");
    cJSON *file_size = cJSON_GetObjectItem(file_header, "file_size");

    cJSON *info_header = cJSON_GetObjectItem(json, "info_header");

    cJSON *colors_used = cJSON_GetObjectItem(info_header, "colors_used");
    cJSON *size = cJSON_GetObjectItem(info_header, "size");
    cJSON *colors_important = cJSON_GetObjectItem(info_header, "colors_important");
    cJSON *image_size = cJSON_GetObjectItem(info_header, "image_size");
    cJSON *y_pixels_per_meter = cJSON_GetObjectItem(info_header, "y_pixels_per_meter");
    cJSON *x_pixels_per_meter = cJSON_GetObjectItem(info_header, "x_pixels_per_meter");
    cJSON *width = cJSON_GetObjectItem(info_header, "width");
    cJSON *planes = cJSON_GetObjectItem(info_header, "planes");
    cJSON *bit_count = cJSON_GetObjectItem(info_header, "bit_count");
    cJSON *compression = cJSON_GetObjectItem(info_header, "compression");
    cJSON *height = cJSON_GetObjectItem(info_header, "height");

    fwrite(&signature->valuestring[0], sizeof(char), 1, output);
    fwrite(&signature->valuestring[1], sizeof(char), 1, output);

    int   bfSize = file_size->valueint;
    fwrite(&bfSize, 4, 1, output);

    int   unused1 = reserved->valueint;
    int   unused2 = reserved->valueint;
    fwrite(&unused1, 2, 1, output);
    fwrite(&unused2, 2, 1, output);

    int   imageDataOffset = offset->valueint;
    fwrite(&imageDataOffset, 4, 1, output);

    int   biSize = size->valueint;
    fwrite(&biSize, 4, 1, output);

    int   width2 = width->valueint;
    int   height2 = height->valueint;
    fwrite(&width2, 4, 1, output);
    fwrite(&height2, 4, 1, output);

    int   planes2 = planes->valueint;
    fwrite(&planes2, 2, 1, output);

    int   bitPix = bit_count->valueint;
    fwrite(&bitPix, 2, 1, output);

    int   biCompression = compression->valueint;
    fwrite(&biCompression, 4, 1, output);

    int   biSizeImage = image_size->valueint;
    fwrite(&biSizeImage, 4, 1, output);

    int   biXPelsPerMeter = x_pixels_per_meter->valueint;
    int   biYPelsPerMeter = y_pixels_per_meter->valueint;
    fwrite(&biXPelsPerMeter, 4, 1, output);
    fwrite(&biYPelsPerMeter, 4, 1, output);

    int   biClrUsed = colors_used->valueint;
    int   biClrImportant = colors_important->valueint;
    fwrite(&biClrUsed, 4, 1, output);
    fwrite(&biClrImportant, 4, 1, output);

    int int_0 = 0;
    for (int i=n-1; i>=0; i-=73*3){

        for (int j=73*3-1; j>=0; j--){

            bitmap_item = cJSON_GetArrayItem(bitmap, i-j);
            fwrite(&bitmap_item->valueint, 1, 1, output);

        }
        fwrite(&int_0, 1, 1, output);

    }
    free(json_str);
    free(json);
    free(bitmap);
    free(bitmap_item);
    free(file_header);
    free(offset);
    free(signature);
    free(reserved);
    free(file_size);
    free(info_header);
    free(colors_used);
    free(size);
    free(colors_important);
    free(image_size);
    free(x_pixels_per_meter);
    free(y_pixels_per_meter);
    free(width);
    free(planes);
    free(bit_count);
    free(compression);
    free(height);
}


void task2(FILE *input, FILE *output){

    int nr_per_row = 9;
    int bit_per_nr = 7;
    int collumn_size = 73;
    char get_data[16115];
    int padding = 0;
    int n = 0;


    fread(get_data, 1, 54, input);
    fwrite(get_data, 1, 54, output);
    n += 54*2;

    for(int i=0; i<collumn_size; i++){

        for (int j=0; j<nr_per_row; j++){

            fread(get_data, 1, 3, input);
            fwrite(get_data, 1, 3, output);
            n += 6;

            fread(get_data, 1, bit_per_nr*3, input);

            fwrite(get_data+18, 1, 3, output);
            fwrite(get_data+15, 1, 3, output);
            fwrite(get_data+12, 1, 3, output);
            fwrite(get_data+9, 1, 3, output);
            fwrite(get_data+6, 1, 3, output);
            fwrite(get_data+3, 1, 3, output);
            fwrite(get_data, 1, 3, output);
            n += 3*7;

        }

        fread(get_data, 1, 3, input);
        fwrite(get_data, 1, 3, output);
        n += 6;

        fseek(input, 1, SEEK_CUR);
        fwrite(&padding, 1, 1, output);
        n += 1;

        // char hex[100];
        // if (i>53){
        //     for (int k = 0, j = 0; k<strlen(get_data); ++k, j += 2){
        //         sprintf(hex + j, "%02x", get_data[k] & 0xff);
        //     }
        //     printf("%d.   %s\n", i, hex);
        // }

    }
    printf("\n\n\nHEY!     %d\n\n\n", n);
}


int main(int argc, char* argv[]){

    char file_name[STR_ATR*2];
    char board_name[STR_ATR];
    int input_f_name_offset = 0;

    if (argc == 4){
        input_f_name_offset = 22;
    } else{
        input_f_name_offset = 14;
    }

    if (strcmp(argv[2], "123") == 0){

        strncpy(board_name, argv[1]+input_f_name_offset, 7);
        board_name[8] = '\0';
        sprintf(file_name, "output_task1_%s.bmp", board_name);
        FILE *input= fopen(argv[1], "r");
        FILE *output = fopen(file_name, "wb");
        task1(input, output);

        input = fopen(file_name, "rb");
        memset(file_name + 11, '2', 1);
        output = fopen(file_name, "wb");
        task2(input, output);
        
        fclose(input);
        fclose(output);

    }

}