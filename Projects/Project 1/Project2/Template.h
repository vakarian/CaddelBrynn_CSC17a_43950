/* 
 * File:   Template.h
 * Author: Brynn
 *
 * Created on June 2, 2015, 9:30 PM
 */

#ifndef __Project_2__TemplateClass__
#define __Project_2__TemplateClass__

template<class TTemplate>
class BoardGame {
private:
    TTemplate* p;
public:
    BoardGame():p(0){}
    BoardGame(TTemplate* t):p(t){}
    ~BoardGame() {delete p;}
    BoardGame<TTemplate>& operator=(const TTemplate&);
    operator bool(){return p;}
    TTemplate* operator->() const;
    TTemplate& operator*() const;
};

template<class T>
T* BoardGame<T>::operator->() const {
    /// only return p if it points to something
    if (p)
        return p;
    return 0;
}

template<class T>
T& BoardGame<T>::operator*() const{
    if (p)
        return *p;
}

template<class T>
BoardGame<T>& BoardGame<T>::operator=(const T& rhs) {
    p = &rhs;
}

#endif /* defined(__Project_2__TemplateClass__) */