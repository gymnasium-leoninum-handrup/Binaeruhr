//Author: Oliver Enes (c) 2017 - Arduino optimated
/*
*	Simple class for managing values with a dynamic array
*/

//BEGIN template class vector 
template <class T> class vector
{
public:
	//BEGIN Constructor
	vector(int len = 0)
	{
		//This is the main part of the class. It is the dynamic array, declared as pointer.
		pArrayPointer = new T[len]; //Create Array with length of Constructor
		size = len; //Set vector size to length

		//Set all to 0
		memset(pArrayPointer, 0, sizeof(T) * size);
	}
	//END Constructor

	//BEGIN Destructor
	~vector()
	{
		delete[] pArrayPointer; //Delete all elements of our array-pointer
	}
	//END Destructor

	//BEGIN operator +=
	void operator +=(T &rhs)
	{
		Push_Back(rhs); //Add new element to vector
	}
	//END operator +=

	//BEGIN Push_Back()
	void Push_Back(T value)
	{
		T *tmp = new T[size + 1];	//	Create temp array
		//tmp[size] = value;	//	set last element to the new element

		//copy all elements to temp array
		//memcpy(tmp, pArrayPointer, sizeof (*pArrayPointer));


   tmp[size] = value;  //  set last element to the new element

   for(int i = 0; i<size; i++)
   {
    tmp[i] = pArrayPointer[i];
   }

		delete[] pArrayPointer;	//	delete all content of our array, because we saved it
		//Fill the new array
		pArrayPointer = tmp;
		size++;	//new size --> count variable up
	}

	//BEGIN remove(int index)
	void remove(int index)
	{
		T* tmp = new T[size - 1];	//create new temp array with the new size

		int x = 0; //Helper Variable for temp array
		//Copy content to tmp array
		for (int i = 0; i < size; i++)
		{
			if (i == index) // if we are on the index, which schould be removed, pass and dont copy
				continue;
			tmp[x] = pArrayPointer[i];
			x++;
		}

		delete[] pArrayPointer; //Remove content from old array

		//fill new array
		pArrayPointer = tmp;

		size--; // set new size
	}
	//END remove(int index)

	//BEGIN Size()
	int Size()
	{
		return size; //return size of vector
	}
	//END Size()

	//BEGIN checkptr()
	void checkptr() // Task: if we have a null pointer in our vector, remove it (Added for Arduino project)
	{
		vector<int> remArr; // vector with the indecies to remove

		//Check, if null pointer
		for (int i = 0; i < size; i++)
		{
			if (pArrayPointer[i] == 0)
				remArr.Push_Back(i);
		}

		int count = 0; //Helpervariable for removing the entries
		//Remov null pointer
		for (int k = 0; k < remArr.Size(); k++)
		{
			remove(remArr[k - count]);
			count++;
		}
	}
	//END checkptr

	//BEGIN operator []
	T& operator [](int number)
	{
		return pArrayPointer[number]; //Return entry at given position
	}
	//END operator[]

	//BEGIN change(int index, T newValue)
	void change(int index, T newValue) // Task: set entry of given index
	{
		pArrayPointer[index] = newValue; // set new entry of given index
	}
	//END change(int index, T newValue)

private:
	T* pArrayPointer; // Array pointer --> the herart of our vector
	int size;	// size of vector
};
//END template class vector


