
/*
 * adder.c - a minimal CGI program that adds two numbers together
 */

#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include	<inttypes.h>

#define MAXLINE 8192    /* 2<<12 */

void httpd_err() {
    printf("Content-type: text/html\r\n\r\n");
    printf("<p>Input error\r\n");
    fflush(stdout);
    exit(0);
}

int main(int argc,char *argv[]) {

    int8_t * inquire_ptr = argv[0];
    int8_t *ptr = NULL;
    int8_t arg1[MAXLINE] = {};
    int8_t arg2[MAXLINE] = {};
    int8_t content[MAXLINE] = {};
    int32_t n1 = 0;
    int32_t n2 = 0;
    
    if(strlen(inquire_ptr) == 0) {
        httpd_err();
    }

    ptr = strchr(inquire_ptr, '&');
    if(ptr == NULL) {
        httpd_err();
    }
    *ptr = '\0';
    strcpy(arg1, inquire_ptr);
    strcpy(arg2, ptr + 1);
    n1 = atoi(arg1);
    n2 = atoi(arg2);
    
    /* Make the response body */
    sprintf(content, "Welcome to add.com: ");
    sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>", 
	    content, n1, n2, n1 + n2);
    sprintf(content, "%sThanks for visiting!\r\n", content);
  
    /* Generate the HTTP response */
    //printf("Content-length: %d\r\n", (int)strlen(content));
    //printf("Content-length: %d\r\n", 9999);
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);
    exit(0);
}
