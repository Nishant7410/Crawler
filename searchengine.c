#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

#define BASE "www.chitkara.edu.in"
char *store1[50];
int k2=-1;
struct Link
{
        int Link_key;
        int Link_flag;
        char *Link_url;
//      struct Link *node_prev;
        struct Link *Link_next;
        int Link_depth;       
};

struct Hash
{
        struct Link* first;
        struct Link* last;
        int count;
};

struct Hash hash[50];

struct Link *head=NULL,*last=NULL;
void initialise()
{
        int i;
        for(i=0;i<50;i++)
        {
                hash[i].first=NULL;
                hash[i].last=NULL;
                hash[i].count=0;
        }
}

int generate_key(char *url)
{
        int i,l=strlen(url),key=0;
        //return strlen(url)%50;
        for(i=0;i<l;i++)
        {
            key+=(int)(url[i]);
        }
        return key%50;
}

void put_links_in_list(char **result,int l,int depth)
{


//for(int i=0;i<l;i++)
//printf("%s\n",result[i]);
        int i;
        struct Link *temp,*trav;
        for(i=0;i<l;i++)
        {
                temp=(struct Link*)calloc(1,sizeof(struct Link));
                temp->Link_key=generate_key(result[i]);
                int key=temp->Link_key;
                temp->Link_url=result[i];
                temp->Link_depth=depth+1;
                if(head==NULL)
                {       temp->Link_next=NULL;
                        temp->Link_flag=1;
                        temp->Link_depth=depth;
                        head=temp;                      //   temp->Link_flag=1;
                        last=temp;
                        hash[key].first=temp;
                        hash[key].last=temp;
                        hash[key].count=1;


                        continue;
                          
                      


                }
                else
                {
                        if(hash[key].first==NULL)
                        {
                               last->Link_next=temp;
                               temp->Link_next=NULL;
                               temp->Link_flag=0;
                               last=temp;
                               hash[key].first=temp;
                               hash[key].last=temp;
                               hash[key].count=1;

                       

                        }
                        else
                        {
                            trav=hash[key].first;
                            if((strcmp(hash[key].first->Link_url,temp->Link_url)==0)||(strcmp(hash[key].last->Link_url,temp->Link_url)==0))
                                {
                                    free(temp);
                                    continue;
                                }
                            int f=0;
                            while(trav->Link_next!=NULL&&trav->Link_next->Link_key==key)
                            {
                                if(strcmp(trav->Link_url,temp->Link_url)==0)
                                {
                                    free(temp);
                                    f=1;
                                    break;
                                }
                                trav=trav->Link_next;
                            }
                            if(f)
                            continue;
                            if(trav->Link_next==NULL)
                            last=temp;
                            temp->Link_flag=0;
                            temp->Link_next=trav->Link_next;
                            trav->Link_next=temp;
                            hash[key].last=temp;
                            hash[key].count++;
                        }
                }
        }

      
                       //  for(int i=0;i<l;i++)
                     //printf("%s\n",result[i]);

       // save_links_in_file(head);
}

//---------------------//


void seedcheck(char ch[])
{
char *check=(char*)calloc(1,1000);
strcat(check,"wget --spider ");
strcat(check,ch); 
if(!system(check))
printf("Link is correct");
else
{
printf("Link is incorrect");
exit(0);
}
free(check);
}
void shift()
{

FILE *fp=fopen("/home/nishant/Downloads/temp.txt","r");
FILE *fp1=fopen("/home/nishant/Downloads/chitkara.txt","w");
char ch=getc(fp);
while(ch!=EOF)
{
putc(ch,fp1);
ch=getc(fp);
}
fclose(fp);
fclose(fp1);
}

int GetNextURL(char* html, char* urlofthispage, char* result, int pos) 
{
  char c;
  int len, i, j;
  char* p1;  //!< pointer pointed to the start of a new-founded URL.
  char* p2;  //!< pointer pointed to the end of a new-founded URL.

  // NEW
  // Clean up \n chars
 // if(pos == 0) {
   // removeWhiteSpace(html);
 // }
  // /NEW

  // Find the <a> <A> HTML tag.
  while (0 != (c = html[pos])) 
  {
    if ((c=='<') &&
        ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
      break;
    }
    pos++;
  }
  //! Find the URL it the HTML tag. They usually look like <a href="www.abc.com">
  //! We try to find the quote mark in order to find the URL inside the quote mark.
  if (c) 
  {  
    // check for equals first... some HTML tags don't have quotes...or use single quotes instead
    p1 = strchr(&(html[pos+1]), '=');
    
    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10)) 
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;

    p1++;    

    p2 = strpbrk(p1, "\'\">");
    if (!p2) 
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*p1 == '#') 
    { // Why bother returning anything here....recursively keep going...

      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (!strncmp(p1, "mailto:",7))
      return GetNextURL(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4)) 
    {
      //! Nice! The URL we found is in absolute path.
      strncpy(result, p1, (p2-p1));
      return  (int)(p2 - html + 1);
    } else {
      //! We find a URL. HTML is a terrible standard. So there are many ways to present a URL.
      if (p1[0] == '.') {
        //! Some URLs are like <a href="../../../a.txt"> I cannot handle this. 
	// again...probably good to recursively keep going..
	// NEW
        
        return GetNextURL(html,urlofthispage,result,pos+1);
	// /NEW
      }
      if (p1[0] == '/') {
        //! this means the URL is the absolute path
        for (i = 7; i < strlen(urlofthispage); i++)
          if (urlofthispage[i] == '/')
            break;
        strcpy(result, urlofthispage);
        result[i] = 0;
        strncat(result, p1, (p2 - p1));
        return (int)(p2 - html + 1);        
      } else {
        //! the URL is a absolute path.
        len = strlen(urlofthispage);
        for (i = (len - 1); i >= 0; i--)
          if (urlofthispage[i] == '/')
            break;
        for (j = (len - 1); j >= 0; j--)
          if (urlofthispage[j] == '.')
              break;
        if (i == (len -1)) {
          //! urlofthis page is like http://www.abc.com/
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          //! urlofthis page is like http://www.abc.com/~xyz
          //! or http://www.abc.com
          strcpy(result, urlofthispage);
          result[len] = '/';
          strncat(result, p1, p2 - p1);
          return (int)(p2 - html + 1);
        }
        strcpy(result, urlofthispage);
        result[i + 1] = 0;
        strncat(result, p1, p2 - p1);
        return (int)(p2 - html + 1);
      }
    }
  }    
  return -1;
}


void store(char *linko,int depth)      //find unique link
{
FILE *fp=fopen("/home/nishant/Downloads/chitkara.txt","r");
struct stat st;
stat("/home/nishant/Downloads/chitkara.txt",&st);
int filesize=st.st_size;
char *arr1=(char*)calloc(1,filesize);
int k=-1;
char ch;
ch=getc(fp);
while(ch!=EOF)
{
arr1[++k]=ch;
ch=getc(fp);
}
arr1[++k]='\0';
fclose(fp);

char *url=(char*)calloc(1,150);
int n=GetNextURL(arr1,linko,url,0);
char *store[100];
int k1=-1;
k2=-1;
for(int i=0;i<100;i++)
{
store[++k1]=url;
url=(char*)calloc(1,150);
n=GetNextURL(arr1,linko,url,n);
}
for(int i=0;i<=k1;i++)
{
int t=0;
for(int k=i-1;k>=0;k--)
{
if(strcmp(store[k],store[i])==0)
{
t=1;
break;
}
}
if(t==0)
{
store1[++k2]=store[i];
}
}
//for(int i=0;i<=k2;i++)
//printf("%s\n",store1[i]);
free(url);
put_links_in_list(store1,k2+1,depth);
}

void getpage(char *url,char ch[],int depth)   //getpage
{
char urlbuffer[300]="wget -O ";
strcat(urlbuffer,ch);
strcat(urlbuffer,"/temp.txt ");
strcat(urlbuffer,url);
system(urlbuffer);

shift();
store(url,depth);
}
void depth(char ch[])
{
int d=atoi(ch);
int flag=0;
for(int i=0;ch[i]!='\0';i++)
{
if(ch[i]>='0'&&ch[i]<='9')
flag=0;
else
{
flag=1;
break;
}
}
if(flag==0)
{
if(d>=1&&d<=5)
printf("depth is correct\n");
else
{
printf("depth is not correct\n");
exit(0);
}
}
else
{
printf("incorrect numeric value\n");
exit(0);
}
}
void testDir(char *dir)
{
  struct stat statbuf;
  if ( stat(dir, &statbuf) == -1 ) 
  {
    fprintf(stderr, "-----------------\n");
    fprintf(stderr, "Invalid directory\n");
    fprintf(stderr, "-----------------\n");
    exit(1);
  }

  //Both check if there's a directory and if it's writable
  if ( !S_ISDIR(statbuf.st_mode) ) 
  {
    fprintf(stderr, "-----------------------------------------------------\n");
    fprintf(stderr, "Invalid directory entry. Your input isn't a directory\n");
    fprintf(stderr, "-----------------------------------------------------\n");
    exit(1);
  }

  if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR ) 
  {
    fprintf(stderr, "------------------------------------------\n");
    fprintf(stderr, "Invalid directory entry. It isn't writable\n");
    fprintf(stderr, "------------------------------------------\n");
    exit(1);
  }
}

/*void removeWhiteSpace(char* html) 
{
  int i;
  char *buffer = malloc(strlen(html)+1), *p=malloc (sizeof(char)+1);
  memset(buffer,0,strlen(html)+1);
  for (i=0;html[i];i++) 
  {
    if(html[i]>32)
    {
      sprintf(p,"%c",html[i]);
      strcat(buffer,p);
    }
  }
  strcpy(html,buffer);
  free(buffer); free(p);
}
*/



void check_link_validity(int argc,char *argv[])
{
if(argc!=4)
{
printf("enter correct Values");
exit(0);
}
else
{
seedcheck(argv[1]);
depth(argv[2]);
testDir(argv[3]);
}
}
/*char* get_next_url(struct Link *head,int depth)
{

        struct Link *temp=head;
        while(temp!=NULL)
        {
                if(temp->Link_flag==0&&temp->Link_depth==depth)
                {
                        temp->Link_flag=1;
                        return temp->Link_url;
                }
                temp=temp->Link_next;
        }
        return NULL;
} */
/*void print_list(struct Link *head)
{
    struct Link *temp=head;
    while(temp!=NULL)
    {
        printf("key = %d Link = %s depth = %d flag= %d\n",temp->Link_key,temp->Link_url,temp->Link_depth,temp->Link_flag);
        temp=temp->Link_next;
    }

}*/
int main(int argc,char *argv[])
{
check_link_validity(argc,argv);
initialise();
getpage(argv[1],argv[3],0);
int depth=1;
        char *url=NULL;
       /* while(depth<atoi(argv[2]))
        {
                while((url=get_next_url(head,depth))!=NULL)
                {
                        getpage(url,argv[3],depth);
                }
                depth++;
        }*/
       // print_list(head);


return 0;
}

