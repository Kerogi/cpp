#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <typeinfo>
#include <iomanip>
std::string demangle(const char *name);

#ifdef __GNUC__
#include <cxxabi.h>
std::string demangle(const char *name) {
    int     status = -1;
    char   *realname;
    realname = abi::__cxa_demangle(name, 0, 0, &status);
    
    std::string res(name);
    
    if(status == 0){
        res.assign(realname);
        delete realname;
    };
    
    return res;
}
#else 
#include <boost/core/demangle.hpp>

std::string demangle(const char *name) {
    return boost::core::demangle(name);
}
#endif

#define type_desc2(t)  #t
#define type_desc(t) type_desc2(t)

#define TYPE size_t


template<class T>
size_t type_string_length_info(std::ostream& os, const char* name_from_preprocessor=0)
{
    os << "type: ";
    if(name_from_preprocessor)
        os<<name_from_preprocessor<<" ==> ";
    os<<typeid(T).name()<<" ==> "<<demangle(typeid(T).name());
    os <<"\n  size in bytes: "<<sizeof(T);
    if(std::numeric_limits<T>::is_specialized) {
        
        size_t max_string_representation_length = ((std::numeric_limits<T>::is_integer)?std::numeric_limits<T>::digits10:std::numeric_limits<T>::max_digits10)+1+std::numeric_limits<T>::is_signed;

        os
        <<"\n                "<<'<'<<std::string(max_string_representation_length-2,'-')<<'>'
        <<"\n  min value:    "<<std::numeric_limits<T>::min()
        <<"\n  max value:    "<<std::numeric_limits<T>::max()    
        <<"\n  is signed:    "<<(std::numeric_limits<T>::is_signed?"yes":"no")
        <<"\n  digits10:     "<<std::numeric_limits<T>::digits10
        <<"\n  digits:       "<<std::numeric_limits<T>::digits
        <<"\n  max digits10: "<<std::numeric_limits<T>::max_digits10
        <<"\n  radix:        "<<std::numeric_limits<T>::radix
        << '\n'
        << '\n';
        return max_string_representation_length;
    } else {
        os<<" have no info about type\n";
    }
    return 0;
}

#define  TYPE_STR_INFO(t) type_string_length_info<t>(std::cout, type_desc(t));

int main()
{
    TYPE_STR_INFO(int);
    TYPE_STR_INFO(char);
    TYPE_STR_INFO(unsigned char);
    TYPE_STR_INFO(unsigned long long);
    TYPE_STR_INFO(size_t);
    TYPE_STR_INFO(ptrdiff_t);
    //TYPE_STR_INFO(std::nullptr_t);

    return 0;
}