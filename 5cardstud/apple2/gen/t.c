/* Generate charset template */

/* gcc -ot t.c */

#include <stdio.h>

void main(void)
{
  FILE *fp = fopen("charset.h","w");

  fputs("#ifndef CHARSET_H\n#define CHARSET_H\n\n",fp);
  fputs("const char charset[]=\n",fp);

  for (int i=0;i<128;i++)
    {
      fprintf(fp,"\t/* 0x%02X */\n",i);
      fputs("\t\"       \"\n",fp);
      fputs("\t\"       \"\n",fp);
      fputs("\t\"       \"\n",fp);
      fputs("\t\"       \"\n",fp);
      fputs("\t\"       \"\n",fp);
      fputs("\t\"       \"\n",fp);
      fputs("\t\"       \"\n",fp);
      fputs("\t\"       \"\n\n",fp);
    }

  fprintf(fp,";\n\n#endif /* CHARSET_H */");  
  
  fclose(fp);
}
