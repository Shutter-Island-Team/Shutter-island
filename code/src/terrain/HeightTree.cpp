/**
 * @file HeightTree.cpp
 * 
 * @see HeightTree.hpp
 */
#include "../../include/terrain/MapUtils.hpp"
#include "../../include/terrain/HeightTree.hpp"

HeightTree::HeightTree(HeightNode content,
		       HeightTree *tlChild, HeightTree *trChild,
		       HeightTree *blChild, HeightTree *brChild) :
    QuadTree<HeightNode, HeightTree>(content,
				     tlChild, trChild,
				     blChild, brChild)
{}

// Freeing recursively the free
void HeightTree::freeHeightTree() {

    HeightTree *tlTree = this->getChild(TopLeft);
    if (tlTree) {
        tlTree->freeHeightTree();
        tlTree = NULL;
    }

    HeightTree *trTree = this->getChild(TopRight);
    if (trTree) {
        trTree->freeHeightTree();
        trTree = NULL;
    }

    HeightTree *blTree = this->getChild(BottomLeft);
    if (blTree) {
        blTree->freeHeightTree();
        blTree = NULL;
    }

    HeightTree *brTree = this->getChild(BottomRight);
    if (brTree) {
        brTree->freeHeightTree();
        brTree = NULL;
    }

    delete this;
}

// Simply test in wich subsquare the point is
HeightTree* HeightTree::locatePosition(Vertex2D & pos) {

    HeightNode content = this->getContent();

    Vertex2D posTL = content.getBlob(TopLeft).getPosition();
    Vertex2D posBR = content.getBlob(BottomRight).getPosition();

    float centerX = (posTL.first  + posBR.first)/2.0f;
    float centerY = (posTL.second + posBR.second)/2.0f;
    
    float posX = pos.first;
    float posY = pos.second;

    if (posY >= centerY) {
	if (posX <= centerX) {
	    return this->getChild(TopLeft);
	} else {
	    return this->getChild(TopRight);
	}
    } else {
	if (posX <= centerX) {
	    return this->getChild(BottomLeft);
	} else {
	    return this->getChild(BottomRight);
	}
    }
}

// Try to find if the point is already in the tree
bool HeightTree::findVertexHeight(Vertex2D & pos, int depth, float* height) {

    if (depth == 1) {
	// Check the node
	HeightNode content = this->getContent();
	HeightBlob blob = content.getBlob(TopLeft);
	if (distanceV2D(pos, blob.getPosition()) < DETECTION_THRESHOLD) {
	    *height =  blob.getHeight();
	    return true;
	}

	blob = content.getBlob(TopRight);
	if (distanceV2D(pos, blob.getPosition()) < DETECTION_THRESHOLD) {
	    *height =  blob.getHeight();
	    return true;
	}

	blob = content.getBlob(BottomLeft);
	if (distanceV2D(pos, blob.getPosition()) < DETECTION_THRESHOLD) {
	    *height =  blob.getHeight();
	    return true;
	}

	blob = content.getBlob(BottomRight);
	if (distanceV2D(pos, blob.getPosition()) < DETECTION_THRESHOLD) {
	    *height =  blob.getHeight();
	    return true;
	}
	return false;	    
    } else {
	// Else try to search deeper
	HeightTree* child = this->locatePosition(pos);
	if (!child)
	    return false;
	return child->findVertexHeight(pos, depth-1, height);
    }
}


void HeightTree::computeTree(voro::container & container, std::vector<Seed> seeds) {
    this->computeTreeInternal(container, seeds, 1, this);
}



void HeightTree::computeTreeInternal(voro::container & container, std::vector<Seed> seeds, 
				     int depth, HeightTree* root) {
    
    // Check the current level must be build or not
    if(!checkSubdivision(depth)) return;

    // Recuperating all the needed informations
    HeightNode content = this->getContent();

    HeightBlob tlBlob = content.getBlob(TopLeft);
    HeightBlob trBlob = content.getBlob(TopRight);
    HeightBlob blBlob = content.getBlob(BottomLeft);
    HeightBlob brBlob = content.getBlob(BottomRight);

    Biome tlBiome = findClosestBiome(tlBlob.getPosition(), container, seeds);
    Biome trBiome = findClosestBiome(trBlob.getPosition(), container, seeds);
    Biome blBiome = findClosestBiome(blBlob.getPosition(), container, seeds);
    Biome brBiome = findClosestBiome(brBlob.getPosition(), container, seeds);


    // Building now the 5 new blobs required to cut the square

    Vertex2D tlPos = tlBlob.getPosition();
    Vertex2D brPos = brBlob.getPosition();

    float tlX = tlPos.first;
    float tlY = tlPos.second;
    float brX = brPos.first;
    float brY = brPos.second;

    float size = brX - tlX;

    float centerX = (tlX + brX)/2.0f;
    float centerY = (tlY + brY)/2.0f;

    // Position of the blobs
    Vertex2D centerPos = Vertex2D(centerX, centerY);
    Vertex2D northPos  = Vertex2D(centerX, tlY);
    Vertex2D southPos  = Vertex2D(centerX, brY);
    Vertex2D eastPos   = Vertex2D(brX,     centerY);
    Vertex2D westPos   = Vertex2D(tlX,     centerY);

    // Their biome
    Biome centerBiome = findClosestBiome(centerPos, container, seeds);
    Biome northBiome  = findClosestBiome(northPos,  container, seeds);
    Biome southBiome  = findClosestBiome(southPos,  container, seeds);
    Biome eastBiome   = findClosestBiome(eastPos,   container, seeds);
    Biome westBiome   = findClosestBiome(westPos,   container, seeds);

    // Computing the height of the blobs
    // NB : Because the search is done in this order : TL -> TR -> BL -> BR
    // Only the norh and the west might have been already computed
    // Thus we only search them
    float centerHeight = biomeHeight(centerBiome);
    float northHeight;
    if (!(root->findVertexHeight(northPos, depth+1, &northHeight)))
	northHeight = biomeHeight(northBiome);
    float southHeight = biomeHeight(southBiome);
    float eastHeight  = biomeHeight(eastBiome);
    float westHeight;
    if (!(root->findVertexHeight(westPos, depth+1, &westHeight)))
	westHeight = biomeHeight(westBiome);
    
    // Computing the scale
    float centerScale = computeBlobScale(centerBiome, eastBiome, westBiome);
    float northScale  = computeBlobScale(northBiome,  tlBiome,   trBiome);
    float southScale  = computeBlobScale(southBiome,  blBiome,   brBiome);
    float eastScale   = computeBlobScale(eastBiome,   trBiome,   brBiome);
    float westScale   = computeBlobScale(westBiome,   tlBiome,   blBiome);

    // Finally the 5 blobs
    HeightBlob centerBlob = HeightBlob(centerPos, centerHeight, centerScale, centerBiome);
    HeightBlob northBlob  = HeightBlob(northPos,  northHeight,  northScale,  northBiome);
    HeightBlob southBlob  = HeightBlob(southPos,  southHeight,  southScale,  southBiome);
    HeightBlob eastBlob   = HeightBlob(eastPos,   eastHeight,   eastScale,   eastBiome);
    HeightBlob westBlob   = HeightBlob(westPos,   westHeight,   westScale,   westBiome);

    // Building now the 4 children of the current node
    float subSize = size/2.0f; 
    HeightTree *tlTree = new HeightTree(HeightNode(subSize,
						   tlBlob,     northBlob,
						   westBlob,   centerBlob));
    HeightTree *trTree = new HeightTree(HeightNode(subSize,
						   northBlob,  trBlob,
						   centerBlob, eastBlob));
    HeightTree *blTree = new HeightTree(HeightNode(subSize,
						   westBlob,   centerBlob,
						   blBlob,     southBlob));
    HeightTree *brTree = new HeightTree(HeightNode(subSize,
						   centerBlob, eastBlob,
						   southBlob,  brBlob));    

    // Setting them
    this->setChild(TopLeft,     tlTree);
    this->setChild(TopRight,    trTree);
    this->setChild(BottomLeft,  blTree);
    this->setChild(BottomRight, brTree);

    // Building now the next level
    tlTree->computeTreeInternal(container, seeds, depth+1, root);
    trTree->computeTreeInternal(container, seeds, depth+1, root);
    blTree->computeTreeInternal(container, seeds, depth+1, root);
    brTree->computeTreeInternal(container, seeds, depth+1, root);
    
}

float HeightTree::evalHeight(Vertex2D pos) {
    
    // Height generated by the 4 blobs of this level
    float height = this->getContent().evalHeight(pos);

    HeightTree *child = this->locatePosition(pos);
    // If the next level is defined
    if (child)
	// Then also bleding with the height generated by the next level
	height += HEIGHT_BLENDING_COEFFICIENT*child->evalHeight(pos);

    return height;
}
