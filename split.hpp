#ifndef __SPLIT_HPP
#define __SPLIT_HPP

#include "vector.hpp"
namespace oe {
String* splitme(String target, String split)
{
    vector<String> arr;

    int x = 0;
    for (int i = 0; i < target.length(); i++)
    {
        if (target.charAt(i) == split.charAt(x))
        {
            if (x == split.length() - 1)
            {
                if (arr.Size() > 1)
                    arr.remove(arr.Size() - 1);
                arr.Push_Back(target.substring(0, i - split.length() + 1));
                target = target.substring(i + 1);
                arr.Push_Back(target);
                i = 0;
                x = 0;
            }
            else
                x++;
        }
        else
        {
            x = 0;
        }
    }

    String* result = new String[arr.Size()];
    for (int i = 0; i < arr.Size(); i++)
    {
        result[i] = arr[i];
    }

    return result;
}
}

#endif
