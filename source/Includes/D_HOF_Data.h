#ifndef HOF_DATA_H
#define HOF_DATA_H 1

/* max len of name in highscore-table inclusive \0 */
#define MAX_NAME_LEN 20
/* max len of score in highscore-table inclusive \0 */
#define MAX_SCORE_LEN 10
/* count of top-positions */
#define MAX_TOPS 10
/* max length of a line in hof.txt */
#define MAX_LINE_LENGTH 50
/* the delimiter in the hof.txt */
#define HOF_DELIMITER ";"

/* a record of a highscore-entry */
typedef struct {

    char score[MAX_SCORE_LEN]; // reserve MAX_SCORE_LEN chars for the score
    char name[MAX_NAME_LEN]; // reserve MAX_NAME_LEN chars for the name

} HSCData_t;

#endif
