//
// Created by archie on 4/17/17.
//

#ifndef BIN_EVO_PROJEKT_NUMBERGENERATOR_H
#define BIN_EVO_PROJEKT_NUMBERGENERATOR_H
#include <iostream>     // std::cout
#include <algorithm>    // std::next_permutation, std::sort

template<class _Tnumber, class _Titerator >
bool next_variation
        (
                _Titerator const& _First
                , _Titerator const& _Last
                , _Tnumber const& _Upper
                , _Tnumber const& _Start = 0
                , _Tnumber const& _Step  = 1
        )
{
    _Titerator _Next = _First;
    while( _Next  != _Last )
    {
        *_Next += _Step;
        if( *_Next < _Upper )
        {
            return true;
        }
        (*_Next) = _Start;
        ++_Next;
    }
    return false;
}

class NumberGenerator {

};



#endif //BIN_EVO_PROJEKT_NUMBERGENERATOR_H
