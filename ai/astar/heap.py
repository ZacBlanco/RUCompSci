import heapq
import itertools
import math

class TileHeap(object):
    REMOVED = "NULL_TILE"
    
    def __init__(self, min_tb=False):
        '''Create a new TileHeap

        Arguments:
            min_tb=False (boolean): If specified, tiebreaks in the heap will be done with minimum g_scores
        '''
        self.heap_list = BinaryHeap()
        self.heap_dict = {}
        self.ctr = itertools.count()
        self.min_tiebreaker = min_tb

    def __len__(self):
        return len(self.heap_dict)

    def __contains__(self, item):
        return item in self.heap_dict

    def peek(self):
        return self.heap_list.peek()

    def __getitem__(self, key):
        return self.heap_dict[key]

    def push(self, tile, gscore, hscore):
        if tile in self.heap_dict:
            self.remove(tile)
        tid = next(self.ctr)
        ent = []
        if self.min_tiebreaker:
            ent = [gscore+hscore, gscore, tid, tile]
        else:
            ent = [gscore+hscore, 100000 - gscore, tid, tile]
        self.heap_dict[tile] = ent
        self.heap_list.push(ent)

    def remove(self, tile_ent):
        ent = self.heap_dict.pop(tile_ent)
        ent[3] = TileHeap.REMOVED

    def pop(self):
        while self.heap_list:
            fscore, gscore, tid, tile = self.heap_list.pop()
            if tile is not TileHeap.REMOVED:
                self.heap_dict.pop(tile)
                return fscore, gscore, tile
        raise KeyError("Attempted to pop empty priority queue")

class BinaryHeap(object):

    def __init__(self):
        self.h = []
        self.minheap = True

    def push(self, item):
        self.h.append(item)
        self.__sift_up__()

    def pop(self):
        if len(self.h) == 1:
            return self.h.pop()
        v = self.h[0]
        self.h[0] = self.h.pop()
        self.__sift_down__()
        return v

    def peek(self):
        return self.h[0]
    
    @staticmethod
    def _parent(i):
        return int((i-1)/2)
    
    @staticmethod
    def _left_child(k):
        return 2*k + 1
    
    @staticmethod
    def _right_child(k):
        return 2*k + 2

    def _compare_nodes(self, i, j):
        '''Compares two nodes in the heap.

        Arguments:
            i: Index of in the heap list
            j: Index in the heap list

        Returns:
            -1: if val(i) < val(j)
             0: if val(i) == val(j)
             1: if val(i) > val(j)
        '''
        n1 = self.h[i]
        n2 = self.h[j]
        if isinstance(n1, tuple) and isinstance(n2, tuple):
            if len(n1) != len(n2):
                raise ValueError("Heap nodes as tuples must have the same number of values. Got lengths {} and {}".format(len(n1), len(n2)))
            i = 0
            while i < len(n1):
                if n1[i] < n2[i]:
                    return -1
                elif n1[i] > n2[i]:
                    return 1
                i += 1
            return 0

        elif n1.__class__ == n2.__class__:
            if n1 < n2:
                return -1
            elif n1 > n2:
                return 1
            return 0
        else:
            raise ValueError("Heap variables must be of the same type and/or length")



    def __sift_up__(self):
        curr = len(self.h) - 1
        parent = BinaryHeap._parent(curr)
        while curr > 0:
            c = self._compare_nodes(curr, parent)
            if self.minheap:
                if c < 0:
                    tmp = self.h[curr]
                    self.h[curr] = self.h[parent]
                    self.h[parent] = tmp
                else:
                    break
            else: # maxheap
                if c > 0:
                    tmp = self.h[curr]
                    self.h[curr] = self.h[parent]
                    self.h[parent] = tmp
                else:
                    break
            curr = parent
            parent = BinaryHeap._parent(curr)


    def __sift_down__(self):
        curr = 0
        while curr < len(self.h):
            left = BinaryHeap._left_child(curr)
            right = BinaryHeap._right_child(curr)
            cl = self._compare_nodes(left, curr) if left < len(self.h) else 0
            cr = self._compare_nodes(left, curr) if right < len(self.h) else 0
            swp = curr
            if self.minheap:
                if cl < 0 and cr >= 0: # swap curr and left
                    swp = left
                elif cl >= 0 and cr < 0: # swap curr and right
                    swp = right
                elif cl < 0 and cr < 0: # must compre left and right now to figure which to swap
                    clr = self._compare_nodes(left, right)
                    swp = left if clr < 0 else right
                else:
                    break # curr is less than left and right children already
                
            else: # maxheap
                if cl > 0 and cr <= 0: # swap curr and left
                    swp = left
                    curr = left
                elif cl <= 0 and cr > 0: # swap curr and right
                    swp = right
                elif cl > 0 and cr > 0: # must compre left and right now to figure which to swap
                    clr = self._compare_nodes(left, right)
                    swp = left if clr > 0 else right
                else:
                    break # curr is less than left and right children already
            tmp = self.h[curr]
            self.h[curr] = self.h[swp]
            self.h[swp] = tmp
            curr = swp

if __name__ == "__main__":
    a = BinaryHeap()
    a.push(0)
    a.push(1)
    a.push(2)
    assert(a.pop() == 0)
    a.push(0)
    a.push(3)
    assert(a.pop() == 0)
    assert(a.pop() == 1)
    assert(a.pop() == 2)
    assert(a.pop() == 3)
    last = (0, 0, 0, 0)
    for i in range(20):
        a.push((i % 2, i*i, i*2, 4))
    
    for i in range(20):
        print(a.pop())

    
    