#include <iostream>
#include <mysql/mysql.h>
#include <string>
using namespace std;
MYSQL mysql, *connection;
MYSQL_RES *result;
MYSQL_ROW row;

int len, size;
char data[1000 * 1024];
char chunk[2 * 1000 * 1024 + 1];
char query[1024 * 5000];

FILE *fp;

char *ip = (char *)"192.168.137.104";
char *usr = (char *)"pi";
char *pass = (char *)"josekroos10";
char *db = (char *)"coba";

int query_state;

int main(int argc, char **argv) {
  mysql_init(&mysql);
  connection = mysql_real_connect(&mysql, ip, usr, pass, db, 0, NULL, 0);
  if (connection == NULL)
    cout << mysql_error(&mysql) << endl;
  else {
    printf("Database connection successful\n");
  }

  return 0;
}
