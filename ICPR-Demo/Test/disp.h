#ifndef DISP_H
#define DISP_H

void colorgraph(IplImage*,int,int);
int get_action_class(char *);
int recipe_chklst_init(IplImage *, IplImage *, char *, int *);
void recipe_checklist(IplImage *, IplImage *,IplImage  *, int *, int *, int);
void priorties_objects(int *);
void labelStatus(IplImage *,int *,int);
int classify_action(IplImage *,int *);

#endif