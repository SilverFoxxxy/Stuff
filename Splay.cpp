//
//  main.cgranny
//  cgrannyHomework
//
//  Created by Belkovanya on 24/05/2020.
//  Copyright © 2020 ИВАН. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <set>
#include <cmath>

typedef long long ll;

class Node{
    
private:
    Node* LocalFind(int i) {
        Upd();
        int ind = i;
        if (left_) {
            int sz = left_->size_;
            if (ind < sz) {
                return left_->LocalFind(ind);
            }
            ind -= sz;
        }
        if (ind == 0) {
            Splay();
            return this;
        }
        if (right_) {
            return right_->LocalFind(ind - 1);
        }
        Splay();
        return nullptr;
    }
    
    Node* LocalUpperBound(ll u, bool incr) {
        Upd();
        if (key_ > u) {
            bool flag = true;
            if (incr) {
                if (left_ && left_->max_ > u) {
                    flag = false;
                }
            } else {
                if (right_ && right_->max_ > u) {
                    flag = false;
                }
            }
            if (flag) {
                Splay();
                return this;
            }
        }
        if (min_ == max_) {
            if (incr) {
                if (left_) return left_->LocalUpperBound(u, incr);
                Splay();
                return this;
            } else {
                if (right_) return right_->LocalUpperBound(u, incr);
                Splay();
                return this;
            }
        }
        if (left_ && right_ &&
            left_->max_ > u && right_->max_ > u) {
            if (left_->min_ < right_->min_) {
                return left_->LocalUpperBound(u, incr);
            } else {
                return right_->LocalUpperBound(u, incr);
            }
        }
        
        if (left_) {
            if (left_->max_ > u) {
                return left_->LocalUpperBound(u, incr);
            }
        }
        if (right_) {
            if (right_->max_ > u) {
                return right_->LocalUpperBound(u, incr);
            }
        }
        Splay();
        if (key_ > u) {
            return this;
        } else {
            return nullptr;
        }
    }
    
    Node* LocalLowerBound(ll u, bool incr) {
        Upd();
        if (key_ < u) {
            bool flag = true;
            if (incr) {
                if (right_ && right_->min_ < u) {
                    flag = false;
                }
            } else {
                if (left_ && left_->min_ < u) {
                    flag = false;
                }
            }
            if (flag) {
                Splay();
                return this;
            }
        }
        if (min_ == max_) {
            if (!incr) {
                if (left_) return left_->LocalLowerBound(u, incr);
                Splay();
                return this;
            } else {
                if (right_) return right_->LocalLowerBound(u, incr);
                Splay();
                return this;
            }
        }
        if (left_ && right_ &&
            left_->min_ < u && right_->min_ < u) {
            if (left_->max_ > right_->max_) {
                return left_->LocalLowerBound(u, incr);
            } else {
                return right_->LocalLowerBound(u, incr);
            }
        }
        if (left_) {
            if (left_->min_ < u) {
                return left_->LocalLowerBound(u, incr);
            }
        }
        if (right_) {
            if (right_->min_ < u) {
                return right_->LocalLowerBound(u, incr);
            }
        }
        Splay();
        if (key_ < u) {
            return this;
        } else {
            return nullptr;
        }
    }
    
    void Zig() {
        Upd();
        if (!parent_) {
            return;
        }
        Node* granny = parent_->parent_;
        if (granny) {
            if (granny->left_ == parent_) {
                granny->left_ = this;
            } else {
                granny->right_ = this;
            }
        }
        if (parent_->left_ == this) {
            Node* b = right_, *q = parent_;
            
            right_ = q;
            q->parent_ = this;
            
            q->left_ = b;
            if (b) b->parent_ = q;
            
            parent_ = granny;
            
            q->Upd();
        } else {
            Node* q = parent_, *b = left_;
            
            left_ = q;
            q->parent_ = this;
            
            q->right_ = b;
            if (b) b->parent_ = q;
            
            parent_ = granny;
            
            q->Upd();
        }
        if (granny) granny->Upd();
        Upd();
    }
    
    void ZigZig() {
        parent_->Zig();
        Zig();
    }
    
    void ZigZag() {
        Zig();
        Zig();
    }
    
    void LocalMakeEqualTo(ll u) {
        add_ = 0;
        make_equal_to_ = true;
        equal_to_ = u;
        key_ = equal_to_;
        min_ = u;
        max_ = u;
        decreasing_ = true;
        increasing_ = true;
        sum_ = size_ * equal_to_;
    }
    
    void LocalAdd(ll add) {
        if (make_equal_to_) {
            LocalMakeEqualTo(equal_to_ + add);
            return;
        }
        key_ += add;
        add_ += add;
        min_ += add;
        max_ += add;
        sum_ += add * size_;
    }
    
public:
    ll key_ = -1;
    
    ll add_ = 0;
    bool make_equal_to_ = false;
    ll equal_to_ = 0;
    
    ll sum_ = 0;
    ll min_;
    ll max_;
    
    bool rotate_ = false;
    int size_ = 1;
    
    bool increasing_ = true;
    bool decreasing_ = true;
    
    Node* left_ = nullptr, *right_ = nullptr, *parent_ = nullptr;
    
    Node(){}
    
    Node(ll x, Node* par){
        key_ = x; parent_ = par; sum_ = x;
        min_ = x; max_ = x;
    }
    
    void Splay() {
        Upd();
        while (parent_ != nullptr) {
            Node* granny = parent_->parent_;
            Node* par = parent_;
            if (granny) {
                //if (granny->parent_) granny->parent_->Upd();
                granny->Upd();
            }
            parent_->Upd();
            Upd();
            if (!granny) {
                Zig();
                continue;
            }
            if ((granny->left_ == parent_ && parent_->left_ == this) ||
                (granny->right_ == parent_ && parent_->right_ == this)) {
                ZigZig();
            } else {
                ZigZag();
            }
            if (granny) {
                granny->Upd();
            }
            par->Upd();
            Upd();
        }
    }
    
    Node* Find(int i) {
        Splay();
        return LocalFind(i);
    }
    
    // first greater
    /// tree must be decreasing or increasing
    Node* UpperBound(ll u) {
        Splay();
        return LocalUpperBound(u, increasing_);
    }
    
    // first less
    /// tree must be decreasing or increasing
    Node* LowerBound(ll u) {
        Splay();
        return LocalLowerBound(u, increasing_);
    }
    
    int GetSize() {
        Splay();
        return size_;
    }
    
    int GetIndex() {
        Splay();
        if (left_ == nullptr) return 0;
        return left_->size_;
    }
    
    ll GetSum() {
        Upd();
        return sum_;
    }
    
    void LocalUpd() {
        if (rotate_) {
            rotate_ = false;
            std::swap(left_, right_);
            if (left_) {
                left_->LocalRotate();
                //left_->rotate_ = !left_->rotate_;
                //left_->parent_ = this;
            }
            if (right_) {
                right_->LocalRotate();
                //right_->rotate_ = !right_->rotate_;
                //right_->parent_ = this;
            }
        }
        
        if (make_equal_to_) {
            if (left_) {
                left_->LocalMakeEqualTo(equal_to_);
            }
            if (right_) {
                right_->LocalMakeEqualTo(equal_to_);
            }
            //key_ = equal_to_;
            make_equal_to_ = false;
        }
        
        if (add_) {
            if (left_) {
                left_->LocalAdd(add_);
            }
            if (right_) {
                right_->LocalAdd(add_);
            }
            //key_ += add_;
            add_ = 0;
        }
        
        sum_ = key_;
        size_ = 1;
        min_ = key_;
        max_ = key_;
        
        if (left_) {
            sum_ += left_->sum_;
            size_ += left_->size_;
            min_ = std::min(min_, left_->min_);
            max_ = std::max(max_, left_->max_);
        }
        
        if (right_) {
            sum_ += right_->sum_;
            size_ += right_->size_;
            min_ = std::min(min_, right_->min_);
            max_ = std::max(max_, right_->max_);
        }
        
        
        if ((!left_ || (left_->increasing_ && left_->max_ <= key_)) &&
             (!right_ || (right_->increasing_ && key_ <= right_->min_))) {
            increasing_ = true;
        } else {
            increasing_ = false;
        }
        
        if ((!left_ || (left_->decreasing_ && left_->min_ >= key_)) &&
             (!right_ || (right_->decreasing_ && key_ >= right_->max_))) {
            decreasing_ = true;
        } else {
            decreasing_ = false;
        }
    }
    
    void Add(ll add) {
        Splay();
        LocalAdd(add);
    }
    
    void MakeEqualTo(ll u) {
        Splay();
        LocalMakeEqualTo(u);
    }
    
    void Upd() {
        LocalUpd();
    }
    
    void LocalRotate() {
        rotate_ = !rotate_;
        std::swap(increasing_, decreasing_);
    }
    
    void Rotate() {
        Splay();
        LocalRotate();
    }
    
    void Print() {
        Upd();
        if (left_) {
            left_->Print();
        }
        std::cout << key_ << ' ';
        if (right_) right_->Print();
    }
    
};

class Tree {
    
public:
    Node* root_ = nullptr;
    
    Tree(){}
    
    int GetSize() {
        if (!root_) {
            return 0;
        }
        Splay(root_);
        return root_->GetSize();
    }
    
    Node* Merge(Node* a, Node* b) {
        if (a == nullptr) {
            return b;
        }
        if (b == nullptr) {
            return a;
        }
        a->Splay();
        b->Splay();
        Node* a_r = a;
        a_r->Upd();
        while (a_r->right_) {
            a_r = a_r->right_;
            a_r->Upd();
        }
        a_r->Splay();
        a_r->right_ = b;
        b->parent_ = a_r;
        a_r->Upd();
        return a_r;
    }

    std::pair <Node*, Node*> Split(Node* a, bool includes) {
        a->Splay();
        if (includes) {
            Node* a_r = a->right_;
            if (a_r) a_r->parent_ = nullptr;
            a->right_ = nullptr;
            a->Upd();
            if (a_r) a_r->Upd();
            return {a, a_r};
        } else {
            Node* a_l = a->left_;
            if (a_l) a_l->parent_ = nullptr;
            a->left_ = nullptr;
            a->Upd();
            if (a_l) a_l->Upd();
            return {a_l, a};
        }
    }
    
    void Splay(Node* a) {
        a->Splay();
        root_ = a;
    }
    
    void Insert(int i, ll a) {
        Node* new_node = new Node(a, nullptr);
        if (root_ == nullptr) {
            root_ = new_node;
            return;
        }
        Tree* tree = new Tree();
        tree->Insert(0, a);
        auto trees = Split(i);
        Merge(trees.first, tree);
        Merge(tree, trees.second);
    }
    
    void Delete(int i) {
        Splay(root_);
        Node* node = root_->Find(i);
        node->Splay();
        Node* left = node->left_;
        Node* right = node->right_;
        if (left) left->parent_ = nullptr;
        if (right) right->parent_ = nullptr;
        root_ = Merge(left, right);
    }
    
    //first member contains i elems
    std::pair <Tree*, Tree*> Split(int i) {
        Splay(root_);
        if (i == 0) {
            return {nullptr, this};
        }
        if (i == GetSize()) {
            return {this, nullptr};
        }
        Tree *left = new Tree(), *right = new Tree();
        auto nodes = Split(root_->Find(i), false);
        left->root_ = nodes.first;
        right->root_ = nodes.second;
        return {left, right};
    }
    
    Tree* Merge(Tree* a, Tree* b) {
        if (a == nullptr) return b;
        if (b == nullptr) return a;
        Tree* tree = new Tree();
        tree->root_ = Merge(a->root_, b->root_);
        return tree;
    }
    
    ll GetSum(int l, int r) {
        ll sum;
        auto trees1 = Split(l);
        auto trees2 = trees1.second->Split(r - l + 1);
        
        trees2.first->root_->Splay();
        sum = trees2.first->root_->GetSum();
        
        Merge(trees2.first, trees2.second);
        Merge(trees1.first, trees1.second);
        return sum;
    }
    
    void Add(int l, int r, ll add) {
        auto trees1 = Split(l);
        auto trees2 = trees1.second->Split(r - l + 1);
        auto tree = trees2.first;
        
        tree->root_->Splay();
        tree->root_->Add(add);
        
        Merge(trees2.first, trees2.second);
        Merge(trees1.first, trees1.second);
        return;
    }
    
    void MakeEqualTo(int l, int r, ll u) {
        auto trees1 = Split(l);
        auto trees2 = trees1.second->Split(r - l + 1);
        auto tree = trees2.first;
        
        tree->root_->Splay();
        tree->root_->MakeEqualTo(u);
        
        Merge(trees2.first, trees2.second);
        Merge(trees1.first, trees1.second);
        return;
    }
    
    int MaxDecreasingSuffix() {
        Splay(root_);
        int sz = root_->size_;
        
        auto last = root_->Find(sz - 1);
        Splay(last);
        
        auto l = last->left_;
        if (l) l->Upd();
        
        while (true) {
            last->Upd();
            if (!l) {
                Splay(last);
                return sz - last->GetIndex();
            }
            l->Upd();
            
            Node* lr = l->right_;
            if (lr) lr->Upd();
            
            if (l->left_) l->left_->Upd();
            
            if (l->decreasing_ &&
                l->min_ >= last->key_) {
                //L subtreee is decreasing
                last = l;
                l = last->left_;
            } else {
                if (lr == nullptr || (lr->decreasing_ && lr->min_ >= last->key_)) {
                    //nice LR (can be included)
                    
                    //check if L can be in
                    if ((lr == nullptr || l->key_ >= lr->max_) && l->key_ >= last->key_) {
                        //L is in - we're going to the left
                        last = l;
                        l = l->left_;
                        continue;
                    } else {
                        //L is out
                        Splay(last);
                        return sz - l->GetIndex() - 1;
                    }
                } else {
                    //checking LR
                    l = lr;
                    continue;
                }
            }
        }
    }
    
    int MaxIncreasingSuffix() {
        Splay(root_);
        int sz = root_->size_;
        
        auto last = root_->Find(sz - 1);
        Splay(last);
        
        auto l = last->left_;
        if (l) l->Upd();
        
        while (true) {
            last->Upd();
            if (!l) {
                last->Splay();
                return sz - last->GetIndex();
            }
            l->Upd();
            Node* lr = l->right_;
            if (lr) lr->Upd();
            if (l->left_) l->left_->Upd();
            if (l->increasing_ &&
                l->max_ <= last->key_) {
                //L subtreee is increasing
                last = l;
                l = last->left_;
            } else {
                if (lr == nullptr || (lr->increasing_ && lr->max_ <= last->key_)) {
                    //nice LR (can be included)
                    
                    //check if L can be in
                    if ((lr == nullptr || l->key_ <= lr->min_) && l->key_ <= last->key_) {
                        //L is in - we're going to the left
                        last = l;
                        l = l->left_;
                        continue;
                    } else {
                        //L is out
                        last->Splay();
                        return sz - l->GetIndex() - 1;
                    }
                } else {
                    //checking LR
                    l = lr;
                    continue;
                }
            }
        }
    }
    
    // first less
    /// tree must be decreasing or increasing
    Node* LowerBound(ll u) {
        Splay(root_);
        return root_->LowerBound(u);
    }
    
    // first greater
    /// tree must be decreasing or increasing
    Node* UpperBound(ll u) {
        Splay(root_);
        return root_->UpperBound(u);
    }
    
    void NextPermutation() {
        Splay(root_);
        if (root_->decreasing_) {
            root_->Rotate();
            return;
        }
        
        int k = MaxDecreasingSuffix();
        int sz = GetSize();
        auto trees = Split(sz - k);
        
        Node* to_change = trees.first->root_->Find(sz - k - 1);
        
        Node* change_to = trees.second->root_;
        change_to->Splay();
        if (change_to->min_ == change_to->max_) {
            change_to = change_to->Find(change_to->size_ - 1);
        } else {
            change_to = change_to->UpperBound(to_change->key_);
        }
        
        Splay(to_change);
        Splay(change_to);
        
        std::swap(to_change->key_, change_to->key_);
        
        to_change->Upd();
        change_to->Upd();
        
        change_to->Rotate();
        Merge(trees.first, trees.second);
    }
    
    void PrevPermutation() {
        
        Splay(root_);
        
        if (root_->increasing_) {
            root_->Rotate();
            return;
        }
        
        int k = MaxIncreasingSuffix();
        int sz = GetSize();
        auto trees = Split(sz - k);
        
        Node* to_change = trees.first->root_->Find(sz - k - 1);
        Node* change_to = trees.second->root_;
        change_to->Splay();
        if (change_to->min_ == change_to->max_) {
            change_to = change_to->Find(change_to->size_ - 1);
        } else {
            change_to = change_to->LowerBound(to_change->key_);
        }
        //Node* change_to = trees.second->LowerBound(to_change->key_);
        
        Splay(to_change);
        Splay(change_to);
        
        std::swap(to_change->key_, change_to->key_);
        
        to_change->Upd();
        change_to->Upd();
        
        change_to->Rotate();
        Merge(trees.first, trees.second);
    }
    
    void NextPermutation(int l, int r) {
        auto trees1 = Split(l);
        auto trees2 = trees1.second->Split(r - l + 1);
        auto tree = trees2.first;
        
        tree->NextPermutation();
        
        Merge(trees2.first, trees2.second);
        Merge(trees1.first, trees1.second);
        return;
    }
    
    void PrevPermutation(int l, int r) {
        auto trees1 = Split(l);
        auto trees2 = trees1.second->Split(r - l + 1);
        auto tree = trees2.first;
        
        tree->PrevPermutation();
        
        Merge(trees2.first, trees2.second);
        Merge(trees1.first, trees1.second);
        return;
    }
    
    void Print() {
        Splay(root_);
        root_->Print();
        std::cout << std::endl;
    }
};

signed main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    Tree* tree = new Tree();
    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        ll u;
        std::cin >> u;
        tree->Insert(tree->GetSize(), u);
    }
    int q;
    std::cin >> q;
    while (q--) {
        int type;
        std::cin >> type;
        if (type == 1) {
            int l, r;
            std::cin >> l >> r;
            std::cout << tree->GetSum(l, r) << '\n';
        }
        if (type == 2) {
            ll x;
            int pos;
            std::cin >> x >> pos;
            tree->Insert(pos, x);
        }
        if (type == 3) {
            int pos;
            std::cin >> pos;
            tree->Delete(pos);
        }
        if (type == 4) {
            ll x;
            int l, r;
            std::cin >> x >> l >> r;
            tree->MakeEqualTo(l, r, x);
        }
        if (type == 5) {
            ll x;
            int l, r;
            std::cin >> x >> l >> r;
            tree->Add(l, r, x);
        }
        if (type == 6) {
            int l, r;
            std::cin >> l >> r;
            tree->NextPermutation(l, r);
        }
        if (type == 7) {
            int l, r;
            std::cin >> l >> r;
            //l = rand()%3;
            //r = rand()%3 + 2;
            //std::cout << l << " " << r << '\n';
            tree->PrevPermutation(l, r);
            //tree->Print();
        }
    }
    tree->Print();
    return 0;
}
