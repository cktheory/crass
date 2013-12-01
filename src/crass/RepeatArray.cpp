//
//  DRArray.cpp
//  crass
//
//  Created by Connor Skennerton on 29/10/13.
//  Copyright (c) 2013 Australian Centre for Ecogenomics. All rights reserved.
//

#include "RepeatArray.h"
#include <iostream>

using namespace crass;

int RepeatArray::numberOfRepeats() {
    int len = static_cast<int>(positions.size());
    return len / 2;
}

int RepeatArray::numberOfSpacers() {
    int len = static_cast<int>(positions.size());
    return (len / 2) - 1;
}

void RepeatArray::reverseRepeatPositions(int finalIndex) {
    int size = static_cast<int>(positions.size()) - 1;
    for (int i = 0, j = size - i; i < j; ++i, --j) {
        SWAP(positions[i], positions[j]);
        positions[i] = finalIndex - positions[i] + 1;
        positions[j] = finalIndex - positions[j] + 1;
    }
}

RepeatArray::RepeatIterator RepeatArray::repeatAt(int i) {
    if (i <= static_cast<int>(positions.size()/2)) {
        return RepeatIterator(positions.begin()+ (i * 2));
    } else {
        return repeatEnd();
    }
}

RepeatArray::SpacerIterator RepeatArray::spacerAt(int i) {
    if (i <= static_cast<int>((positions.size()/2) - 1)) {
        return SpacerIterator(positions.begin() + 1 + (i * 2));
    } else {
        return spacerEnd();
    }
}

int RepeatArray::repeatLengthAt(int i) {
    auto it = repeatAt(i);
    return (*it).second - (*it).first;
}

int RepeatArray::spacerLengthAt(int i) {
    auto it = spacerAt(i);
    return (*it).second - (*it).first;
}

std::ostream& crass::operator<<(std::ostream &output, RepeatArray& data) {
    
    if (data.empty()) {
        return output;
    }
    for(auto it = data.repeatBegin(); it != (data.repeatEnd() - 1); ++it ) {
        output << "("<<(*it).first<<","<<(*it).second<<"), ";
    }
    auto it = data.repeatEnd();
    it--;
    output << "("<<(*it).first<<','<<(*it).second<<')';
    return output;
}

#ifdef crass_RepeatArray_main
#include <cassert>
int main() {
    crass::RepeatArray r = crass::RepeatArray();
    r.add(1,3);
    r.add(7,12);
    std::cout<<"original:"<<std::endl;
    r.dump();
    
    std::cout << "number of repeats: "<<r.numberOfRepeats()<<std::endl;
    std::cout << "number of spacers: "<<r.numberOfSpacers()<<std::endl;
    
    std::cout << "Testing Repeat iterator:"<<std::endl;
    for (RepeatArray::RepeatIterator it2 = r.repeatBegin(); it2 != r.repeatEnd(); ++it2) {
        std::cout << (*it2).first <<","<< (*it2).second << std::endl;
    }
    std::cout << "Testing Spacer iterator:"<<std::endl;
    for (RepeatArray::SpacerIterator it3 = r.spacerBegin(); it3 != r.spacerEnd(); ++it3) {
        std::cout << (*it3).first <<","<< (*it3).second << std::endl;
    }
    
    std::cout<<"Testing modification:"<<std::endl;
    
    auto it = r.repeatBegin();
    auto it2 = r.repeatBegin();
    (*it).first = 0;
    assert(it == it2);
    assert((*it).first == (*it2).first);
    r.dump();
    
    std::cout<<"Testing operators:"<<std::endl;
    
    std::cout<<"right shift:"<<std::endl;
    std::cout << r << std::endl;
    
    std::cout<<"plus (+):";
    assert((*(it + 1)).first == 7);
    std::cout << " pass"<<std::endl;
    
    std::cout<<"plus equals (+=):";
    assert((it += 1) == (it2+1));
    assert((*it).first == 7);
    std::cout << " pass"<<std::endl;
    
    std::cout<<"minus (-):";
    assert((*(it - 1)).first == 0);
    std::cout << " pass"<<std::endl;
    
    std::cout<<"plus equals (-=):";
    assert((it -= 1) == it2);
    assert((*it).first == 0);
    std::cout << " pass"<<std::endl;
    
    std::cout<< "Testing reversal:"<<std::endl;
    r.reverseRepeatPositions(14);
    r.dump();
    
    std::cout<<"Testing repeat direct access:"<<std::endl;
    it = r.repeatAt(0);
    std::cout<<(*it).first <<","<<(*it).second;
    it = r.repeatAt(1);
    std::cout<<","<<(*it).first <<","<<(*it).second<<std::endl;

    std::cout<<"Testing spacer direct access:"<<std::endl;
    RepeatArray::SpacerIterator it3 = r.spacerAt(0);
    std::cout<<(*it3).first <<","<<(*it3).second<<std::endl;

    return 0;
}
#endif