#include "../header/sysfn.h"

extern Stack sys;

int main(){
	node_t *root = initialiseFileSystem();
	node_t *curr = root;

	size_t size = 25;
	char *cmdLine = (char *) malloc(size);

	prompt(curr);
	while (getline(&cmdLine,&size,stdin)){
		char *cmd = strtok(cmdLine," ");
		strip(cmd,'\n');
		if (!strcmp(cmd,"touch")){
			cmd = strtok(NULL," ");
			strip(cmd,'\n');
			node_t *file = createNode(cmd,false);
			addNode(curr,file);
			readFile(file);
			writeFile(file);
		}
		else if (!strcmp(cmd,"mkdir")){
			cmd = strtok(NULL," ");
			strip(cmd,'\n');
			node_t *file = createNode(cmd,true);
			addNode(curr,file);
		}
		else if (!strcmp(cmd,"open")){
			cmd = strtok(NULL," ");
			strip(cmd,'\n');
			node_t *file = findDir(curr,cmd);
			if (file != NULL && file->dir == false){
				readFile(file);
				writeFile(file);
			}
			else{
				printf("Couldn't locate %s\n",cmd);
			}
		}
		else if (!strcmp(cmd,"cd")){
			cmd = strtok(NULL," ");
			strip(cmd,'\n');
			if (!strcmp(cmd,"..")){
				curr = returnDir(curr);
			}
			else{
				curr = changeDir(curr,cmd);
			}
		}
		else if(!strcmp(cmd,"rm")){
			cmd = strtok(NULL," ");
			strip(cmd,'\n');
			node_t *Node = findDir(curr,cmd);
			if (Node != NULL){
				if (!Node->dir){
					deleteFile(Node);
				}
				removeNode(curr,Node);
			}
			else printf("Error - Couldn't locate %s\n",cmd);
		}
		else if (!strcmp(cmd,"ls")){
			listDir(curr);
		}
		else if (!strcmp(cmd,"ls-l")){
			displayRoot(curr);
			printf("\n");
		}
		else if (!strcmp(cmd,":q")){
			break;
		}
		else{
			printf("*Error - Unknown command\n");
		}
		prompt(curr);
		cmd = NULL;
	}
	shutdownFileSystem(root);
	return 0;
}
