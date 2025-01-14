#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h> 

void *agentAction();
void *tobaccoAction();
void *paperAction();
void *matchAction();

pthread_mutex_t lock         = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t agent_lock   = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t match_lock   = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t paper_lock   = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tobacco_lock = PTHREAD_MUTEX_INITIALIZER;

int paper = 0, tobacco = 0, match = 0;
int counter = 10;

int main() {
    pthread_t agent, smoker_tobacco, smoker_match, smoker_paper;

    srand(time(0));

    pthread_mutex_lock  (&agent_lock);
    pthread_mutex_lock  (&match_lock);
    pthread_mutex_lock  (&paper_lock);
    pthread_mutex_lock  (&tobacco_lock);
    pthread_mutex_unlock(&lock);

    pthread_create(&agent,          NULL, &agentAction,   NULL);
    pthread_create(&smoker_tobacco, NULL, &tobaccoAction, NULL);
    pthread_create(&smoker_paper,   NULL, &paperAction,   NULL);
    pthread_create(&smoker_match,   NULL, &matchAction,   NULL);

    pthread_join(agent, NULL);

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&agent_lock);
    pthread_mutex_destroy(&tobacco_lock);
    pthread_mutex_destroy(&paper_lock);
    pthread_mutex_destroy(&match_lock);

    return 0;
}

void* agentAction() {
    while(counter > 0) {
        pthread_mutex_lock(&lock);
        printf("\n\nAgent turn: %d:\n", counter);
        int randNum = (rand() % 3) + 1;
        if (randNum == 1) {
            tobacco++;
            paper++;
            printf("Agent gives tobacco and paper.\n");
            pthread_mutex_unlock(&match_lock); // Wake up smoker with match
        } else if (randNum == 2) {
            tobacco++;
            match++;
            printf("Agent gives tobacco and match.\n");
            pthread_mutex_unlock(&paper_lock); // Wake up smoker with paper
        } else {
            match++;
            paper++;
            printf("Agent gives match and paper.\n");
            pthread_mutex_unlock(&tobacco_lock); // Wake up smoker with tobacco
        }
        pthread_mutex_unlock(&lock);
        pthread_mutex_lock(&agent_lock); // Agent sleeps
        counter--;
    }
    return NULL;
}

void *tobaccoAction() {
    while(1){
        pthread_mutex_lock(&tobacco_lock); // Put to sleep immediately, waiting for unlock
        pthread_mutex_lock(&lock); // Wait for CS to be available
        paper--;
        match--;
        printf("Tobacco Smoker took them to smoke.\n");
        pthread_mutex_unlock(&agent_lock); // Wake Agent up
        pthread_mutex_unlock(&lock); // Release CS
    }
}

void *paperAction() {
    while(1){
        pthread_mutex_lock(&paper_lock); // Put to sleep immediately, waiting for unlock
        pthread_mutex_lock(&lock); // Wait for CS to be available
        tobacco--;
        match--;
        printf("Paper smoker took them to smoke.\n");
        pthread_mutex_unlock(&agent_lock); // Wake Agent up
        pthread_mutex_unlock(&lock); // Release CS
    }
}

void *matchAction() {
    while(1){
        pthread_mutex_lock(&match_lock); // Put to sleep immediately, waiting for unlock
        pthread_mutex_lock(&lock); // Wait for CS to be available
        tobacco--;
        paper--;
        printf("Match smoker took them to smoke.\n");
        pthread_mutex_unlock(&agent_lock); // Wake Agent up
        pthread_mutex_unlock(&lock); // Release CS
    }
}
