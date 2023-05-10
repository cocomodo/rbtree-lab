#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nilNode=(node_t *)calloc(1, sizeof(node_t));
  // TODO: initialize struct if needed
  nilNode->color=RBTREE_BLACK;
  p->root=nilNode;
  p->nil=nilNode;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);

}


void left_rotation(rbtree *t,node_t *x){
  node_t *y=x->right;
  
  x->right=y->left;
  if(y->left!=t->nil)
    y->left->parent=x;
  
  y->parent=x->parent;
  if(x->parent==t->nil)
    t->root=y;
  else if(x==x->parent->left)
    x->parent->left=y;
  else 
    x->parent->right=y;

  y->left=x;
  x->parent=y;
}

void right_rotation(rbtree *t, node_t *x){
  node_t *y=x->left;

  x->left=y->right;
  if(y->right!=t->nil)
    y->right->parent=x;
  
  y->parent=x->parent;
  if(x->parent==t->nil)
    t->root=y;
  else if(x==x->parent->left)
    x->parent->left=y;
  else
    x->parent->right=y;

  y->right=x;
  x->parent=y;
}

/*
free_node 함수와 delete_rbtree 함수가 놓일 위치
*/

void rbtree_insert_fixup(rbtree *t,node_t *z){
  node_t *y;

  while(z->parent->color==RBTREE_RED){
    if(z->parent== z->parent->parent->left){
      y=z->parent->parent->right;
      if(y->color==RBTREE_RED){
        z->parent->color=RBTREE_BLACK;
        y->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        z=z->parent->parent;
      }
      else {//case 2: z의 삼촌 y가 black이고, z가 오른쪽 자식인 경우
        if(z==z->parent->right){
          z=z->parent;
          left_rotation(t,z);//z의 부모를 기준으로 왼쪽으로 회전
        }
        z->parent->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        right_rotation(t,z->parent->parent);//조부모를 기준으로 오른쪽으로 회전
      }
    }
    else{//case 4~6: z의 부모가 조부모의 오른쪽 자실이 경우
      y=z->parent->parent->left;
      //case 4
      if(y->color==RBTREE_RED){
        z->parent->color=RBTREE_BLACK;
        y->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        z=z->parent->parent;
      }
      //case 5
      else {
        if(z==z->parent->left){
          z=z->parent;
          right_rotation(t,z);
        }
        //case 6
        z->parent->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        left_rotation(t,z->parent->parent);
      }
    }
  }
  t->root->color=RBTREE_BLACK;
}

// rbtree_insert_fixup 함수를 위에 작성. 그 함수에 필요한 left_rotation
//right_rotation 함수를 그 위에 미리 선언. 
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y=t->nil;
  node_t *x=t->root;
  node_t *z=(node_t *)calloc(1, sizeof(node_t));

  z->key=key;

  while(x!=t->nil){
    y=x;
    if(z->key<x->key)
      x=x->left;
    else
      x=x->right;
  }

  z->parent=y;
  
  if(y==t->nil)
    t->root=z;
  else if(z->key < y->key)
    y->left=z;
  else 
    y->right=z;

  z->left=t->nil;
  z->right=t->nil;
  z->color=RBTREE_RED;

  rbtree_insert_fixup(t,z);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur=t->root;
  
  while(cur!=t->nil){
    if(cur->key==key){
      return cur;//key를 찾으면 현재 노트 cur을 반환
    }
    if(key<cur->key)
      cur=cur->left;
    else
      cur=cur->right;
  }
  return NULL;//key를 못찾으면 NULL 반환
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if(t->root==t->nil){
    return NULL;
  }
  node_t *cur=t->root;

  while(cur->left!=t->nil){
    cur=cur->left;
  }
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if(t->root==t->nil){
    return NULL;
  }

  node_t *cur=t->root;
  
  while(cur->right!=t->nil){
    cur=cur->right;
  }
  return cur;
}


void rbtree_delete_fixup(rbtree *t,node_t *x){
  while(x!=t->root && x->color==RBTREE_BLACK){
    //case 1~4: left case
    if(x==x->parent->left){
      node_t *w=x->parent->right;

      //case 1 : x의 형제 w가 red인 경우
      if(w->color==RBTREE_RED){
        w->color=RBTREE_BLACK;
        x->parent->color=RBTREE_RED;
        left_rotation(t,x->parent);
        w=x->parent->right;
      }
      //case 2 : x의 형제 w는 black이고 w의 두 자식이 모두 black인 경우
      if(w->left->color==RBTREE_BLACK && w->right->color==RBTREE_BLACK){
        w->color=RBTREE_RED;
        x=x->parent;
      }

      //case 3: x의 형제 w는 black, w의 왼쪽 자식은 red, w의 오른쪽 자식은 black인 경우
      else{
        if(w->right->color==RBTREE_BLACK){
          w->left->color=RBTREE_BLACK;
          w->color=RBTREE_RED;
          right_rotation(t,w);
          w=x->parent->right;
        }

        //case 4 : x의 형제 w는 black이고 w의 오른쪽 자식은 red인 경우
        w->color=x->parent->color;
        x->parent->color=RBTREE_BLACK;
        w->right->color=RBTREE_BLACK;
        left_rotation(t,x->parent);
        x=t->root;
      }
    }
    //case 5 ~ 8: right case
    else{
      node_t *w=x->parent->left;

      //case 5 : x의 형제 w가 red인 경우
      if(w->color==RBTREE_RED){
        w->color=RBTREE_BLACK;
        x->parent->color=RBTREE_RED;
        right_rotation(t,x->parent);
        w=x->parent->left;
      }

      //case 6 : x의 형제 w는 black이고 w의 두 자식이 모두 black인 경우
      if(w->right->color==RBTREE_BLACK && w->left->color==RBTREE_BLACK){
        w->color=RBTREE_RED;
        x=x->parent;
      }
      
      //case 7 : x의 형제 w는 black, w의 왼쪽 자식은 red, w의 오른쪽 자식은 black인 경우
      else{
        if(w->left->color==RBTREE_BLACK){
          w->right->color=RBTREE_BLACK;
          w->color=RBTREE_RED;
          left_rotation(t,w);
          w=x->parent->left;
        }

        //case 8 : x의 형제 w는 black이고 w의 오른쪽 자식은 red인 경우
        w->color=x->parent->color;
        x->parent->color=RBTREE_BLACK;
        w->left->color=RBTREE_BLACK;
        right_rotation(t,x->parent);
        x=t->root;
      }
    }
  }

  x->color=RBTREE_BLACK;
}

node_t *find_successor(rbtree *t,node_t *cur){
  while(cur->left!=t->nil){
    cur=cur->left;
  }
  return cur;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent==t->nil)//u의 부모가 nil. 즉 u가 루트노드라면
    t->root=v;         //v를 트리의 루트노드로 삼는다. 
  else if(u==u->parent->left) //u가 부모의 왼쪽 자식일 경우
    u->parent->left=v;        //v를 왼쪽 자식으로 이식(u를 대체)
  else                        //오른쪽 자식일 경우
    u->parent->right=v;       //v를 오른쪽 자식으로 이식

  v->parent=u->parent;
}


//tree_transplant, *node_nill, *find_successor, *erase_fixup이 있어야함
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y;
  node_t *x;
  color_t y_original_color;

  //삭제하려는 노드 p의 정보를 우선 y에 다 저장해둔다
  //(y가 p를 기준으로 시작하지만 중간에 바뀔수 있음)
  y=p;
  y_original_color=y->color;
  
  //노드 p에게 유효한 값을 가진 자식이 하나 있는데 
  //그 자식이 오른쪽에 있는 경우(왼쪽 자식은 nill)
  if(p->left==t->nil){
    x=p->right;
    rbtree_transplant(t,p,p->right);//p의 왼쪽 자식을 p의 위치에 이식하면서 p는 제거됨
  }
  //유효한 값을 가진 자식이 왼쪽에만 하나 있는 경우
  else if(p->right==t->nil){
    x=p->left;
    rbtree_transplant(t,p,p->left);//p의 왼쪽 자식을 p의 위치에 이삭하면서 p는 제거됨
  }
  //유효한 자식이 둘인 경우(왼쪽 자식도 nil 아니고 오른쪽 자식도 nil이 아님)
  else{
    y=find_successor(t,p->right);
    y_original_color=y->color;
    x=y->right;
    
    if(y->parent==p){
      x->parent=y;
    }
    else{
      rbtree_transplant(t,y,y->right);
      y->right=p->right;
      y->right->parent=y;
    }
    
    rbtree_transplant(t,p,y);
    y->left=p->left;
    y->left->parent=y;
    y->color=p->color;
  }
  //삭제되는 색이 black인 경우에만 속성 위반 여부 확인
  if(y_original_color==RBTREE_BLACK){
    rbtree_delete_fixup(t,x);
  }
  free(p);
  return 0;
}

void subtree_to_array(const rbtree *t, node_t *cur, key_t *arr, size_t n, size_t *cnt){
  if(cur==t->nil){
    return;
  }

  subtree_to_array(t,cur->left,arr,n,cnt);
  if(*cnt<n){
    arr[(*cnt)++]=cur->key; // [*cnt++] 으로 작성하지 않게 주의. [(*cnt)++]여야 한다. 
  }
  else return;
  subtree_to_array(t,cur->right,arr,n,cnt);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array

  if(t->root==t->nil){
    return 0;
  }

  size_t cnt=0;
  subtree_to_array(t,t->root,arr,n, &cnt);
  return 0;


}