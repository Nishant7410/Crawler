#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>

int page = 0;
typedef struct countnode
{
  char countfile[50];
  char url[150];
  int count;
  struct countnode *countnext;
}countnode;
typedef struct wordnode
{
  char word[30];
  int word_key;
  struct wordnode *next;
  struct countnode *list;
}wordnode;
wordnode *head = NULL,*last=NULL;
countnode *tail1=NULL;

struct hash
{
  struct wordnode *first;
  struct wordnode *last;
};
struct hash h[50];
void initialise()
{
for(int i=0;i<50;i++)
{
h[i].first=NULL;
h[i].last=NULL;
}
}
int hash_func(char s[])
{
			int i=0,sum=0;
			for(i=0;s[i]!='\0';i++)
			{
				sum=sum+s[i];
			}
			return (sum%50);
}
wordnode* check_duplicate(wordnode *k)
{
  wordnode *chk = head;
  while(chk!=NULL)
  {
    if(strcmp(chk->word,k->word)==0)
    {
      return chk;
    }
    chk=chk->next;
  }
  return NULL;
}


/*<--------------------------------------->*/
    void pagecount(wordnode *point)
     {
     point->list=NULL;

            for(int i=1;i<=10;i++)
            {
              countnode *countTemp=NULL;
                     countTemp = (countnode*)calloc(1,sizeof(countnode));
              int c=0;
             char indexarr[5]={0};
             sprintf(indexarr,"%d",i);
             char str[100]={0};
             strcat(str,"/home/nishant/Documents/");
             strcat(str,indexarr);
             strcat(str,".txt");
             strcpy(countTemp->countfile,str);
             //countTemp->pageno = page;
               struct stat st;
               stat(str,&st);
               int file_size  = st.st_size;
               char *file = (char*)calloc(1,file_size);
               FILE *f1 = fopen(str,"r");
               char ch1;
               int a=0;
               while((ch1=fgetc(f1))!=EOF)
               {
                 file[a++]=ch1;
               }
             file[a]='\0';
             char ch[100]={0};
             int lk=-1;
             for(int i=0;i<strlen(file);i++)
                {
                  if(file[i]=='\n')
                   break;
                   ch[++lk]=file[i];
                }
             ch[++lk]='\0';
             strcpy(countTemp->url,ch);
             char *tstart,*tend;
             for(int i=0;i<file_size;i++)
             {
               if(file[i]=='<'&&file[i+1]=='t')
                 {
                   tstart = &file[i];
                   tstart = strpbrk(tstart,">");
                   tstart++;
                   break;
                  }
              }
             tend = strpbrk(tstart,"<");
              int d = tend-tstart;
    //printf("%d\n",d);
              char *str1 = (char*)calloc(1,d);
              int k1=0;
              char *start = tstart;
              for(;start!=tend;start++)
              str1[k1++]=*start;
              str1[k1]='\0';
              char *wrd = (char*)calloc(1,30);
              int z=0,j=0,f=0;
              char *array[50];
              int k=-1;
              for(z=0;z<=strlen(str1);z++)
              {
              if(str1[z]==' '||str1[z]==','||str1[z]==':'||str1[z]=='.'||str1[z]=='\0')
              {
                if(f==1)
                 {
                  wrd[j]='\0';
                   f=0;
                   if((wrd[0]>='A'&&wrd[0]<='Z')||(wrd[0]>='a'&&wrd[0]<='z'))
                    {
                      if(strlen(wrd)>2)
                          {
                           array[++k] = (char*)calloc(1, strlen(wrd));
                           strcpy(array[k], wrd);

                            }
                       }
                       j=0;
                    }
                }
            else
            {
               wrd[j++]=str1[z];
               f=1;
            }

           }
           for(int p=0;p<=k;p++)
               {
               if(strcmp(point->word,array[p])==0)
               c++;
               }
           countTemp->count=c;
           if(c==0)
           {
            free(countTemp);
	           continue;
           }
            if(point->list == NULL)
             {
              point->list=countTemp;
            tail1=countTemp;
             }
            else
             {
             tail1->countnext=countTemp;
             tail1=tail1->countnext;
             tail1->countnext=NULL;
              }
}
     }


void create_node(char wrd[50])
{
// printf("head");
      wordnode *temp=NULL;
      temp = (wordnode*)calloc(1,sizeof(wordnode));
      strcpy(temp->word,wrd);
      int index = hash_func(wrd);
      temp->word_key = index;
      temp->next=NULL;
      if(head==NULL)
      {
            head = temp;
            last=temp;
            h[index].first = temp;
           // h[index].last = temp;
            pagecount(head);
          }
          else
          {
            wordnode *link = check_duplicate(temp);
              if(link==NULL)
              {
                //create new because it donot exist earler
                          if(h[index].first==NULL)
                          {
                            last->next=temp;
                            last=last->next;
                            last->next=NULL;
                            h[index].first = temp;
                            pagecount(last);
                          }
                          else
                          {

                //this is to insert the node at the for the same key   <---------------------------------------------->
                           wordnode *trav=NULL;
                          trav=(wordnode*)calloc(1,sizeof(wordnode));
                           trav=h[index].first;


                           if(trav->next==NULL)
                             {
                              trav->next=temp;
                              trav=trav->next;
                              trav->next=NULL;
                              //trav->list=NULL;
                              pagecount(trav);
                              h[index].first = temp;

                             }
                             else
                             {
                              wordnode *temp1=NULL;
                              temp1 = (wordnode*)calloc(1,sizeof(wordnode));
                              temp1=trav->next;
                              trav->next=temp;
                              temp->next=temp1;
                              //temp->list=NULL;
                              pagecount(temp);
                              h[index].first = temp;
                             }

              }
          }
              else
              {

                free(temp);
          }






//<--------------------------------------------------------------------->


}
}
void extract_word(char *start,char *end)
{
    int d = end-start;
    //printf("%d\n",d);
    char *str = (char*)calloc(1,d);
    int k=0;
    char *temp = start;
    for(;temp!=end;temp++)
    str[k++]=*temp;
    str[k]='\0';
    char wrd[50];
    int i=0,j=0,f=0;
    for(i=0;i<=strlen(str);i++)
    {
   if(str[i]==' '||str[i]==','||str[i]==':'||str[i]=='.'||str[i]=='\0')
   {
   if(f==1)
   {
   wrd[j]='\0';
    f=0;
    if((wrd[0]>='A'&&wrd[0]<='Z')||(wrd[0]>='a'&&wrd[0]<='z'))
            {
              if(strlen(wrd)>2)
                {
                 create_node(wrd);

            }
          }
   }
  j=0;
   }
   else
   {
   wrd[j++]=str[i];
   f=1;
    }

}
}
void print()
{
  wordnode *p=NULL;
  p=head;
  FILE *save=fopen("/home/nishant/Downloads/savelist.txt","w");
  while(p!=NULL)
  {
    printf("%s== ",p->word);
    fprintf(save,"%s ",p->word);
    countnode *k = p->list;
    while(k!=NULL)
    {
      printf(" %d %s %s",k->count,k->countfile,k->url);
      fprintf(save,"%d ",k->count);
      fprintf(save,"%s ",k->countfile);
      if(k->countnext==NULL)
      fprintf(save,"%s\n",k->url);
      else
      fprintf(save,"%s ",k->url);
      k=k->countnext;
    }
    printf("\n\n\n");
    p=p->next;

  }
  fclose(save);
}
void read_title(char *file,int file_size)
{
    char *tstart,*tend;
    for(int i=0;i<file_size;i++)
    {
      if(file[i]=='<'&&file[i+1]=='t')
      {
        tstart = &file[i];
        tstart = strpbrk(tstart,">");
        tstart++;
        break;
      }
    }
    tend = strpbrk(tstart,"<");
    extract_word(tstart,tend);



}


void read_heading(char *file,int file_size)
{

  char *hstart,*hend;
  for(int i=0;i<file_size;i++)
  {
    int flag=0;
    if(file[i]=='<'&&file[i+1]=='h'&&(file[i+2]>='1'&&file[i+2]<='6'))
    {
      hstart = &file[i];
      hstart = strpbrk(hstart,">");
      hstart++;
      hend = strpbrk(hstart,"<");
      extract_word(hstart,hend);
    }
  }


}

void extract_file(char *str)
{
  struct stat st;
  stat(str,&st);
  int file_size  = st.st_size;
  char *file = (char*)calloc(1,file_size);
  FILE *temp = fopen(str,"r");
  char ch;
  int i=0;
  while((ch=fgetc(temp))!=EOF)
  {
      file[i++]=ch;
  }
  file[i]='\0';
  read_title(file,file_size);
  //read_heading(file,file_size);
  
}
void retrieve()
{
 /* initialise();
  printf("retrueve");
  FILE *f=fopen("savelist.txt","r");
  char str[500];
  int k=-1;
  char ch;
  while((ch=fgetc(f))!=EOF)
{
  if(ch=='\n')
  {
    str[++k]='\0';
    char wrd[300];
    int s=-1;
    int f=0;
    for(int i=0;i<=strlen(str);i++)
    {
      if(str[i]==' ')
      {
        wrd[++s]='\0';
        break;
      }
      else
      {
        wrd[++s]=str[i];
        f=1;
      }
    }
    wordnode *temp=NULL;
    temp = (wordnode*)calloc(1,sizeof(wordnode));
    strcpy(temp->word,wrd);
    int index = hash_func(wrd);
    temp->word_key = index;
    temp->next=NULL;
   if(head==NULL)
    {
      head = temp;
      last=temp;
      h[index].first = temp;
     // h[index].last = temp;
      pagecount(head);
    }
    else
    {
      wordnode *link = check_duplicate(temp);
        if(link==NULL)
        {
          //create new because it donot exist earler
                    if(h[index].first==NULL)
                    {
                      last->next=temp;
                      last=last->next;
                      last->next=NULL;
                      h[index].first = temp;
                      pagecount(last);
                    }
                    else
                    {

          //this is to insert the node at the for the same key   <---------------------------------------------->
                     wordnode *trav=NULL;
                    trav=(wordnode*)calloc(1,sizeof(wordnode));
                     trav=h[index].first;


                     if(trav->next==NULL)
                       {
                        trav->next=temp;
                        trav=trav->next;
                        trav->next=NULL;
                        //trav->list=NULL;
                        pagecount(trav);
                        h[index].first = temp;

                       }
                       else
                       {
                        wordnode *temp1=NULL;
                        temp1 = (wordnode*)calloc(1,sizeof(wordnode));
                        temp1=trav->next;
                        trav->next=temp;
                        temp->next=temp1;
                        //temp->list=NULL;
                        pagecount(temp);
                        h[index].first = temp;
                       }

        }
    }
        else
        {

          free(temp);
    }






//<--------------------------------------------------------------------->


}
k=-1;
  }
  else
  {
    str[++k]=ch;
  }
}
fclose(f);*/
 int i;
    initialise();
    for(i=1;i<=10;i++)
    {
      char indexarr[5]={0};
      sprintf(indexarr,"%d",i);
      char str[100]={0};
      strcat(str,"/home/nishant/Documents/");
      strcat(str,indexarr);
      strcat(str,".txt");
      extract_file(str);

    }
}

int main()
{
    int i;
    initialise();
    char c;
    printf("for retrieve(r) and for new data(n)");
    scanf("%c",&c);
    if(c=='n')
    {
    for(i=1;i<=10;i++)
    {
      char indexarr[5]={0};
      sprintf(indexarr,"%d",i);
      char str[100]={0};
      strcat(str,"/home/nishant/Documents/");
      strcat(str,indexarr);
      strcat(str,".txt");
      extract_file(str);
     print();
    }
  }
  else{
    retrieve();
    print();
  }
}
