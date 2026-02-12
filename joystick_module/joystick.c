#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>

void init_module(){
  printf("Module loaded\n");
}

void exit_module(){
  printf("Module exited\n");
}
