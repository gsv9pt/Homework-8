/*******************************************************
 * CS 2130 Homework 8
 * Fall 2025 - Simplified File System 
 *
 * Computing ID: gsv9pt
 * Sources: Cite your sources, including any GenAI
 *          usage.  (See syllabus for full details)
 *******************************************************/

//TODO: Remember to add include statement here. 

#include "filesystem.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

dir_node *create_dir(const char *name) {
if (name == NULL)          // if the value is empty a directory cant be made, this begins all functions 
return NULL;


dir_node *newdir = malloc(sizeof(dir_node));  // new directory struct
if (newdir == NULL)
return NULL;



newdir->name = strdup(name); // make copy of directory name
newdir->parent = NULL;   // initialize directory fields
newdir->next = NULL;
newdir->subdirs = NULL;
newdir->files = NULL;

return newdir;

}


dir_node *root_dir(void *node) {
if (node == NULL)
return NULL;


dir_node *cur;   // declares pointer for current directory
file_node *fp = (file_node *)node; // casts access to parent file
cur = ((file_node *)node)->parent; // gets parent of node

if (cur == NULL) // parent null then input is directory
    cur = cur->parent;

while (cur && cur->parent != NULL){  //follows parent until highest level
cur = cur ->parent;
}

return cur;  //returns pointer to the highest level directory
}


file_node *first_file(dir_node *dir) {
if (dir == NULL)
return NULL;


return dir->files; // return pointer to first file in directory
}



file_node *last_file(dir_node *dir) {
if (dir == NULL)
return NULL;


file_node *cur = dir->files;  // if no files return NULL
if (cur == NULL)
return NULL;


while (cur->next != NULL){  // goes through list until last
cur = cur->next;
}

return cur;  // cur will update to last file and return it
}



unsigned long dir_entries(dir_node *dir) {
if (dir == NULL)
return 0;

unsigned long c = 0;             // intializes counter and pointers that goes through files and subdirs
file_node *file = dir->files;
dir_node *subdir = dir->subdirs;


while (file != NULL){ // goes through files and increase counters until NULL is found
c++;
file = file->next;
}

while (subdir != NULL){ // same as file loop just with subdics
c++;
subdir = subdir->next;
}

return c; // return the counter
}


file_node *find_file(dir_node *dir, const char *name) {
if (dir == NULL || name == NULL)
return NULL;


file_node *cur = dir->files; // starts at beginning of files

while (cur != NULL) {                  // compares each file name to target name using strcmp command
     if (strcmp(cur->name, name) == 0){
     return cur; // return pointer to that node
     }
cur = cur->next;
}

return NULL; // return NULL if nothing found
}



dir_node *find_dir(dir_node *dir, const char *name) {
if (dir == NULL || name == NULL)
return NULL;


dir_node *cur= dir->subdirs;

while (cur != NULL){                    // same as file one but with subdirs
     if (strcmp(cur->name, name) == 0){
     return cur;
     }
     cur = cur->next;
}

return NULL;
}




file_node *remove_file(dir_node *dir, file_node *file) {
if (dir == NULL || file == NULL)
return NULL;


file_node *cur = dir->files;   // starts at head
file_node *pre = NULL;   // track node before cur

while (cur != NULL){  // goes through list
    if (cur == file){   // whne cur is file remove node
    file_node *nextn = cur->next;  // node that followed cur in list. Have to return
    
    if (pre == NULL){ // cur was first
    dir->files = nextn; // head becomes next node
    }
    else{
    pre->next = nextn;  //skip over cur
    }
free((void *)cur->name); // free mem
free(cur);

return nextn;// return node in removed one's position
}

pre = cur;
cur = cur->next;
}

return NULL;
}


dir_node *remove_dir(dir_node *dir, dir_node *subdir){
if (dir == NULL || subdir == NULL)
return NULL;


dir_node *cur = dir->subdirs;  // cur is subdic, pre keeps up of previous node
dir_node *pre = NULL;

while (cur){                  // go through subdirs, when cur equals subdir remove node otherwise continue
     if (cur == subdir){
     dir_node *nextn = cur->next;  //stores node after current
         
         if(pre == NULL)       // update dir->subdirs if pre NULL
         dir->subdirs = nextn;
         
         else
         pre->next = nextn; // skip over cur
         

     if (cur->subdirs){            // check if cur has subs then attach to end of parent sub list, update parent pointer
     dir_node *lsub = dir->subdirs;
         
         if (!lsub){
         dir->subdirs = cur->subdirs;
         }
         else{
              while (lsub->next){
              lsub = lsub->next;
              }
              lsub->next = cur->subdirs;
         }

         dir_node *temp = cur->subdirs;
         while (temp){
         temp->parent = dir;
         temp = temp->next;
         }
       }

if (cur->files){
file_node *lfile = dir->files;
         if (!lfile){
         dir->files = cur->files;
         }
         else{
              while (lfile->next){
              lfile = lfile->next;
              }
              lfile->next = cur->files;
         }

         file_node *temp2 = cur->files;
         while (temp2){
         temp2->parent = dir;
         temp2 = temp2->next;
         }
       }
free((void *)cur->name);   //free dir_node structure
free(cur);

return nextn;  // node that replaces cur
}

pre = cur;
cur = cur->next;
}

return NULL;
}








file_node *insert_file(dir_node *dir, const char *name, int size, const char *after) {
if (dir == NULL || name == NULL)
return NULL;

file_node *nf = malloc(sizeof(file_node));   // give new node, if malloc can't then return NULL
if (nf == NULL){
return NULL;
}

file_node *cur = dir->files; 

nf->name = strdup(name);  // initialize new file node
nf->size = size;
nf->parent = dir;
nf->next = NULL;

if (after == NULL){         // if after is NULL put at head 
    nf->next = dir->files;
    dir->files = nf;
    return nf;
}

while (cur){
      if (strcmp(cur->name, after) == 0){  // If file matches after then insert in following node
      nf->next = cur->next;
      cur->next = nf;
      return nf;
      }

cur = cur->next;

}

if (dir->files == NULL){   // directory is empty, make new file the first
dir->files = nf;
}
else{                       // go to final node and attach new file at the end
cur = dir->files;
     while(cur->next){
         cur = cur->next;
     }
     cur->next = nf;
}

return nf;  // new mode returned
}







dir_node *insert_dir(dir_node *dir, const char *name, const char *after) {
if (dir == NULL || name == NULL){
return NULL;
}

dir_node *nd = malloc(sizeof(dir_node)); // this has same concepts as file version 
if (nd == NULL){
return NULL;
}

dir_node *cur = dir->subdirs;

nd->name = strdup(name);  //initialize new directory
nd->parent = dir;
nd->next = NULL;
nd->subdirs = NULL;
nd->files = NULL;

if (after == NULL){
    nd->next = dir->subdirs;
    dir->subdirs = nd;
    return nd;
}

while (cur){
      if (strcmp(cur->name, after) == 0){
      nd->next = cur->next;
      cur->next = nd;
      return nd;
}

cur = cur->next;

}

if (dir->subdirs == NULL){
dir->subdirs = nd;
}
else{
cur = dir->subdirs;
     while(cur->next)
     cur = cur->next;
     cur->next = nd;
}

return nd;
}







// The following function has been provided for you
void directory_list(dir_node *root, int indent) {
  // Stop if the root is null (nowhere to go)
  if (root == NULL)
    return;

  // If indent is 0, we need to start at 4 for correct indentation
  if (!indent)
    indent = 4;
  
  // Print the current directory node
  printf("%*s %s (%ld entries)\n", indent, "-(D)", root->name, dir_entries(root));

  // Recursively print the subdirectories and their contents
  indent += 4;
  for (dir_node *i = root->subdirs; i != NULL; i = i->next) {
    directory_list(i, indent);
  }

  // Print out the files (no recursion needed)
  for (file_node *i = root->files; i != NULL; i = i->next) {
    printf("%*s %s (%d bytes)\n", indent, "-(F)", i->name, i->size);
  }
}

// Test your code in main
int main(int argc, const char *argv[]) {

  //TODO: test your code more than this
  dir_node *d = create_dir("Top");
 
  dir_node *fd = insert_dir(d, "First Dir", NULL);
  dir_node *sd = insert_dir(d, "Second Dir", "First Dir"); 

  insert_file(d, "First File", 42, NULL);
  insert_file(fd, "Second File", 67, NULL);
  insert_file(fd, "Third File", 21, "Second File");
  insert_file(sd, "Fourth File", 0, NULL);

  // print the directory listing starting at the root for sd
  directory_list(root_dir(sd), 0);

  return 0;
}



