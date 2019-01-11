
template <class T>
class LinkedList
{
public:
	LinkedList() : first{nullptr}, last{nullptr}, size{0} {}
	virtual ~LinkedList() 
	{
		int SIZE = size;
		for (int i = 0; i < SIZE; ++i)
			remove(0);
	}

	void append(T data)
	{
		Link<T>* newL = new Link<T>{data, nullptr};
		if (first == nullptr)
			first = last = newL;
		else
		{
			last->next = newL;
			last = newL;
		}
		++size;
	}

	T remove(int index)
	{
		// INVALID INDEX
		if (index < 0 || index >= size)
			throw std::invalid_argument( "index < 0 || index >= size" );

		Link<T>* ptr;
		T tempData;

		// REMOVE FIRST
		if (index == 0)
		{
			ptr = first->next;
			tempData = first->data;
			delete first;
			first = ptr;
		}

		// REMOVE ANYWHERE
		else
		{
			ptr = first;
			int count = 0;
			while(count++ != (index - 1))
				ptr = ptr->next;

			Link<T>* temp = ptr->next->next;
			tempData = ptr->next->data;
			delete ptr->next;
			ptr->next = temp;
		}
		--size;
		return tempData;
	}

	void traverse(void(*work)(T data))
	{
		Link<T>* ptr = first;
		while(ptr != nullptr)
		{
			work(ptr->data);
			ptr = ptr->next;
		}
	}

	int getSize()
	{
		return size;
	}

	bool isEmpty()
	{
		return size == 0;
	}

private:

	// NOTE(moritz): Komische Kompilierungsfehler treten auf,
	// wenn "template <class T>" fehlt und das struct nach
	// den beiden Link Membern steht.
	template <class T>
	struct Link
	{
		Link(T data, Link<T>* next) : data{data}, next{next} {}
		~Link() {}

		T data;
		Link<T>* next;
	};

	Link<T>* first;
	Link<T>* last;
	int size;
};

template <class T>
class Queue : public LinkedList<T>
{
public:
	Queue() : LinkedList{} {}
	~Queue() {}

	void enqueue(T data)
	{
		append(data);
	}

	T dequeue()
	{
		return remove(0);
	}
};