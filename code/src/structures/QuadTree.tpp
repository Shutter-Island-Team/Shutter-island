/**
 * @file QuadTree.tpp
 *
 * @see QuadTree.tpp
 */

template <typename T>
QuadTree<T>::QuadTree(T newContent,
		      QuadTree<T> *newTLChild, QuadTree<T> *newTRChild,
		      QuadTree<T> *newBLChild, QuadTree<T> *newBRChild) :
    content{newContent},
    topLeftChild{newTLChild},    topRightChild{newTRChild},
    bottomLeftChild{newBLChild}, bottomRightChild{newBRChild}
{}




template <typename T>
T QuadTree<T>::getContent() {
    return content;
}

template <typename T>
void QuadTree<T>::setContent(T newContent) {
    content = newContent;
}



template<typename T>
QuadTree<T>* QuadTree<T>::getChild(QuadPosition position) {
    switch(position) {
    case TopLeft:
	return topLeftChild;
	break;
    case TopRight:
	return topRightChild;
	break;
    case BottomLeft:
	return bottomLeftChild;
	break;
    case BottomRight:
	return bottomRightChild;
	break;
    }
}

template <typename T>
void QuadTree<T>::setChild(QuadPosition position, QuadTree<T>* newChild) {
    switch(position) {
    case TopLeft:
	topLeftChild = newChild;
	break;
    case TopRight:
	topRightChild = newChild;
	break;
    case BottomLeft:
	bottomLeftChild = newChild;
	break;
    case BottomRight:
	bottomRightChild = newChild;
	break;
    }
}
