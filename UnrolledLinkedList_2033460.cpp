#include"UnrolledLinkedList.h"

void UnrolledLinkedList::add(int val) {
    Node *node = NULL;

    //Is list empty? => create new node and add val in element.
    if(head == NULL){
        node = new Node(nodeSize);
        node->add(val);
        head = tail = node;
        numOfNodes++;
        size1++;
    }
    else { //get last node and add val in element
        node = tail;

        if (!node->isFull()){
            node->add(val);
            size1++;
        }
        else{
            //number of element move to new node
            int n = node->getHalfNodeSize() - 1;
            int moveFrom = node->maxElements - n;

            Node *newNode = new Node(nodeSize);
            memmove(newNode->elements, node->elements + moveFrom, n*sizeof(int));
            newNode->numElements = n;
            newNode->add(val);
            size1++;

            node->numElements = moveFrom;
            node->next = newNode;
            newNode->prev = node;
            tail = newNode;
            numOfNodes++;
        }
    }
}

int UnrolledLinkedList::getAt(int pos) {
    // check whether pos is valid or not
    if (pos < 0 || pos >= size1){
		throw "IndexOutOfBoundsException";
		return 0;
    }

    //check list is null
    Node *p = head;
    if (p == NULL){
        throw "EmptyListExeception";
        return 0;
    }

    //Search element
    while (p != NULL) {
        pos -= p->numElements;
        if (pos < 0)
            return p->elements[p->numElements + pos];

        p = p->next;
    }
}

void UnrolledLinkedList::setAt(int pos, int val) {
	// check whether pos is valid or not
    if (pos < 0 || pos >= size1){
		throw "IndexOutOfBoundsException";
		return;
    }

    //check list is null
    Node *p = head;
    if (p == NULL){
        throw "EmptyListExeception";
        return;
    }

    //Search element
    while (p != NULL) {
        pos -= p->numElements;
        if (pos < 0){
            p->elements[p->numElements + pos] = val;
            return;
        }

        p = p->next;
    }
}


void UnrolledLinkedList::insertAt(int pos, int val) {
	if (pos < 0 || pos > size1)
		throw "IndexOutOfBoundsException"; // check whether pos is valid or not

	// TODO
    if(pos == size1) {
        this->add(val);
        return;
    }

    //search node contain pos element
    Node *p = head;
    while (p != NULL){
        pos -= p->numElements;
        if (pos < 0){ //index found
            int index = p->numElements + pos;

            //node is not full
            if(!p->isFull()) {
                p->insertAt(index, val);
                size1++;
                return;
            }
            else{ //Node is full
                //Remove the last element of array
                int temp = p->elements[p->numElements - 1];
                p->removeAt(p->numElements - 1);

                //Insert val
                p->insertAt(index, val);

                //Create new node
                Node *newNode = new Node(nodeSize);
                numOfNodes++;

                //number of element move to new node
                int n = p->getHalfNodeSize() - 1;
                int moveFrom = p->maxElements - n;

                //move final half of tail into new node
                memmove(newNode->elements, p->elements + moveFrom, n*sizeof(int));
                newNode->numElements = n;
                newNode->add(temp);
                size1++;

                newNode->next = p->next;
                p->next = newNode;
                newNode->prev = p;
                if(newNode->next != NULL)
                    newNode->next->prev = newNode;
                if(p == tail)
                    tail = newNode;
                return;
            }
        }

        p = p->next;
    }
}

void UnrolledLinkedList::deleteAt(int pos)
{
	if (pos < 0 || pos >= size1)
		throw "IndexOutOfBoundsException"; // check whether pos is valid or not

	// TODO
    //if Empty List => throw exeception
    Node *p = head;
    if (p == NULL)
        throw "EmptyListExeception";

    //if list has only node
    if (numOfNodes == 1){
        p->removeAt(pos);
        //if node has no element
        if (p->numElements == 0){
            delete p;
            head = tail = NULL;
            size1 = numOfNodes = 0;
        }
    }
    else{ //list has more only node
        //search node contain delete element
        int index = 0;
        while (p != NULL){
            for(int i=0; i < p->numElements; i++){
                if (index == pos){ //index found
                    if (p->numElements >  p->getHalfNodeSize()) {
                        p->removeAt(i);
                        return;
                    }
                    else{
                        //check if has prev node
                        if (p->prev != NULL){
                            //Yes > halfNodeSize phan tu
                            if((p->prev->numElements > p->getHalfNodeSize())){
                                p->removeAt(i);
                                p->insertAt(0, p->prev->elements[p->prev->numElements - 1]);
                                p->prev->removeAt(p->prev->numElements - 1);
                                return;
                            }
                        }
                        //check if has next node
                        else if (p->next != NULL){
                            //Yes > halfNodeSize phan tu
                            if (p->next->numElements >  p->getHalfNodeSize()) {
                                p->removeAt(i);
                                p->add(p->next->elements[0]);
                                p->next->removeAt(0);
                                return;
                            }
                        }
                        else if(p->prev != NULL){
                            p->removeAt(i);
                            for (int j = 0; j < p->numElements; j++)
                                p->prev->add(p->elements[j]);

                            //delete p node
                            Node *delNode = p;
                            if (delNode->next != NULL)
                                delNode->next->prev = delNode->prev;
                            else
                                tail = delNode->prev;

                            if (delNode->prev != NULL)
                                delNode->prev->next = delNode->next;

                            delete delNode;
                            numOfNodes--;
                            return;
                        }
                        else if(p->next != NULL){
                            p->removeAt(i);
                            for (int j = 0; j < p->next->getHalfNodeSize(); j++)
                                p->add( p->next->elements[j]);

                            //Delete next node
                            Node *delNode =  p->next;
                            if (delNode->next != NULL)
                                delNode->next->prev = delNode->prev;
                            else
                                tail = delNode->prev;

                            if (delNode->prev != NULL)
                                delNode->prev->next = delNode->next;

                            delete delNode;
                            numOfNodes--;
                            return;
                        }
                    }
                }
                index++;
            }
            p = p->next;
        }
    }
    size1--;
}

int UnrolledLinkedList::firstIndexOf(int val) {
	// TODO
    //if Empty List => throw exeception
    Node *p = head;
    if (p == NULL)
        throw "EmptyListExeception";

    int pos = 0;
    while (p != NULL) {
        for (int i = 0; i < p->numElements; i++){
            if (p->elements[i] == val)
                return pos;
            pos++;
        }
        p = p->next;
    }

    //val is not found
    return -1;
}

int UnrolledLinkedList::lastIndexOf(int val) {
	// TODO
	//if Empty List => throw exeception
    Node *p = tail;
    if (p == NULL)
        throw "EmptyListExeception";

    int pos = size1 - 1;
    while (p != NULL) {
        for (int i = p->numElements - 1; i > 0 ; i--){
            if (p->elements[i] == val)
                return pos;
            pos--;
        }
        p = p->prev;
    }

    //val is not found
    return -1;
}

bool UnrolledLinkedList::contains(int val) {
	Node *p = head;
    if (p == NULL)
        return false;

    while (p != NULL) {
        for (int i = 0; i < p->numElements; i++)
            if (p->elements[i] == val)
                return true;

        p = p->next;
    }

    return false;
}

void swap(Node* &a, Node* &b) {
    Node *tmp = a;
    a = b;
    b = tmp;
}

void UnrolledLinkedList::reverse1() {
	// TODO
    if (head == NULL) return;
    //swap head and tail
    swap(head, tail);

    for(Node *pNode = tail; pNode != NULL; pNode = pNode->prev) {
        swap(pNode->next, pNode->prev);
        pNode->reverse();
    }
}

