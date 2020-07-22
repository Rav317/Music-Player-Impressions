#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>

typedef struct snode{        // Trie node for storing songs
    char sname[100];
    struct snode* next;
    struct snode* prev;
}snode;

typedef struct pl          // Playist data stucture
{
    char plname[100];
    struct snode* root;
}pl;

pl* all_pl[10];  
int np = 0;

pl* insert_song_in_playlist(pl* pl1, char*song_name)  //adding songs to a playlist
{
    snode* new = malloc(sizeof(snode));
    strcpy(new->sname,song_name);
    new->next = new->prev = NULL;

    if(pl1->root == NULL)
    {
        pl1->root = new;    
        new->prev = new->next = pl1->root;
    }
    else
    {
        snode* temp = pl1->root;
        while(temp->next != pl1->root)
            temp = temp->next;
        temp->next = new;
        new->prev = temp;
        new->next = pl1->root;
        pl1->root->prev = new;
    }
    return pl1;
}


void load_playlists()       // using the saved playlists
{
    FILE* pl_file;
    pl_file = fopen("playlists.txt", "r");
    if(pl_file == NULL)
    {
        printf("The file could not be opened");
        exit(1);
    }
    int i = 0;
    char x[100];
    char* token;
    while(fgets(x,100,pl_file))
    {
        if(!strcmp(x, "\n"))
            continue;
        token = strtok(x," ");
        all_pl[i] = malloc(sizeof(pl));
        all_pl[i]->root = NULL;
        strcpy(all_pl[i]->plname, token);
        while(token != NULL)
        {
            token = strtok(NULL, " ");
            if(token != NULL)
                insert_song_in_playlist(all_pl[i], token);
        }
        i++;
        np++;
    }
    fclose(pl_file);
}

void create_playlist()       // creating a custom playlist
{   
    int n = np;
    char nplname[100];
    printf("Enter the name of the playlist:");
    scanf("%s", nplname);
    all_pl[n] = malloc(sizeof(pl));
    all_pl[n]->root = NULL;
    strcpy(all_pl[n]->plname, nplname);

    char new_song[100];
    printf("Enter the song name(-1 to exit): ");
    if(!strcmp(new_song, "-1"))
		exit(1);
	scanf("%s", new_song);
    while(strcmp(new_song, "-1"))
    {
        insert_song_in_playlist(all_pl[n], new_song);
        fflush(stdin);
        printf("\nEnter the song name(-1 to exit):");
        scanf("%s", new_song);
    } 

    np++;
}

void save_playlists()         // saving the playist as text
{
    FILE* pl_file;
    pl_file = fopen("playlists.txt", "w");
    if(pl_file == NULL)
    {
        printf("The file could not be opened");
        exit(1);
    }
    for(int i = 0; i<np; i++)
    {
        fprintf(pl_file, "%s ", all_pl[i]->plname);
        snode* temp = all_pl[i]->root;
        while(temp->next != all_pl[i]->root)
        {
            fprintf(pl_file, "%s ", temp->sname);
            temp = temp->next;
        }
        fprintf(pl_file, "%s\n", temp->sname);
    }
    fclose(pl_file);
}

void view_playlists()
{
    printf("PLAYLISTS\n");
    for(int i = 0; i<np; i++)
    {
        if(all_pl[i] == NULL)
            continue;
        printf("%s : ", all_pl[i]->plname);
        snode* temp = all_pl[i]->root;
        while(temp->next != all_pl[i]->root)
        {
            printf("%s ,", temp->sname);
            temp = temp->next;
        }
        printf("%s\n", temp->sname);
    }
}

void play_playlist()    // impersonating playing of a playlist
{
    char playlist[100];
    printf("Enter the playlist to play: ");
    scanf("%s", playlist);
    printf("PLAYING the playlist %s\n\n",playlist);
    char choose[1] = "F";
    for(int i = 0; i<10; i++)
    {
        if(all_pl[i] != NULL && strcmp(all_pl[i]->plname, playlist) == 0)
        {
            snode* temp = all_pl[i]->root;
            while(1)
            {
                printf("Playing the song: %s\n", temp->sname);
                Sleep(2000);
                printf("F: Next Song, B: Previous Song, S:Stop ?\n");
                scanf("%s", choose);
                if(!strcmp(choose, "F"))
                	temp = temp->next;
                else if(!strcmp(choose, "B"))
                	temp = temp->prev;
                else
                	break;
            }
            if(!strcmp(choose,"B"))
            	printf("%s\n",temp->sname);
            else
            	break;
        }
    }
}


struct trie_node{

struct trie_node*child[255];
int eow;
};


struct node{

char *data;
struct trie_node*next;
};

struct node*adj[4];

struct trie_node* get_node(){     // tries implementation to retrieve a node

struct trie_node*temp;
temp = malloc(sizeof(struct trie_node));
int i=0;
for(i=0;i<255;i++){
temp ->child[i] = NULL;
}

temp ->eow =0;
return temp;
}

struct trie_node* insert(struct trie_node*root,char *key){   // tries implementation to insert a node
int index;
struct trie_node*curr = root;

for(int i=0;key[i]!='\0';i++){
index = key[i];
if(curr -> child[index] == NULL){
curr -> child[index] = get_node();
}
curr = curr -> child[index];

}
curr -> eow = 1;
return root;
}


void display(struct trie_node*root,char word[],int level){   // displaying a trie

int i;

if(root == NULL){
return;
}

if(root ->eow){
word[level] =0;
printf("\n");
for(i=0;i<level;i++){
printf("%c",word[i]);}
printf("\n");

}
for(i=0;i<255;i++){
if(root -> child[i]!=NULL){
word[level] = i;
display(root ->child[i],word,level+1);
}

}
}

int search(struct trie_node*root,char *key){     // searching the songs

struct trie_node*curr = root;
int i,index;

for(i=0;key[i]!='\0';i++){
index= key[i];
if(curr -> child[index] == NULL){
return 0;
}
curr = curr -> child[index];
}
if(curr -> eow==1){
return 1;
}
}

void load_songs()      // loading all the saved songs from the file
{
    FILE* f = fopen("songs.txt", "r");
    char y[100];
    for(int i = 0; i<4; i++)
    {
        fgets(y,100,f);
        y[strlen(y)-1] = 0;
        char* token2;
        token2 = strtok(y, " ");
        token2 = strtok(NULL, " ");
        while(token2 != NULL)
        {
            adj[i]->next = insert(adj[i]->next, token2);
            token2 = strtok(NULL, " ");
        }
    }
}


void word_save(struct trie_node*root,char word[],int level,FILE*f){   // saving data on files

int i;
if(root == NULL){
return;
}
if(root ->eow){
word[level] =0;
    fprintf(f, "%s ", word);   
}
for(i=0;i<255;i++){
if(root -> child[i]!=NULL){
word[level] = i;
word_save(root ->child[i],word,level+1,f);
}

}
}

void save_songs()     // saving the songs in txt file
{   
    FILE* f = fopen("songs.txt", "w");
    char song[100];
    for(int i = 0; i<4; i++)
    {
        fprintf(f, "%s ", adj[i]->data);
        word_save(adj[i]->next, song, 0,f);
        fprintf(f, "%s\n", "");
    }
    fclose(f);

}

int main(){

for(int i = 0; i<10; i++)
        all_pl[i] = NULL;

    load_playlists();

    printf("MUSIC PLAYER\n");



char *arr[100] = {"Pop","English","Hindi","Rock"};
int option;
char song[100];
int i;
for(i=0;i<4;i++){
adj[i] = malloc(sizeof(struct node*));
adj[i] -> data = arr[i];
adj[i] -> next = NULL;
}

struct trie_node*pop = NULL;
pop = get_node();
struct trie_node*english = NULL;
english = get_node();
struct trie_node*hindi = NULL;
hindi = get_node();
struct trie_node*rock = NULL;
rock= get_node();

adj[0] -> next = pop;
adj[1] -> next = english;
adj[2] -> next = hindi;
adj[3] -> next = rock;

load_songs();

printf("\nCHOOSE A CATEGORY IN WHICH YOU WANNA INSERT THE SONG\n");
char word[100];
char word1[100];
char word2[100];
char word3[100];

do{
int flag=0;
char sng[100];
printf("\n");
for(i=0;i<4;i++){
printf("%d.>%s\t",i+1,adj[i]->data);
}
printf("\n5.>Search a song...");
printf("\n6.>View all songs list...");
printf("\n7.>View playlists...");
printf("\n8.>Create playlists....");
printf("\n9.>Play playlists...");
printf("\n10.> Exit...\n");
scanf("%d",&option);

switch(option){

case 1:
    printf("\nEnter the name of the pop song---");
    scanf("%s",song);
    pop= insert(pop,song);
    break;

case 2:
    printf("\nEnter the name of the english song---");
    scanf("%s",song);
    english = insert(english,song);
    break;

case 3:
    printf("\nEnter the name of the hindi song---");
    scanf("%s",song);
    hindi= insert(hindi,song);
    break;

case 4:
    printf("\nEnter the name of the rock song---");
    scanf("%s",song);
    rock = insert(rock,song);
    break;

case 5:
    printf("\nENTER THE NAME OF THE SONG---");
    scanf("%s",sng);
    for(i=0;i<4;i++){
    flag = search(adj[i] -> next,sng);
    if(flag ==1){
    printf("\nSong found in category ---%s",adj[i]->data);
    break;

    }
    }

    if(flag ==0){
    printf("\nSong not found...");
    }
    break;


case 6:
    printf("\nPop");
    display(pop,word,0);
    printf("\nEnglish");
    display(english,word1,0);
    printf("\nHindi");
    display(hindi,word2,0);
    printf("\nRock");
    display(rock,word3,0);
    break;
case 7: view_playlists(); //done
                    break;
case 8: create_playlist();  //done
        break;
case 9: play_playlist(); //done
        break;

}}while(option!=10);
save_playlists();
save_songs();
printf("\n%s\n", "EXITED");
return 0;
}





