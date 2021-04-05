#include<string>

using string;

class string_plus{

public:
std::string  plus( string& a, string& b){

    string tmp_a = a;
    string tmp_b = b;

    tmp_a.reverse();
    tmp_b.reverse();
    
    string result;

    unsigned long len = min( a.length(), b.length());

    unsigned i = 0;

    unsigned carry = 0;

    while( len ){

        unsigned curr = (tmp_a[i]- '0') + (tmp_b[i]-'0') + carry;

        carry = curr/10;

        curr =  curr % 10;

        result[i] = '0'+ curr;

        i++;
        len--;

    }
    string *p = NULL;
    
    if (tmp_a.length()>i) p = &tmp_a;
    else{

        p = &tmp_b;

    }

    while (i < p->length()){

        unsigned curr = (*p[i]- '0') + carry;

        carry = curr/10;

        curr =  curr % 10;
        result[i] = '0'+ curr;

        i++;
    }
    
    if ( carry) 
        result[i] = '0'+ 1;


    result.reverse();

    return result;
}


};


long string_length( char* str){

    if (str == NULL) return -1;

    if (str && (*str == 0)) return 0;
    ++str;
    return 1 + string_length(str);

}

四万三千五百二十三 -> 43523 


long convert_chinese_to_number( unsigned short *input){

    if (input == NULL) return -1;

    if (*input =='\0' ) return -1;

    long ret = 0;

    while( *input!= '\0'){

        long number = convert(*input);

        ++input;

        long unit = 0;        
        if (*input !='\0'){

            unit = convert_to_unit(*input);

            ret += calculate_by_unit(number, unit):

            input++;

        }else{

            ret += number;

        }
    }

    return ret;
}



    }


}

