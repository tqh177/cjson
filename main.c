#include <stdio.h>

#include "json.h"

int main(){
   object *window;
   char buf[2048];
   FILE *fp;
   int i = 0;
   fp = fopen("test.json", "r");
   if (!fp)
   {
      puts("文件不存在");
      return -1;
   }
   do
   {
      buf[i] = fgetc(fp);
      if (i == sizeof(buf))
         return -1;
   } while (buf[i++] != EOF);
   buf[i] = 0;
   window = json_decode(buf);
   if (window)
      puts(json_encode(window));
   else
   {
      puts("NULL");
   }
	return 0;
}