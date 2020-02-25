#ifndef __SPLIT_HPP
#define __SPLIT_HPP

#include "vector.hpp"
namespace oe {
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

  
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
