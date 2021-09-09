#ifndef AVL_H
#define AVL_H

#include <assert.h>

#include <algorithm>

struct AvlNode {
  AvlNode *left = nullptr;
  AvlNode *right = nullptr;
  AvlNode *parent = nullptr;
  int height = 0;
  int value = 0;

  AvlNode() = default;
  AvlNode(int value): value(value) {}
};

class Avl {
 public:
  using NodeVisitor = void (*)(const AvlNode *);
 public:
  Avl() = default;
  ~Avl() {
    CleanUp();
  }

  void Insert(int value) {
    root_ = Insert(root_, value);
    SetH(root_, std::max(H(root_->left), H(root_->right)) + 1);
  }
  void Delete(int value) {
    root_ = Delete(root_, value);
    if (root_) SetH(root_, std::max(H(root_->left), H(root_->right)) + 1);
  }
  int Search(int value) {
    AvlNode *p = Search(root_, value);
    if (!p) return -1;
    return p->value;
  }
  int Max() {
    AvlNode *p = Max(root_);
    if (!p) return -1;
    return p->value;
  }
  int Min() {
    AvlNode *p = Min(root_);
    if (!p) return -1;
    return p->value;
  }
  int Precessor(int value) {
    AvlNode *p = Search(root_, value);
    if (!p) return -1;
    AvlNode *t = Precessor(p);
    if (!t) return -1;
    return t->value;
  }
  int Successor(int value) {
    AvlNode *p = Search(root_, value);
    if (!p) return -1;
    AvlNode *t = Successor(p);
    if (!t) return -1;
    return t->value;
  }
  void Preorder(NodeVisitor visitor) {
    Preorder(root_, visitor);
  }
  void Inorder(NodeVisitor visitor) {
    Inorder(root_, visitor);
  }
  void Postorder(NodeVisitor visitor) {
    Postorder(root_, visitor);
  }
 private:
  AvlNode *Insert(AvlNode *r, int value) {
    if (!r) return new AvlNode(value);
    if (r->value < value) {
      r->right = Insert(r->right, value);
      r->right->parent = r;
    } else {
      r->left = Insert(r->left, value);
      r->left->parent = r;
    }
    return Balance(r);
  }
  AvlNode *Delete(AvlNode *r, int value) {
    if (!r) return nullptr;
    if (r->value == value) {
      AvlNode *to_remove = nullptr;
      if (!r->left && !r->right) {
        to_remove = r;
        r = nullptr;
      } else if (r->left && !r->right) {
        to_remove = r;
        r = r->left;
      } else if (!r->left && r->right) {
        to_remove = r;
        r = r->right;
      } else {
        AvlNode *t = Successor(r);
        r->value = t->value;
        r->right = Delete(r->right, t->value);
      }
      delete to_remove;
    } else if (r->value < value) {
      r->right = Delete(r->right, value);
      if (r->right) r->right->parent = r;
    } else {
      r->left = Delete(r->left, value);
      if (r->left) r->left->parent = r;
    }
    return Balance(r);
  }
  AvlNode *Search(AvlNode *r, int value) {
    if (!r) return nullptr;
    if (r->value == value) {
      return r;
    } else if (r->value < value) {
      return Search(r->right, value);
    } else {
      return Search(r->left, value);
    }
  }
  AvlNode *Max(AvlNode *r) {
    AvlNode *p = r;
    while (p->right) p = p->right;
    return p;
  }
  AvlNode *Min(AvlNode *r) {
    AvlNode *p = r;
    while (p->left) p = p->left;
    return p;
  }
  AvlNode *Successor(AvlNode *node) {
    if (node->right) return Min(node->right);
    AvlNode *u = node->parent;
    AvlNode *v = node;
    while (u && u->right == v) {
      v = u;
      u = u->parent;
    }
    return u;
  }
  AvlNode *Precessor(AvlNode *node) {
    if (node->left) return Max(node->left);
    AvlNode *u = node->parent;
    AvlNode *v = node;
    while (u && u->left == v) {
      v = u;
      u = u->parent;
    }
    return u;
  }
  void Preorder(AvlNode *r, NodeVisitor visitor) {
    if (r) {
      visitor(r);
      Preorder(r->left, visitor);
      Preorder(r->right, visitor);
    }
  }
  void Inorder(AvlNode *r, NodeVisitor visitor) {
    if (r) {
      Inorder(r->left, visitor);
      visitor(r);
      Inorder(r->right, visitor);
    }
  }
  void Postorder(AvlNode *r, NodeVisitor visitor) {
    if (r) {
      Postorder(r->left, visitor);
      Postorder(r->right, visitor);
      visitor(r);
    }
  }
 private:
  void SetH(AvlNode *r, int h) {
    if (!r) return;
    r->height = h;
  }
  int H(AvlNode *r) {
    if (!r) return -1;
    return r->height;
  }

  AvlNode *Balance(AvlNode *r) {
    if (!r) return nullptr;
    if (H(r->left) - H(r->right) == 2) {
      if (H(r->left->left) - H(r->left->right) == 1) {
        r = RightRotate(r);
      } else {
        r->left = LeftRotate(r->left);
        r = RightRotate(r);
      }
    } else if (H(r->left) - H(r->right) == -2) {
      if (H(r->right->left) - H(r->right->right) == -1) {
        r = LeftRotate(r);
      } else {
        r->right = RightRotate(r->right);
        r = LeftRotate(r);
      }
    }
    SetH(r, std::max(H(r->left), H(r->right)) + 1);
    return r;
  }
  //   P            Q
  // A  Q    =>   P  C
  //   B C       A B
  AvlNode *LeftRotate(AvlNode *P) {
    AvlNode *Q = P->right;
    assert(Q);
    P->right = Q->left;
    if (Q->left) Q->left->parent = P;
    Q->left = P;
    P->parent = Q;
    SetH(P, std::max(H(P->left), H(P->right)) + 1);
    SetH(Q, std::max(H(Q->left), H(Q->right)) + 1);
    return Q;
  }
  //    P           Q
  //  Q  C   =>   A   P
  // A B             B C
  AvlNode *RightRotate(AvlNode *P) {
    AvlNode *Q = P->left;
    assert(Q);
    P->left = Q->right;
    if (Q->right) Q->right->parent = P;
    Q->right = P;
    P->parent = Q;
    SetH(P, std::max(H(P->left), H(P->right)) + 1);
    SetH(Q, std::max(H(Q->left), H(Q->right)) + 1);
    return Q;
  }
 private:
  void CleanUp() {
    Free(root_);
    root_ = nullptr;
  }
  void Free(AvlNode *r) {
    if (!r) return;
    if (r->left) Free(r->left);
    r->left = nullptr;
    if (r->right) Free(r->right);
    r->right = nullptr;
    delete r;
  }
 private:
  AvlNode *root_ = nullptr;
};


#endif