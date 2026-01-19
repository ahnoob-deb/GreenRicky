#ifndef HOF_DATA_H
#define HOF_DATA_H 1

#define MAX_NAME_LEN 20
#define MAX_SCORE_LEN 10
#define MAX_TOPS 10
#define MAX_LINE_LENGTH 50
#define HOF_DELIMITER ";"

typedef struct {

    char score[MAX_SCORE_LEN];
    char name[MAX_NAME_LEN];

} HSCData;

#endif
