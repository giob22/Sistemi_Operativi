#include <stdio.h>

int main(){
    int matrice[3][3];

    for(int i = 0; i < 3 ; i++){
        for(int j = 0; j < 3; j++){
            matrice[i][j] = i * 3 + j;
        }
    }
    for(int i = 0; i < 3 ; i++){
        for(int j = 0; j < 3; j++){
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < 3*3; i++){
        printf("%d ", matrice[i/3][i%3]);
        if (i%3 == 2){
            printf("\n");
        }
    }


    return 0;

}

