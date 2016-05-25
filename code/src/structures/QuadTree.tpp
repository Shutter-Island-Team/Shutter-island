/**
 * @file QuadTree.tpp
 *
 * @see QuadTree.tpp
 */

template <typename T, typename S>
QuadTree<T, S>::QuadTree(T newContent,
		      S *newTLChild, S *newTRChild,
		      S *newBLChild, S *newBRChild) :
    content{newContent},
    topLeftChild{newTLChild},    topRightChild{newTRChild},
    bottomLeftChild{newBLChild}, bottomRightChild{newBRChild}
{}




template <typename T, typename S>
T QuadTree<T, S>::getContent() {
    return content;
}

template <typename T, typename S>
void QuadTree<T, S>::setContent(T newContent) {
    content = newContent;
}



template <typename T, typename S>
S* QuadTree<T, S>::getChild(QuadPosition position) {
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

template <typename T, typename S>
void QuadTree<T, S>::setChild(QuadPosition position, S* newChild) {
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
