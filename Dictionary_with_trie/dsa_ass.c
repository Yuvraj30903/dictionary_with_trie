#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define clrscr() printf("\e[1;1H\e[2J")
struct node
{
    struct node *child[26];
    int m;
    char meaning[200];
};
struct node *root = NULL;
void load()
{
    root = malloc(sizeof(struct node));
    root->m = 0;
    for (int i = 0; i < 26; i++)
        root->child[i] = NULL;

    struct node* temp = root;
    FILE *d=fopen("dict.txt","r+");
    char word[20];
    char mean[200];
    while((fscanf(d,"%s",word))!=EOF)
    {
        fgets(mean,200,d);
        int word_l = strlen(word);
        for (int i = 0; i < word_l; i++)
        {
            int ind;
            ind = (int)tolower(word[i]) - 'a';
            if(!(isalpha(word[i]))){
                printf("Invalid input from file");
                return;
                }
            if (ind > 25 || ind < 0 )
                continue;
            if (temp->child[ind] == NULL)
            {
                temp->child[ind] = malloc(sizeof(struct node));
                temp->child[ind]->m = 0;
                for (int j = 0; j < 26; j++)
                    temp->child[ind]->child[j] = NULL;
                temp = temp->child[ind];
            }
            else
            {
                temp = temp->child[ind];
            }
            if (i == word_l - 1)
            {
                temp->m = 1;
                strcpy(temp->meaning, mean);
            }
        }
        temp=root;
    }
    fclose(d);
}
void unload(struct node* t)
{
    struct node* tmp = t;
    for (int i = 0; i < 26; i++)
        if (tmp -> child[i] != NULL)
            unload(tmp -> child[i]);

    free (tmp);
    tmp = NULL;
}
void lexo(struct node* t,char str[20])
{
    if(t->m==1 && t!=root)
        printf("%s ",str);
    for (int i = 0; i < 26; i++)
    {
        if(t->child[i]!=NULL)
        {
            char temp[20];
            strcpy(temp,str);
            char ch=(char)(i+97);

            lexo(t->child[i],strncat(temp,&ch,1));
        }
    }
}
void add()
{
    int t;
    char word[20];
    char mean[200];
    printf("===========================");
    printf("\n->Enter a word to add:");
    scanf("%s", word);
    fflush(stdin);
    printf("->Enter the meaning:");
    gets(mean);
    printf("===========================\n");

    struct node *temp = root;
    int word_l = strlen(word);
    for (int i = 0; i < word_l; i++)
    {
        int ind;
        ind = (int)tolower(word[i]) - 'a';
        if(!(isalpha(word[i]))){
                printf("Invalid input");
                return;
        }
        if (ind > 25 || ind < 0 )
            continue;
        if (temp->child[ind] == NULL)
        {

            temp->child[ind] = malloc(sizeof(struct node));
            temp->child[ind]->m = 0;
            for (int j = 0; j < 26; j++)
                temp->child[ind]->child[j] = NULL;

            temp = temp->child[ind];
        }
        else
        {
            temp = temp->child[ind];
        }
        if (i == word_l - 1)
        {
            if(temp->m)
            {
                t=1;
                continue;
            }
            temp->m = 1;
            strcpy(temp->meaning, mean);
        }
    }
    if(t==1)
    {
        printf("Word is already added !!\n");
        return;
    }
    FILE *d = fopen("dict.txt", "a");
    fprintf(d, "%s ", word);
    fputs(strcat(mean,"\n") , d);
    fclose(d);
}
void view()
{
    FILE *d = fopen("dict.txt", "r");
    char word_str[20];
    char meaning[200];
    printf("\n===============================================\n");
    while ((fscanf(d, "%s", word_str)) != EOF)
    {
        fgets(meaning, 200, d);

        printf("->%s :", word_str);
        printf(" %s", meaning);
    }
    printf("===============================================\n");
    // printf("]");
    fclose(d);
}

int havechidren(struct node* nd)
{
    for (int i = 0; i < 26; i++)
    {
        if(nd->child[i]!=NULL)
        {
            // printf("%d-",i+1);
            return 1;
        }
    }
    return 0;
}
int j=2;
void did(struct node* t,char str[20])
{
    if  (j==0)
        return;
    if(t->m==1 && t!=root){
        printf("%s ",str);
        --j;
        }
    for (int i = 0; i < 26; i++)
    {
        if(t->child[i]!=NULL)
        {
            char temp[20];
            strcpy(temp,str);
            char ch=(char)(i+97);

            did(t->child[i],strncat(temp,&ch,1));
        }
    }
}
void didyoumean(struct node * t,char str[20])
{
    printf("Did you mean....:");
    did(t,str);
}
void search(char c[20])
{
    struct node *temp=root;
    int i=0;
    for (i = 0; c[i]!='\0'; i++)
    {
        int ind=(int)tolower(c[i])-'a';

        if(temp->child[ind]==NULL)
        {

            printf("Word not found!!\n");
            if (i>2)
            {
            c[i]='\0';
            didyoumean(temp,c);
            }
            return;
        }

        temp=temp->child[ind];
    }

    if(temp->m)
    {
        printf("%s:%s\n",c,temp->meaning);
    }
    else
    {
        printf("word not found!!\n");
        if (i>2)
        {
            c[i]='\0';
            didyoumean(temp,c);
        }
    }

}
void delete(struct node* temp,int wl,char c[])
{
    int ind=(int)tolower(c[wl])-'a';
    if (wl==strlen(c))
    {
        if (temp->m==1)
        {
            temp->m=0;
            // temp->meaning[0]='\0';
            int re=rename("dict.txt","dict1.txt");


            FILE *d1=fopen("dict1.txt","r+");
            FILE *d=fopen("dict.txt","a");
            char word[20];
            char mean[200];
            while((fscanf(d1,"%s",word))!=EOF)
            {
                fgets(mean,200,d1);
                if (stricmp(word,c)==0)
                    continue;
                fprintf(d, "%s", word);
                fputs(mean , d);

            }
            fclose(d);
            fclose(d1);
            remove("dict1.txt");

        }
        else{
            printf("Word not found!!\n");
            if (wl>2)
            {
                c[wl]='\0';
                didyoumean(temp,c);
            }
        }
        // printf("%s",temp->meaning);
        if(havechidren(temp)==0)
        {
            // printf("y");
            temp=NULL;
            free(temp);
        }
        return;
    }
    if(temp->child[ind]==NULL)
    {

        printf("Word not found!!\n");
        if (wl>2)
        {
            // printf("%d",wl);
            c[wl]='\0';
            didyoumean(temp,c);
        }
        return;
    }
    delete(temp->child[ind],++wl,c);
    if(havechidren(temp->child[ind])==0 && temp->child[ind]->m==0)
    {
        // printf("h");
        temp->child[ind]=NULL;
        free(temp->child[ind]);
        return;
    }
}

int main()
{
    load();
    while (1)
    {
        j=3;
        printf("\n\t\t====================Dictionary====================");
        printf("\n\n-->Enter your choice.\n\n1.View dictionary\n2.Add word\n3.Search meaning of a word\n4.Delete a word\n5.To view words lexicographically \n0.Exit\n");
        int n;
        char c[20];
        c[0]='\0';
        scanf("%d", &n);
        clrscr();
        clrscr();

        switch (n)
        {
        case 0:
            printf("Unloading dictionary........");
            unload(root);
            exit(0);
            break;
        case 1:
            printf("\n-->List of words in dictionary with thier meaning:\n");
            view();
            break;
        case 2:
            add();
            break;
        case 3:
            printf("-->Enter a word to get meaning:\n");
            printf("\n===========================\n\n");
            scanf("%s",c);
            search(c);
            printf("===========================\n");
            break;
        case 4:
            printf("-->Enter a word to delete :\n");
            printf("\n===========================\n\n");
            scanf("%s",c);
            delete(root,0,c);
            printf("\n===========================\n");
            break;
        case 5:
            printf("\n-->List of words in dictionary in lexicographic order:\n[");
            lexo(root,c);
            printf("]\n");
            break;
        default:
            printf("\nEnter a valid key\n");
        }
    }
    return 0;
}
