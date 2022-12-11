#include "./manipulate_string.h"
#include <stdio.h>
#include <stdlib.h>

int count_char(char *str, char sig){
    int i, counter = 0, len = strlen(str);

    for(i = 0; i < len; i++){
        if(str[i] == sig){
            counter++;
        }
    } 

    return counter;
}

char **split(char *str, char divider){
    int i, j = 0, y = 0, len = strlen(str);

    char **tmp = (char **) malloc(sizeof(char *) * (count_char(str, divider) + 1));

    for(i = 0; i < count_char(str, divider) + 1; i++){
        tmp[i] = (char *) malloc(sizeof(char) * len);
        memset(tmp[i], 0, len);
    }

    for(i = 0; i < len; i++){
        if(str[i] != divider){
            tmp[j][y] = str[i];
            y++;
        }else if(str[i] == divider){
            y = 0;
            j++;
        }
    }

    return tmp;
}

char *reverse(char *str){
    int i, j, len = strlen(str); 
    char *tmp = (char *) malloc(sizeof(char) * (len + 1));

    j = len - 1;

    for(i = 0; i < len; i++){
        tmp[i] = str[j];
        j--;
    }

    return tmp;
}

char *substr(char *str, int start, int end){
    int i, j = start; 

    char *sub; 

    if(end > strlen(str) || end < start){
        end = strlen(str);
    }

    sub = (char *) malloc(sizeof(char) * (end - start));

    for(i = 0; i < (end - start); i++){
        sub[i] = str[j];
        j++;
    }

    return sub;
}

char *delete_space(char *str){
    int n = count_char(str, ' '), i;
    char **part = split(str, ' ');

    char *s = malloc(sizeof(char) * (strlen(str) - n));

    sprintf(s, "%s", part[0]);

    for(i = 1; i < n + 1; i++){
        strcat(s, part[i]);
    }

    return s;
}

int str_regex_contains(char *str, char *pattern){
	regmatch_t match[_MATCH_SIZE_];
    regex_t regex;

    if(str == NULL){
	    return -1;
	}

    if(regcomp(&regex, pattern, REG_ICASE | REG_EXTENDED) != 0){
        regfree(&regex);         
  
        return -1;
    }

    if(!regexec(&regex, str, (size_t) _MATCH_SIZE_, match, 0)){
        fprintf(stderr, "Pattern match from %d to %d\n", match[0].rm_so, match[0].rm_eo);

	    regfree(&regex);
	
	    return 1;
    }else{
        regfree(&regex);

	    return 0;
	}
}
