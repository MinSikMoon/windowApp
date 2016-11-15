#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

class Person {
	int age;
	char name[100];

public:
	Person() { cout << "!!!!! i'm a parent constructor !!!!! " << endl; }
	//1. constructor, destructor
	Person(int _age, char* _name):age(_age) {
		cout << "!!!!! i'm a parent constructor !!!!! " << endl;
		strcpy(name, _name);
	}
	~Person() {
		cout << "%%parent destructor called!!!!" << endl;
	}
	//2. SHOW NAME
	void showName() {
		cout << name << endl;
	}

	//3. show age
	void showAge() {
		cout << age << endl;
	}
};
//////////////////////////person ��ӹ��� univ Person
class UnivPerson : public Person {
private: //�̸�, ���� �� �߰� ��� : ����
	char major[50];

public: //�߰� �޼ҵ� : �̸�,����,���� �Ѳ����� showInfo
	//1. constructor
	UnivPerson() { cout << "$$$$$ i'm a child constructor" << endl; };
	UnivPerson(int _age, char* _name, char* _major) : Person(_age,_name) { //��� �޾Ҵ��� person�� private���� ����� ����� ���� ���� �Ұ���
		//���� person
		strcpy(major, _major);
	}
	~UnivPerson() {
		cout << "child destructor called!!!!!~~~~" << endl;
	}

	//2. method
	void showInfo() {
		showName();
		showAge();
		cout << major << endl;
	}


};

int main() {
	UnivPerson u1;

	system("pause");
	return 0;
}
