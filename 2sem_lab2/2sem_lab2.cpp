#include <iostream>
#include <string>
#include <chrono>

struct Node
{
	int value;
	Node* next = nullptr;
	Node* prev = nullptr;

	explicit Node(const int val) : value(val)
	{
	}
};

class LinkedList
{
private:
	Node* head = nullptr;
	Node* tail = nullptr;
	int length = 0;

	static Node* createBefore(Node* node, const int val)
	{
		const auto newNode = new Node(val);
		newNode->next = node;
		newNode->prev = node->prev;
		if(node->prev != nullptr)
			node->prev->next = newNode;
		node->prev = newNode;
		return newNode;
	}

	static Node* createAfter(Node* node, int val)
	{
		const auto newNode = new Node(val);
		newNode->next = node->next;
		newNode->prev = node;
		if(node->next != nullptr)
			node->next->prev = newNode;
		node->next = newNode;
		return newNode;
	}

	bool deleteNode(Node* node)
	{
		if (node == nullptr) return false;
		if (node->prev != nullptr) node->prev->next = node->next;
		if (node->next != nullptr) node->next->prev = node->prev;
		if (node == head) head = head->next;
		if (node == tail) tail = tail->prev;
		--length;
		delete node;
		return true;
	}

public:
	Node* getHead() const
	{
		return head;
	}

	Node* getTail() const
	{
		return tail;
	}

	int getLength() const
	{
		return length;
	}

	Node* get(int pos) const
	{
		Node* ptr = head;
		while(pos > 0)
		{
			ptr = ptr->next;
			--pos;
		}
		return ptr;
	}

	int getIndex(const int val) const
	{
		Node* ptr = head;
		int iter = 0;
		while(ptr != nullptr)
		{
			if (ptr->value == val)
			{
				return iter;
			}
			iter++;
			ptr = ptr->next;

		}
		return -1;
	}

	Node* getByValue(const int val) const
	{
		Node* ptr = head;
		while(ptr != nullptr)
		{
			if (ptr->value == val) return ptr;
			ptr = ptr->next;
		}
		return nullptr;
	}

	bool insert(int num, int pos)
	{
		if (pos > length) return false;

		if(length == 0)
		{
			const auto node = new Node(num);
			head = node;
			tail = node;
		}
		else if (pos == length) 
		{
			tail = createAfter(tail, num);
		}
		else if (pos == 0) 
		{
			head = createBefore(head, num);
		}
		else
		{
			createBefore(get(pos), num);
		}
		++length;
		return true;
	}

	bool pushBack(const int num)
	{
		return insert(num, length);
	}

	bool pushFront(const int num)
	{
		return insert(num, 0);
	}

	bool deleteAt(const int pos)
	{
		return deleteNode(get(pos));
	}

	bool deleteVal(const int val)
	{
		return deleteNode(getByValue(val));
	}

	void clear()
	{
		auto node = head;
		while(node != nullptr)
		{
			const auto next = node->next;
			delete node;
			node = next;
		}
		length = 0;
	}

	~LinkedList()
	{
		clear();
	}
};

LinkedList list;

constexpr auto FILL_RANDOM = "random";
constexpr auto FILL = "create";
constexpr auto DELETE = "delete";
constexpr auto UPDATE = "update";
constexpr auto INSERT = "insert";
constexpr auto FIND = "find";
constexpr auto FIND_DELETE = "find_delete";
constexpr auto READ_POS = "read_pos";
constexpr auto READ = "read";
constexpr auto READ_REVERSE = "read_reverse";
constexpr auto EXIT = "exit";
constexpr auto HELP = "help";
constexpr auto ABOUT = "about";

inline void askFor(const char* item)
{
    std::cout << "\n" << item << " : ";
}

inline void printLine(const char* item)
{
	std::cout << "\n" << item;
}
void infoEndpoint()
{
	std::cout << "\nThis lab is about comparing performance of linked list";
	std::cout << "\nUser should be able to perform CRUD operations";
	std::cout << "\nUser should also see benchmarks";
	std::cout << "\nThis program stores numbers in this linked list";
	std::cout << "\nTo view all available operations, print " << HELP;
}

inline void printCommandDescription(const char * commandName, const char * desc)
{
	std::cout << "\n" << commandName << " - " << desc;
}
void helpEndpoint()
{
	std::cout << "\nCommands:";
	printCommandDescription(FILL_RANDOM, "Fill structure of fixed size N with random values");
	printCommandDescription(FILL, "Fill structure with values from console.");
	printCommandDescription(INSERT, "Insert value in specific position");
	printCommandDescription(READ_POS, "Read value from specific position");
	printCommandDescription(READ, "Read all values");
	printCommandDescription(READ_REVERSE, "Read all values in reverse order");
	printCommandDescription(FIND, "Get value position");
	printCommandDescription(UPDATE, "Update value in specific position");
	printCommandDescription(DELETE, "Delete value in specific position");
	printCommandDescription(FIND_DELETE, "delete specific value from structure");
	printCommandDescription(ABOUT, "View info about this program");
	printCommandDescription(EXIT, "Stop program execution");
}
void exitEndpoint()
{
	std::cout << "\nProgram execution stopped";
}

void fillRandomEndpoint()
{
	int n;
	askFor("amount of random elements");
	std::cin >> n;

	list.clear();
	for(int i = 0; i < n; ++i)
	{
		list.pushBack(rand() % 100);
	}
}

void fillEndpoint()
{
	std::cout << "\nPrint number to insert into linked list";
	std::cout << "\nTo stop this, enter a non-number value";
	askFor("number");
	std::string num;
	while (std::cin >> num)
	{
		try
		{
			const int n = stoi(num);
			const auto startedTime = std::chrono::steady_clock::now();
			list.pushBack(n);
			const auto endedTime = std::chrono::steady_clock::now();
			const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(endedTime - startedTime).count();
			std::cout << "\nInserting number " << n << " took " << microseconds << " microseconds.";
			askFor("number");
		}
		catch (std::invalid_argument&)
		{
			std::cout << "\nReceived a non-number value";
			std::cout << "\nStopping command execution";
			break;
		}
	}
}

void insertEndpoint()
{
	int pos, n;
	askFor("position");
	std::cin >> pos;
	askFor("value");
	std::cin >> n;

	const auto startedTime = std::chrono::steady_clock::now();
	list.insert(n, pos);
	const auto endedTime = std::chrono::steady_clock::now();
	const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(endedTime - startedTime).count();
	std::cout << "\nInserting number on position " << pos << " took " << microseconds << " microseconds.";
}

void readPosEndpoint()
{
	int pos;
	askFor("position");
	std::cin >> pos;

	const auto startedTime = std::chrono::steady_clock::now();
	const auto val = list.get(pos)->value;
	const auto endedTime = std::chrono::steady_clock::now();
	const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(endedTime - startedTime).count();
	std::cout << "\nvalue: " << val;
	std::cout << "\nreading number on position " << pos << " took " << microseconds << " microseconds.";
}

void deleteEndpoint()
{
	int pos;
	askFor("position");
	std::cin >> pos;

	const auto startedTime = std::chrono::steady_clock::now();
	list.deleteAt(pos);
	const auto endedTime = std::chrono::steady_clock::now();
	const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(endedTime - startedTime).count();

	std::cout << "\ndeleting node on position " << pos << " took " << microseconds << " microseconds.";
}

void findEndpoint()
{
	int val;
	askFor("value");
	std::cin >> val;

	const auto startedTime = std::chrono::steady_clock::now();
	const int pos = list.getIndex(val);
	const auto endedTime = std::chrono::steady_clock::now();
	const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(endedTime - startedTime).count();

	std::cout << "\nPosition of value (" << val << "): " << (pos == -1 ? "not found" : std::to_string(pos));
	
	std::cout << "\nSearching for position of the first encounter of value " << val << " took " << microseconds << " microseconds.";
}

void updateEndpoint()
{
	int pos, newValue;
	askFor("position");
	std::cin >> pos;
	askFor("new value");
	std::cin >> newValue;

	const auto startedTime = std::chrono::steady_clock::now();
	const auto node = list.get(pos);
	const auto prevValue = node->value;
	node->value = newValue;
	const auto endedTime = std::chrono::steady_clock::now();
	const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(endedTime - startedTime).count();

	std::cout << "\nUpdated element at position " << pos << "(" << prevValue << " -> " << newValue << ")";
	std::cout << "\nUpdating this element took " << microseconds << " microseconds.";
}

void findDeleteEndpoint()
{
	int val;
	askFor("value");
	std::cin >> val;

	const auto startedTime = std::chrono::steady_clock::now();
	const auto res = list.deleteVal(val);
	const auto endedTime = std::chrono::steady_clock::now();
	const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(endedTime - startedTime).count();

	if (res) std::cout << "\nDeleted item with value " << val;
	else std::cout << "\nCould not delete item with value " << val;

	std::cout << "\nDeleting this element took " << microseconds << " microseconds.";
}

void readAllEndpoint()
{
	if(list.getLength() == 0)
	{
		std::cout << "\nThere are no items in linked list";
		return;
	}
	std::cout << "\nvalues: (Head) ";
	const auto startedTime = std::chrono::steady_clock::now();
	auto node = list.getHead();
	while(node->next != nullptr)
	{
		std::cout << node->value << " <-> ";
		node = node->next;
	}
	std::cout << node->value << " (Tail)";
	const auto endedTime = std::chrono::steady_clock::now();
	const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(endedTime - startedTime).count();

	std::cout << "\nPrinting values of linked list took " << microseconds << " microseconds.";
}

void readAllReverseEndpoint()
{
	if(list.getLength() == 0)
	{
		std::cout << "\nThere are no items in linked list";
		return;
	}

	std::cout << "\nvalues: (Tail) ";
	const auto startedTime = std::chrono::steady_clock::now();
	auto node = list.getTail();
	while(node->prev != nullptr)
	{
		std::cout << node->value << " <-> ";
		node = node->prev;
	}
	std::cout << node->value << " (Head)";
	const auto endedTime = std::chrono::steady_clock::now();
	const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(endedTime - startedTime).count();

	std::cout << "\nPrinting values of linked list took " << microseconds << " microseconds.";
}

void processEndpoint(const char * endpoint)
{
	if (strcmp(endpoint,FILL_RANDOM) == 0) return fillRandomEndpoint();
	if (strcmp(endpoint,FILL) == 0) return fillEndpoint();
	if (strcmp(endpoint,INSERT) == 0) return insertEndpoint();
	if (strcmp(endpoint,READ_POS) == 0) return readPosEndpoint();
	if (strcmp(endpoint,READ) == 0) return readAllEndpoint();
	if (strcmp(endpoint,READ_REVERSE) == 0) return readAllReverseEndpoint();
	if (strcmp(endpoint,FIND) == 0) return findEndpoint();
	if (strcmp(endpoint,UPDATE) == 0) return updateEndpoint();
	if (strcmp(endpoint,DELETE) == 0) return deleteEndpoint();
	if (strcmp(endpoint,FIND_DELETE) == 0) return findDeleteEndpoint();
	if (strcmp(endpoint,HELP) == 0) return helpEndpoint();
	if (strcmp(endpoint,ABOUT) == 0) return infoEndpoint();
	if (strcmp(endpoint,EXIT) == 0) return exitEndpoint();

	std::cout << "\nCommand not found";
	std::cout << "\nPrint help to view list of all commands\n";
}

int main()
{
    std::string endpoint;
	infoEndpoint();
	srand(time(nullptr));
	bool shouldAskForYourCommand = true;
    while(endpoint != EXIT)
    {
		if(shouldAskForYourCommand) askFor("\nYour Command");
		std::getline(std::cin, endpoint);
		shouldAskForYourCommand = true;

		if (!endpoint.empty())
			processEndpoint(endpoint.c_str());
		else shouldAskForYourCommand = false;
    }
}
