#include<stdio.h> 
#include<pthread.h> 
#include<stdlib.h> 
#include<unistd.h> 
 
pthread_mutex_t mutex, wrthr; 
int global_variable, rc = 0; 
 
void *writer(void *args){ 
        pthread_mutex_lock(&wrthr); 
        int no = rand() % 10; 
        int dint = ((int)args); 
        printf("--------------------------------------------------\n"); 
        printf("w%d waits for %d\n", dint, no); 
        sleep(no); 
        printf("Enter no. of time w%d wants to write\n", dint); 
        int tyme; 
        scanf("%d", &tyme); 
        printf("w%d is writing\n", dint); 
        int j; 
        for(j=0; j<tyme; j++){ 
                printf("Enter the %dth value to write\n", (j+1)); 
                int updated_value; 
                scanf("%d", &updated_value); 
                global_variable = global_variable + updated_value; 
        } 
        printf("UPDATED value of Shared variable = %d \n", global_variable); 
        printf("--------------------------------------------------\n"); 
        pthread_mutex_unlock(&wrthr); 
} 
 
void *reader(void *args){ 
        //Entry Part 
        pthread_mutex_lock(&mutex); 
        rc++; 
        if(rc==1){ 
                pthread_mutex_lock(&wrthr);//No writer should come 
        } 
        pthread_mutex_unlock(&mutex); 
        int no = rand() % 10; 
        int dint = ((int)args); 
        printf("r%d waits for  %d\n", dint, no); 
        sleep(no); 
        printf("Enter the number of time r%d wants to read:\n", dint); 
        int tyme,j; 
        scanf("%d", &tyme); 
        printf("r%d is reading....\n", dint); 
        for(j=0; j<tyme; j++){ 
                printf("r%d reads the global_value = %d\n", dint, global_variable); 
        } 
        printf("No. of readers active = %d\n", rc); 
        pthread_mutex_lock(&mutex); 
        rc--; 
        if(rc==0){ 
                pthread_mutex_unlock(&wrthr); 
        } 
        pthread_mutex_unlock(&mutex); 
} 

 
void main(){ 
        printf("Enter the first value of share variable: \n"); 
        scanf("%d", &global_variable); 
        printf("---------------------------------------------\n"); 
        int no_of_readers, no_of_writers, k; 
        printf("Enter no. of Reader:\n"); 
        scanf("%d", &no_of_readers); 
        for(k=0; k<no_of_readers; k++){ 
                printf("r%d\n", k); 
        } 
        printf("---------------------------------------------\n"); 
        printf("Enter no. of Writer:\n"); 
        scanf("%d", &no_of_writers); 
        for(k=0; k<no_of_writers; k++){ 
                printf("w%d\n", k); 
        } 
        printf("---------------------------------------------\n"); 
 
        pthread_t r[no_of_readers], w[no_of_writers]; 
        pthread_mutex_init(&wrthr, NULL); 
        pthread_mutex_init(&mutex, NULL); 
 
        if(no_of_readers<0 || no_of_writers<0){ 
                printf("You have Entered NEGATIVE number of READER or WRITER\n"); 
                printf("try again"); 
                return; 
        }else if(no_of_readers == 0){ 
                printf("Warning: You have not taken any READER\n"); 
                printf("No READER thread will be creaded\n"); 
        }else if(no_of_writers == 0){ 
                printf("Sorry: You have not taken any WRITER\n"); 
                printf("So no WRITER thread will be creaded\n"); 
        }else{ 
                printf("=====Thread Creating....=====\n"); 
        } 
        printf("---------------------------------------------\n"); 
 
        if(no_of_writers==no_of_readers){ 
                for(k=0; k<no_of_writers; k++){ 
                        pthread_create(&w[k], NULL, &writer, (int *)k); 
                        pthread_create(&r[k], NULL, &reader, (int *)k); 
                } 
                for(k=0; k<no_of_writers; k++){ 
                        pthread_join(w[k], NULL); 
                        pthread_join(r[k], NULL); 
                } 
        }else if(no_of_writers>no_of_readers){ 
                for(k=0; k<no_of_readers; k++){ 
                        pthread_create(&w[k], NULL, &writer, (int *)k); 
                        pthread_create(&r[k], NULL, &reader, (int *)k); 
                } 
                for(k=no_of_readers; k<no_of_writers; k++){ 
                        pthread_create(&w[k], NULL, &writer, (int *)k); 
                } 
                for(k=0; k<no_of_readers; k++){ 
                        pthread_join(w[k], NULL); 
                        pthread_join(r[k], NULL); 
                } 
                for(k=no_of_readers; k<no_of_writers; k++){ 
                        pthread_join(w[k], NULL); 
                } 
        }else{ 
                for(k=0; k<no_of_writers; k++){ 
                        pthread_create(&w[k], NULL, &writer, (int *)k); 
                        pthread_create(&r[k], NULL, &reader, (int *)k); 
                } 
                for(k=no_of_writers; k<no_of_readers; k++){ 
                        pthread_create(&r[k], NULL, &reader, (int *)k); 
                } 
                for(k=0; k<no_of_writers; k++){ 
                        pthread_join(w[k], NULL); 
                        pthread_join(r[k], NULL); 
                } 
                for(k=no_of_writers; k<no_of_readers; k++){ 
                        pthread_join(r[k], NULL); 
                } 
         
        printf("Final val. of global_variable = %d\n", global_variable); 
} 
 
