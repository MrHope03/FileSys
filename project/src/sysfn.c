#include "../header/sysfn.h"
#include "../header/node.h"
#include "../header/stack.h"

extern Stack sys;

// System functions
void strip(char *str,char delim){
	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';	
}

void displayRoot(node_t *root){
	printf("%s [ ",root->name);
	node_t *p = root->next;
	while (p != NULL){
		if (p->dir){
			displayRoot(p);
		}else{
		printf("%s ",p->name);
		}
		p = p->adj;
	}
	printf("] ");
}

node_t *findDir(node_t *curr,char *fileName){
	node_t *p = curr->next;
	while (p != NULL){
		if (strcmp(p->name,fileName) == 0)
			return p;
		p = p->adj;
	}
	return NULL;
}

void listDir(node_t *root){
	printf("%s$ ",root->name);
	node_t *p = root->next;
	while (p != NULL){
		if (p->dir)
			printf("%s\\ ",p->name);
		else
			printf("%s ",p->name);
		p = p->adj;
	}
	printf("\n");
}

node_t *changeDir(node_t *curr,char *dir){
	node_t *child = findDir(curr,dir);
	if (child == NULL){
		printf("*ERROR - Couldn't locate %s\n",dir);
		return curr;
	}
	push(curr);
	if (!child->dir){
		printf("*ERROR - %s is not a dir\n",dir);
		return curr;
	}
	return child;
}

node_t *returnDir(node_t *curr){
	node_t *prev = pop(curr);
	if (prev == NULL){
		printf("*ERROR - No dir to move back\n");
		return curr;
	}
	return prev;	
}

void readFile(node_t *child){
	char fileName[25] = "../files/";
	strcat(fileName,child->name);
	FILE *fp = fopen(fileName,"r");
	if (fp == NULL)
		return;
	char c;
	c = fgetc(fp);
	while (!feof(fp)){
		printf("%c",c);
		c = fgetc(fp);
	}
	fclose(fp);
}

void writeFile(node_t *child){
	char fileName[25] = "../files/";
	strcat(fileName,child->name);
	FILE *fp = fopen(fileName,"a+");
	char c;
	while ((c = getchar()) != '\e'){
		fputc(c,fp);
	}
	c = getchar();
	fclose(fp);		
}

void deleteFile(node_t *child){
	if (remove(child->name))
		printf("File deleted\n");
	else
		printf("File Couldn't delete\n");
}

bool startRoot(node_t *root, FILE *file){
	node_t *temp = (node_t *) malloc(sizeof(node_t));
	node_t *p = root;
	while (fread(temp,sizeof(node_t),1,file)){
		if (p == root){	
			p->next = createNode(temp->name,temp->dir);	
			p = p->next;	
			if (p->dir){
				startRoot(p,file);
			}
		}
		else{
			p->adj = createNode(temp->name,temp->dir);
			p = p->adj;
			if (p->dir){
				startRoot(p,file);
			}
		}
		if (temp->adj == NULL)
			break;
	}
	if (fread == 0)
		return false;
	return true;
}

node_t *initialiseFileSystem(){
	FILE *file = fopen("../files/root.txt","r+");
	if (file == NULL){
		printf("RootSystem doesn't exist\n");
		printf("Created New ROOT\n");
		return createNode("root",true);
	}
	node_t *temp = (node_t *) malloc(sizeof(node_t));
	fread(temp,sizeof(node_t),1,file);
	node_t *root = temp;
	if (startRoot(root,file)){
		printf("Successfully Initialised\n");
		return root;
	}
	else{
		printf("Failed to Initiate\n");
		return NULL;
	}
}

bool closeRoot(node_t *root,FILE *file){
	fwrite(root, sizeof(node_t), 1, file);
	node_t *p = root->next;
	while (p != NULL){
		if (p->dir){
			closeRoot(p,file);
		}
		else{
			fwrite(p, sizeof(node_t), 1, file);
		}
		p = p->adj;
	}
	if (fwrite == 0) return false;
	return true;
}

void shutdownFileSystem(node_t *root){
	FILE *file = fopen("../files/root.txt","w+");
	if (closeRoot(root,file))
		printf("Shutdowned Properly\n");
	else printf("System Shutdown Error\n");
}

void pwd(node_t *curr){
	if (sys.top != NULL){
		node_t *temp = pop();
		pwd(temp);
		push(temp);
		printf("%s\\",curr->name);
	}
	else{
		printf("%s\\",curr->name);
	}

}

void prompt(node_t *curr){
	pwd(curr);
	printf("> ");
}
