/* 
 * File:   Abstract.h
 * Author: Brynn Caddel
 * Created on June 2, 2015, 9:26 PM
 */

#ifndef AbstractClass
#define AbstractClass

class Abstract {
    virtual void config() =0;
    virtual void print() const = 0;
    virtual void vRows(int)=0;
    virtual void vColmn(int)=0;
    virtual int retRows() const =0;
    virtual int retColmn() const =0;
};

#endif