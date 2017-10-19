/*************************************************************************
	> File Name: red_simple.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月23日 星期六 21时23分52秒
 ************************************************************************/

#include "red_simple.h"

char* get_rand_str(char s[], int num)
{
    char *str = "abcdefghigklmnopqrstuvwxyz";
    int i, lstr;
    char ss[2] = {0};
    lstr = strlen(str);

    srand((unsigned int)time((time_t *)NULL));
    for(i = 1; i <= num; i++){
        sprintf(ss, "%c", str[(rand() % lstr)]);
        strcat(s, ss);
    }

    return s;
}

int main(void)
{
    int err;
    pthread_t productor;
    pthread_mutex_init(&mutex, 0);
    
    pro *r_p = (pro *)malloc(sizeof(pro));

    if((err = pthread_create(&productor, NULL, red_give, (void*)&r_p)) != 0){
        perror("pthread create error");
    }
    pthread_join(productor);
    printf("please input people:");

    pthread_t *customer = (pthread_t *)malloc(sizeof(cus) * number);
    
    int i;
    char s[5] = {'0'};
    r_c = (cus*)malloc(sizeof(cus) * number); 
    for(i = 0; i < number; i++){
        strcpy(r_c[i].name, get_rand_str(s, 5));
        memset(s, 0, sizeof(s));
        r_c[i].have = 0;
    }

    int *arg = (int*)malloc(sizeof(int) * number);
    for(i = 0; i < number; i++){
        arg[i] = i;
        if(r_c[i].have == 0){
            if((err = pthread_create(&customer[i], NULL, red_get, (void*)&arg[i])) != 0){
                perror("pthread create error");
            }
        }
    }

    for(i = 0; i < number; i++){
        pthread_join(customer[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
